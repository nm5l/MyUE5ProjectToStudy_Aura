// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraAbilitySystemComponent;
class USplineComponent;

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override; // 重写PlayerTick函数
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override; // 重写SetupInputComponent函数，用于设置输入组件，绑定输入动作和回调函数

private:
	/*
	 * 玩家输入操作->增强输入映射上下文->角色响应动作 */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext; // 暴露给蓝图设置，用于增强输入映射上下文

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;		// 暴露给蓝图设置，用于角色移动的输入动作

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig; // 暴露给蓝图设置，用于存储输入配置数据资产的指针


	/*
	 * 回调函数 */
	void Move(const FInputActionValue& InputActionValue); // 角色移动的回调函数

	void AbilityInputTagPressed(FGameplayTag InputTag); // 按下技能输入标签的回调函数
	void AbilityInputTagReleased(FGameplayTag InputTag); // 释放技能输入标签的回调函数
	void AbilityInputTagHeld(FGameplayTag InputTag); // 持续按压技能输入标签的回调函数


	/* 获取鼠标指针下的actor
	 * 鼠标指针追踪功能，获取到actor调用接口实现高亮显示 */
	void CursorTrace(); // PlayerTick调用，用于获取鼠标下的actor
	TObjectPtr<IEnemyInterface> LastActor; // 上一帧鼠标下的actor
	TObjectPtr<IEnemyInterface> ThisActor; // 本帧鼠标下的actor
	FHitResult CursorHit; // 鼠标射线碰撞结果

	// 获取AbilitySystemComponent
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent; // 缓存AbilitySystemComponent指针
	UAuraAbilitySystemComponent* GetASC(); // 获取AbilitySystemComponent

	/*
	 * 实现鼠标点击移动 */
	FVector CachedDestination = FVector::ZeroVector; // 缓存目标位置: 鼠标点击位置，也就是角色要移动到的目的地
	float FollowTime = 0.f; // 缓存跟随时间: 鼠标持续按下的时间
	float ShortPressThrshold = 0.5f; // 短按阈值: 超过该时间视为长按，未超过视为短按
	bool bAutoRunning = false; // 是否自动奔跑
	bool bTargeting = false; // 是否正在锁定目标

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f; // 自动奔跑接受半径: 角色距离目标点多少范围内算到达

	UPROPERTY(VisibleAnywhere, DisplayName = "自动寻路路径")
	TObjectPtr<USplineComponent> Spline; // 用于显示自动奔跑路径的样条组件，在构造函数中创建

	void AutoRun(); // 实现按键释放后自动奔跑到目标点的函数

};
