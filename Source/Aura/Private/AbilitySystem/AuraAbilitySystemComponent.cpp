// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// 调用该函数说明Actor信息初始化完成（InitActorInfo）
	// 绑定委托与回调函数
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);



}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	// 添加角色初始能力
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			// 将能力的StartupInputTag添加到AbilitySpec的DynamicAbilityTags中，DynamicAbilityTags是在运行时动态添加的标签，能够用于能力的激活条件判断
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec); // 将AbilitySpec注册到能力系统组件中
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) // 遍历所有可激活的能力。GetActivatableAbilities()函数用于获取当前组件上所有可以激活的能力（Abilities）的列表。返回值通常是一个 TArray<FGameplayAbilitySpec>，每个元素代表一个已注册到该组件、并且可以被激活的能力实例。
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) // 检查AbilitySpec的DynamicAbilityTags中是否包含输入标签InputTag
		{
			AbilitySpecInputPressed(AbilitySpec); // 模拟按下与该AbilitySpec相关联的输入，该函数用于记录某个技能输入被按下，帮助能力系统正确响应玩家的技能操作。
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle); // 尝试激活该能力
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) // 遍历所有可激活的能力。GetActivatableAbilities()函数用于获取当前组件上所有可以激活的能力（Abilities）的列表。返回值通常是一个 TArray<FGameplayAbilitySpec>，每个元素代表一个已注册到该组件、并且可以被激活的能力实例。
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) // 检查AbilitySpec的DynamicAbilityTags中是否包含输入标签InputTag
		{
			AbilitySpecInputReleased(AbilitySpec); // 模拟按下与该AbilitySpec相关联的输入，该函数用于记录某个技能输入被松开，帮助能力系统正确响应玩家的技能操作。
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle); // 尝试激活该能力
			}
		}
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	// OnGameplayEffectAppliedDelegateToSelf委托的回调函数

	// 获取效果标签GameplayEffectAssetTags（需要在编辑器中设置标签）
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);


}
