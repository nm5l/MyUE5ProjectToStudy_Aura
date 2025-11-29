// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/AuraPlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Math/RotationMatrix.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; // 操作复制，用于多个客户端对服务器的同步
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// tick函数重复执行：获取鼠标射线碰撞到的actor->调用该actor的接口（用于高亮显示）
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	// 获取鼠标射线碰撞。参数：检测视线，简单碰撞，返回给变量
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return; // 检测是否有结果

	//结果处理：通过类型转换检测目标actor是否实现了接口功能，转换成功就说明实现了，否则返回空指针
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	思路：创建两个接口指针用于承载鼠标获取actor的结果。一个承载上一帧的结果（LastActor），一个承载当前帧的结果（ThisActor）。
	每帧在运行时，将ThisActor的值给到LastActor，然后将当前帧的结果给到ThisActor，也就是上面两行。
	接下来有五种情况：
		一、LastActor为空，ThisActor为空：什么都不做
			对应鼠标没有选中敌人的情况
		二、LastActor为空，ThisActor为有效值：将ThisActor进行高亮处理HighlightActor
			对应鼠标移动到敌人身上的情况
		三、LastActor为有效值，ThisActor为空：将LastActor进行去高亮处理UnHighlightActor
			对应鼠标从敌人身上移走的情况
		四、LastActor和ThisActor都为有效值，但是不是同一个有效值：将LastActor去高亮，将ThisActor高亮
			对应鼠标从一个敌人移动到另一个敌人的情况
		五、LastActor和ThisActor都为同一个有效值：不做任何处理
			对应鼠标一直悬停在一个敌人的情况
	*/
	if (LastActor == nullptr) {
		if (ThisActor != nullptr) {
			// 情况二
			ThisActor->HighlightActor();
		}
		else {
			// 情况一：什么都不做
		}
	}
	else {
		if (ThisActor != nullptr) {
			if (LastActor == ThisActor) {
				// 情况五：什么都不做
			}
			else {
				// 情况四
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
		else {
			// 情况三
			LastActor->UnHighlightActor();
		}
	}

}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 检查指针是否已被设置。若没有则会出现控制无效的问题。使用check会直接退出
	ensure(AuraContext);

	// 添加强化输入映射上下文：访问本地玩家输入增强子系统，通过静态函数GetSubsystem（参数GetLocalPlayer）来获取。单例模式
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//check(Subsystem);	// 检查是否获取到
	// 这里要检查是否获取到本地玩家 多人模式可能获取为空
	if (Subsystem) {
		Subsystem->AddMappingContext(AuraContext, 0); // 添加给反射蓝图的指针中，设置优先级为0最高
	}


	// 展示鼠标指针，第三人称俯视角需要鼠标操作。样式设为默认即可
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// 设置输入模式：使用鼠标或键盘来控制操作甚至UI
	FInputModeGameAndUI InputModeData; // 局部变量用于设置
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);// 不锁定视口
	InputModeData.SetHideCursorDuringCapture(false); // 光标进入视口不隐藏
	SetInputMode(InputModeData); // 将设置传给玩家控制器 



}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 父类中InputComponent输入组件类型为UInputComponent，这里将其转换为UEnhancedInputComponent类型（注意添加头文件）
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent); // CastCheck函数同时完成转类型与检查工作

	// 绑定动作：绑定到MoveAction，持续触发，绑定该控制器，动作函数Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);

	}



}

