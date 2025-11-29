// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

/**
 *
 */
UCLASS()
class AURA_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	virtual void PossessedBy(AController* NewController) override; // 将pawn与控制器联系起来
	virtual void OnRep_PlayerState() override; // 在PlayerState指针变化时调用，用于客户端响应状态更新
private:
	void InitAbilityActorInfo(); // 用于初始化角色能力信息。有的代码要重复用所以创建函数来代替复制粘贴

};
