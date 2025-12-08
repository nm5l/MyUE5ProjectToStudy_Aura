// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();

	// CalculateBaseMagnitude_Implementation函数根据游戏效果的实例（也就是在游戏效果GamplayEffect应用时）被调用，用于计算属性的基础数值（Base Value）。
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef; // 最大法力值根据智力来计算，这里捕获智力属性。FGameplayEffectAttributeCaptureDefinition是一个结构体，用来定义要捕获的属性

};
