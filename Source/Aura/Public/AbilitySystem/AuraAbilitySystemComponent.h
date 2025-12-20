// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//  声明一个多播委托FEffectAssetTags，带有一个参数FGameplayTagContainer，用于传递效果的资产标签
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);


/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();

	// 绑定回调：OverlayWidgetController::BindCallbacksToDependencies()；广播：UAuraAbilitySystemComponent::EffectApplied()
	FEffectAssetTags EffectAssetTags;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities); // 添加角色初始能力

	void AbilityInputTagHeld(const FGameplayTag& InputTag); // 输入标签持续按住
	void AbilityInputTagReleased(const FGameplayTag& InputTag); // 输入标签释放

protected:
	// 回调函数，当效果应用到自身时调用
	UFUNCTION(Client, Reliable) // 客户端可靠调用
		void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
