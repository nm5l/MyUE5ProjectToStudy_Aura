// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 * 这个单例对象负责存储和管理所有在代码里定义的 Gameplay Tags，确保标签唯一且可全局访问。这样可以避免重复定义，也方便在各个系统中统一使用标签。
 *
 */
 // 使用 struct 而不是 class，因为我们不需要封装和继承特性
struct FAuraGameplayTags
{
public:
	// 返回单例实例
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags(); // 初始化标签

	// FGameplayTag是一个结构体定义在GameplayTagContainer.h，用于表示单个Gameplay Tag

	/*
	 *
	 *主要属性标签*/
	FGameplayTag Attributes_Primary_Strength; // 力量
	FGameplayTag Attributes_Primary_Intelligence; // 智力
	FGameplayTag Attributes_Primary_Resilience; // 韧性
	FGameplayTag Attributes_Primary_Vigor; // 活力

	/*
	 *
	 *次要属性标签*/
	FGameplayTag Attributes_Secondary_Armor;			// 护甲
	FGameplayTag Attributes_Secondary_ArmorPenetration; // 护甲穿透
	FGameplayTag Attributes_Secondary_BlockChance;		// 格挡几率
	FGameplayTag Attributes_Secondary_CriticalHitChance; // 暴击几率
	FGameplayTag Attributes_Secondary_CriticalHitDamage; // 暴击伤害
	FGameplayTag Attributes_Secondary_CriticalHitResistance; // 暴击抗性
	FGameplayTag Attributes_Secondary_HealthRegeneration; // 生命恢复
	FGameplayTag Attributes_Secondary_ManaRegeneration; // 法力恢复
	FGameplayTag Attributes_Secondary_MaxHealth;		// 最大生命值
	FGameplayTag Attributes_Secondary_MaxMana;			// 最大法力值

	/*
	 *
	 *输入动作标签*/

	FGameplayTag InputTag_LMB; // 左键点击
	FGameplayTag InputTag_RMB; // 右键点击
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;



protected:

private:
	static FAuraGameplayTags GameplayTags; // 单例实例
};
