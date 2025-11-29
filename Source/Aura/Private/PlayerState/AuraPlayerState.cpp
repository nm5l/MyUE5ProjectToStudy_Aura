// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/AuraPlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	// 构建GAS组件，初始化能力系统组件的两个指针用于敌人的属性构建
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // 设置：复制为真，？可能是用于客户端同步的
	// 用于多人同步的模式选择：
	// Full适合单人游戏，所有游戏效果都会同步到所有客户端；
	// Mixed多人玩家控制，游戏效果只会同步到自己的客户端，但是游戏标签tags会同步到所有客户端；
	// Minimal多人AI控制，游戏效果不会同步，但是游戏标签会同步到所有客户端
	// 玩家状态这里为玩家控制，作为多人游戏选择Mixed模式。敌人类将会使用Minimal模式。
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 构建属性集
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f; // 网络更新频率 用于同步客户端
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
