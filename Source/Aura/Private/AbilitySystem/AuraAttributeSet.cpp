// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "Net/UnrealNetwork.h"

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
