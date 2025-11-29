// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/AuraUserWidget.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr) {
		// 没有OverlayWidgetController就创建一个。NewObject参数：一个持有对象引用的 Outer，该对象的类
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		// 在这里初始化 设置好参数
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		// 绑定回调依赖项
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 构建Widget 设置WidgetController 添加到视口
	checkf(OverlayWidgetClass, TEXT("覆盖层控件类未初始化，请检查BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("覆盖层控件控制器类未初始化，请检查BP_AuraHUD"));


	// 参数：拥有该控件的对象，该控件的类
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget); // 类型转换，这样可以访问到覆盖层

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);// 参数打包
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams); // 覆盖层控件控制器参数初始化

	OverlayWidget->SetWidgetController(WidgetController); // 绑定覆盖层控件和覆盖层控件控制器
	// 到此为止，WidgetController有了有效的值，OverlayWidget也成功绑定，这个时候就可以进行广播操作了
	WidgetController->BroadcastInitalValues();

	Widget->AddToViewport(); //添加到视口

}

