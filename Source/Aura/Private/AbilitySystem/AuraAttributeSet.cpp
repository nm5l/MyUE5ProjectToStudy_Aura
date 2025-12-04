// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include <AbilitySystemBlueprintLibrary.h>

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*DOREPLIFETIME_CONDITION_NOTIFY 宏用于 Unreal Engine 5 的网络复制系统中，声明一个属性进行条件复制，并且带有RepNotify回调通知。
	具体功能包括：
		条件复制：指定属性何时复制给客户端（如全部复制、只复制给拥有者等，使用COND_条件）。
		通知触发：控制客户端收到属性更新时，是否调用 OnRep 通知函数，以及通知的触发时机（如每次复制都触发或仅在值变化时触发）。*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeBaseChange(Attribute, NewValue); // 一定要加避免报错
	/*作用
		在属性值实际改变之前调用，允许你调整、限制或修正即将被设置的新数值。
		常用于数值夹紧（clamp）、比例缩放或其他规则限制，避免属性出现非法或不合理的值。
	使用场景
		限制生命值不超过最大值或不低于零。
		调整耐力、法力等属性，确保它们在合理范围内。
		根据游戏逻辑修改属性增减比例*/

		// 限制生命值和法力值
	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source:效果的来源，Target:效果作用的目标

	// 获取Source的信息
	// Data作为FGameplayEffectModCallbackData包含了很多信息，效果规模（也就是效果的实例化）也在其中。通过它来获取上下文结构。
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	// 获取原始施法者的ASC。也就是效果来源的ASC
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// 多重检查：检查效果来源的ASC是否有效，及其两个指针是否有效，接下来会用到他们
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();// 获取SourceASC的“表现体”Avatar，类型是AActor*
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();// 获取SourceASC的控制器

		/*这里插一句。Avatar更倾向于玩家或AI的化身。玩家或AI通过控制器Controller控制Pawn或者Character从而成为一个Avatar。这个过程叫做Possession*/

		// 如果没有从ASC中获取到控制器，就从Pawn中获取。因为控制器可能不在AbilityActorInfo中，但是从Pawn中获取一定有。
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr) {
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor)) {
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController) { // 拿到控制器后转为character类型便于调用角色相关API
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	//获取Target的信息
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();	// 获取Avatar
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();	// 获取Controller
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);				// 获取Character
		// 获取ASC
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	/*常见用途包括：
		根据刚才的属性变化做后续逻辑（例如：处理伤害、检测死亡、把血量 clamp 在范围内、触发 GameplayCue、广播事件给 UI）
		读取 Data 里提供的「来源」和「目标」信息（谁对谁造成），以便做上下文相关处理（例如伤害来源、致死者是谁）
		执行额外的副作用（如播放粒子、声效、应用额外的强制 effect 等）
	换句话说：这是在属性值实际上被修改之后做“业务逻辑”的最佳位置。*/

	FEffectProperties Props;
	SetEffectProperties(Data, Props);


}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	/*当一个 Gameplay Attribute（如生命值、法力值等）在服务器上更改并通过网络复制到客户端时，客户端会调用与之关联的 OnRep 函数。
		在这个 OnRep 函数里，GAMEPLAYATTRIBUTE_REPNOTIFY 宏帮你完成了属性变更的默认处理逻辑，比如更新状态和触发对应的事件。
		它作用是保证属性复制后，Ability System 做好内部同步与事件广播，避免你手动管理复杂的同步代码。*/
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);

}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);

}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);

}


