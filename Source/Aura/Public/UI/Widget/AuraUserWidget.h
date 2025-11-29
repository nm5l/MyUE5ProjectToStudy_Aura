// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController; // 使用这个指针，控件控制器将会通过控件基类操作数据的展示

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController); // 这个函数就是用于调用protected里面那个WidgetControllerSet的，用于将控件与控制器绑定在一起

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet(); // 这个函数用于在获取到控件控制器后进行设置各项配置。由于是类内设置所以设为protected，需要通过其他函数调用它

};
