// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled(); // 检查是否是本地控制：本地控制指的是玩家自己控制的角色，而不是其他玩家或AI控制的角色
	if (bIsLocallyControlled) // 如果是本地玩家输入控制操作，则将自己的操作TargetData发送到服务器
	{
		SendMouseCursorData(); // 发送鼠标光标数据
	}
	else // 否则就不是本地，则是服务器或者其他客户端，则等待并处理服务器发送过来的TargetData
	{
		/* 注册TargetDelegates */
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();		 // 获取能力规格句柄
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey(); // 获取激活预测键
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback); // 注册目标数据集委托，以便在目标数据被复制时调用回调函数

		/*检查是否已经到达，因为有可能数据先到，Task后创建，这样就可以阻止Ability被提前结束，标记Task正在等待远程数据，防止错过了已经到达的数据 */
		const bool bCalledDelegates = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey); // 检查是否已经调用了目标数据委托
		if (!bCalledDelegates)		// 如果没到达则等待
		{
			SetWaitingOnRemotePlayerData(); // 设置等待远程玩家数据标志，表示当前任务正在等待远程玩家的数据
		}
	}


}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	/* 设置预测窗口，告诉ASC接下来的操作是本地预测操作，需要GAS记录预测密钥PredictionKey */
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get()); // 创建一个作用域预测窗口，用于处理能力的预测执行，窗口范围是该变量的生命周期

	/* 获取鼠标命中结果 */
	FHitResult HitResult;
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();				// 获取当前能力的玩家控制器
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);	// 获取鼠标光标下的命中结果，使用可见性碰撞通道进行检测

	/* 创建TargetData存入DataHandle自动管理生命周期 */
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(); // 创建一个新的目标数据对象，该对象用于存储单个目标的命中信息
	FGameplayAbilityTargetDataHandle DataHandle;
	Data->HitResult = HitResult;					// 将获取到的命中结果赋值给目标数据对象
	DataHandle.Add(Data);							// 将目标数据对象添加到目标数据句柄中，通过句柄管理避免了New可能会出现的内存泄露等问题

	/* 从客户端提交TargetData到服务器 */
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),								// Ability规格句柄：返回当前能力的规格句柄
		GetActivationPredictionKey(),						// 激活预测键：返回当前能力激活的预测键
		DataHandle,											// 目标数据句柄：包含了目标数据对象，也就是鼠标命中的结果HitResult
		FGameplayTag(),										// 游戏玩法标签：空标签
		AbilitySystemComponent->ScopedPredictionKey); // 作用域预测键：用于预测能力的执行，现在本地完成操作，后通过服务器验证和同步得到近似零延迟的体验。

	/* 本地立即广播执行，后续服务器验证，以达到流畅的体验 */
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle); // 广播有效数据委托，将目标数据句柄传递给监听该委托的蓝图或代码
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	// 该函数用于处理从服务器复制过来的目标数据

	/* 防止重复触发 */
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey()); // 消费客户端复制的目标数据，确保数据只被处理一次

	/* 然后广播，保持服务器与各个客户端一致同步 */
	if (ShouldBroadcastAbilityTaskDelegates())	// 检查是否应该广播能力任务委托
	{
		ValidData.Broadcast(DataHandle);		// 广播有效数据委托，将目标数据句柄传递给监听该委托的蓝图或代码
	}
}
