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

/**
 *
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 将变量标记为可复制，用于客户端和服务器之间的同步与协调
	/*属性创建流程：
		1. 添加一个UPROPERTY变量，一定要设置ReplicatedUsing = OnRep_<变量名>
		2. 添加一个UFUNCTION函数，即OnRep_<变量名>函数：	void OnRep_<变量名>(const FGameplayAttributeData& Old<变量名>) const;
		3. 第二步的函数只用于服务器上更改并通过网络复制到客户端时，客户端调用与之关联的 OnRep 函数。所以函数中只写一个宏函数：GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, <变量名>, Old<变量名>);
		4. 在GetLifetimeReplicatedProps函数中注册该属性，添加一条宏函数：DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, <变量名>, COND_None, REPNOTIFY_Always);
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health; // 生命值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth; // 最大生命值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana; // 法力值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana; // 最大法力值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);


	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

};
