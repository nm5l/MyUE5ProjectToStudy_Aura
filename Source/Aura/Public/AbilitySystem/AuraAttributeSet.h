// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeGetterFuncPtr; // 用于存储属性获取函数指针的类型定义，不够泛用
// 改为更泛用的模板类型定义
template<class T>
using TStaticFunPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;



// 用于存储GameplayEffect相关的属性信息
USTRUCT()
struct FEffectProperties {
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// PreAttributeChange 是 Unreal Engine 5 Gameplay Ability System 中 UAttributeSet 类的一个虚函数，用于在属性（Attribute）值修改之前进行预处理。
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// PostGameplayEffectExecute 是 Gameplay Ability System(GAS) 中 UAttributeSet 的回调之一。它在一个 GameplayEffect 的 modifier 已经被计算并应用到属性之后调用。
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// 用于将GameplayTag映射到对应的属性获取函数
	// TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagsToAttributes;
	TMap<FGameplayTag, TStaticFunPtr<FGameplayAttribute()>> TagsToAttributes; // 更泛用的模板类型定义



	// 将变量标记为可复制，用于客户端和服务器之间的同步与协调
	/*属性创建流程：
		1. 添加一个UPROPERTY变量，一定要设置ReplicatedUsing = OnRep_<变量名>
		2. 添加一个UFUNCTION函数，即OnRep_<变量名>函数：	void OnRep_<变量名>(const FGameplayAttributeData& Old<变量名>) const;
		3. 第二步的函数只用于服务器上更改并通过网络复制到客户端时，客户端调用与之关联的 OnRep 函数。所以函数中只写一个宏函数：GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, <变量名>, Old<变量名>);
		4. 在GetLifetimeReplicatedProps函数中注册该属性，添加一条宏函数：DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, <变量名>, COND_None, REPNOTIFY_Always);
	*/

	/*****************************************************************
		基础属性 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "基础属性 Vital Attributes")
	FGameplayAttributeData Health; // 生命值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health); // 访问宏函数：是一组预定义的宏，用于简化对属性的访问和操作。在GAS中，属性通常是通过FGameplayAttributeData类型来表示的，而访问宏函数提供了一种方便的方法来获取、设置和初始化这些属性的值。

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "基础属性 Vital Attributes")
	FGameplayAttributeData Mana; // 法力值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);



	/*****************************************************************
		主要属性：不取决于其他属性的属性 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "主要属性 Primary Attributes")
	FGameplayAttributeData Strength; // 力量
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "主要属性 Primary Attributes")
	FGameplayAttributeData Intelligence; // 智力
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "主要属性 Primary Attributes")
	FGameplayAttributeData Resilience; // 韧性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "主要属性 Primary Attributes")
	FGameplayAttributeData Vigor; // 活力
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);



	/*****************************************************************
	次要属性：取决于其他属性的属性 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData Armor; // 护甲
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData ArmorPenetration; // 护甲穿透
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData BlockChance; // 格挡几率
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData CriticalHitChance; // 暴击率
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage; // 暴击伤害
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance; // 暴击耐性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData HealthRegeneration; // 生命值恢复（自然恢复）
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData ManaRegeneration; // 法力值恢复（自然恢复）
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData MaxHealth; // 最大生命值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "次要属性 Secondary Attributes")
	FGameplayAttributeData MaxMana; // 最大法力值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);




	// RepNotify回调函数，当属性通过网络复制到客户端时调用
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const; // 设置Effect属性信息
};
