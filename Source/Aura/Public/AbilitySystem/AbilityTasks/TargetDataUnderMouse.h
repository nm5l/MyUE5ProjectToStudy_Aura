// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 这个类用于在状态机中进行异步操作
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "AbilityTasks", meta = (DisplayName = "鼠标下的目标数据", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility); // 创建任务实例

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData; // 作为“鼠标下目标数据”节点的一个引脚，还有一个数据引脚“Data”，就是委托里面的那个Data

private:
	virtual void Activate() override; // 激活任务

	/* 处理网络同步的函数：GAS系统的网络同步步骤是本地先执行，数据发送到服务器后处理，最后将服务器的权威结果返回给客户端以达到本地流畅的体验 */
	void SendMouseCursorData();		// 发送鼠标光标数据到服务器
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);  // 处理服务器发送过来的目标数据
};
