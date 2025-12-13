// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UAttributeSet;
class UAttributeMenuWidgetController;

/**
* 在 AuraWidgetController 打包好参数（Struct），并准备一个初始化参数的方法 （Set）。
	通过其子类 OverlayWidgetController 覆盖层控件控制器 在本类中进行初始化等操作。
 *
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:


	// 一个有则获取无则创建OverlayWidgetController的方法,创建出来的存储在一个指针OverlayWidgetController中
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);


	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);



private:

	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget; // 用于存储覆盖层的小部件
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass; // 覆盖层小部件的类


	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController; // 覆盖层控件控制器的指针
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass; // 覆盖层控件控制器的类


	// 这里不需要类似OverlayWidget的成员变量，因为我们在蓝图中直接创建了该小部件，也就是AttributeMenuWidget，所以直接用控制器来管理它即可
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController; // 属性菜单控件控制器的指针
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass; // 属性菜单控件控制器的类


};
