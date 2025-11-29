// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	// 重写IAbilitySystemInterface中的虚函数用于获取能力系统组件，正如该函数名一样。
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// 类似的，属性也可以这样写一个get方法
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 武器插槽
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapons;

	// 创建下面两个指针用于后续的GAS。角色的属性将会使用这些，但是敌人的属性将写到敌人类内部。
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
