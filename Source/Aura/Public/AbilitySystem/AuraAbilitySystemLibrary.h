// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
/**
 * 这个类是一个能力系统的蓝图函数库，提供了一些静态函数来简化与能力系统相关的操作。
 * 为什么用静态函数？因为这样可以直接通过类名调用函数，而不需要创建类的实例，方便在蓝图中使用。
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// BlueprintPure是指这个函数不会修改任何状态，只会返回数据，适合用来获取信息。（纯函数）
	UFUNCTION(BlueprintPure, Category = "能力系统蓝图函数库 AuraAbilitySystemLibrary | 控件控制器 WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject); // 获取覆盖控件控制器

	UFUNCTION(BlueprintPure, Category = "能力系统蓝图函数库 AuraAbilitySystemLibrary | 控件控制器 WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
};
