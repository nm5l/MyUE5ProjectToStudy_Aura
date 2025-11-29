// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

//class APlayerController;
//class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;

/*
在使用 GAS（Gameplay Ability System）时，UI（UMG Widget）通常需要：
显示角色血量、蓝量、体力
监听属性变化（如生命值变化）
显示技能冷却
获取玩家状态（例如：职业、等级）

这些数据都来自 ASC 和 AttributeSet。
但 UI 不直接绑定 Pawn 或 Character，设计了一个 参数结构体，初始化 WidgetController 时把数据传进去。

总结：初始化 WidgetController
*/

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 *
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams); // 初始化的方法

	virtual void BroadcastInitalValues(); // 用于广播初始值 子类实现

	virtual void BindCallbacksToDependencies(); // 绑定回调依赖项 用于响应绑定的属性变化
protected:
	// 控件控制器对四个重要的类感兴趣，即控件控制器及其子类会接触到下面四个极其重要的类，所以创建指针以便于后续访问他们。
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

};
