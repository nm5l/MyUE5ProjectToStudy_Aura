// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

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
	virtual void SetupInputComponent() override; // 还需要创建回调函数来控制角色移动

private:
	// 玩家输入操作->增强输入映射上下文->角色响应动作
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext; // 这个指针用于将输入映射上下文给到蓝图，并将输入映射上下文绑定到玩家控制器

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;		// 这个指针用于绑定输入动作到映射上下文

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace(); // PlayerTick调用，用于获取鼠标下的actor

	TObjectPtr<IEnemyInterface> LastActor;
	TObjectPtr<IEnemyInterface> ThisActor;

};
