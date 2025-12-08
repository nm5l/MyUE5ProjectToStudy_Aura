// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 *
 * 添加一个基于其他属性的属性计算类，你可以直接在编辑器中使用GameplayEffect蓝图来实现，这种比较简单
 * 如果需要更复杂的逻辑，可以通过C++代码来实现，这里是一个基于活力属性来计算最大生命值的例子
 * 在这里实现完成后别忘了在GameplayEffect蓝图中将幅度计算类型设置为“自定义计算 Custom Calculation Class”，并选择这个类
 *
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxHealth(); // 构造函数，用于初始化属性捕获定义

	// CalculateBaseMagnitude_Implementation函数根据游戏效果的实例（也就是在游戏效果GamplayEffect应用时）被调用，用于计算属性的基础数值（Base Value）。
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
private:
	FGameplayEffectAttributeCaptureDefinition VigorDef; // 最大生命值根据活力来计算，这里捕获活力属性。FGameplayEffectAttributeCaptureDefinition是一个结构体，用来定义要捕获的属性

};
