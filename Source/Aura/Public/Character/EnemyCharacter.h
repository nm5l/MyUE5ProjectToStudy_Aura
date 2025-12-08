// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AEnemyCharacter();
	// 接口函数
	// nemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	// CombatInterface
	virtual int32 GetPlayerLevel() override;
protected:
	virtual void BeginPlay() override;
	void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色类默认 Character Class Defaults")
	int32 Level = 1;
};
