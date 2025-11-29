// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include <AbilitySystem/AuraAttributeSet.h>

void UOverlayWidgetController::BroadcastInitalValues()
{
	// 通过AttributeSet来获取生命值等属性值，然后进行广播
	const UAuraAttributeSet* AuraAttributeSet = CastChecked< UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// 获取属性集
	const UAuraAttributeSet* AuraAttributeSet = CastChecked< UAuraAttributeSet>(AttributeSet);

	// 每当对应的属性值发生变化时 这些函数回调就会被调用
	// 生命值
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()
	).AddUObject(this, &UOverlayWidgetController::HealthChange);

	// 最大生命值
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()
	).AddUObject(this, &UOverlayWidgetController::MaxHealthChange);

	// 法力值
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()
	).AddUObject(this, &UOverlayWidgetController::ManaChange);

	// 最大法力值
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()
	).AddUObject(this, &UOverlayWidgetController::MaxManaChange);

}

void UOverlayWidgetController::HealthChange(const FOnAttributeChangeData& Data) const
{
	// 回调函数广播新数据
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChange(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);

}

void UOverlayWidgetController::ManaChange(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChange(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

