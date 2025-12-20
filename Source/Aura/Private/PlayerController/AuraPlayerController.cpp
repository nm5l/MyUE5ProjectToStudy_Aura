// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Interaction/EnemyInterface.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Factories/TextureFactory.h"
#include "Input/AuraInputComponent.h"
#include "Math/RotationMatrix.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; // 操作复制，用于多个客户端对服务器的同步
	Spline = CreateDefaultSubobject<USplineComponent>("Spline"); // 创建样条组件
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// tick函数重复执行：获取鼠标射线碰撞到的actor->调用该actor的接口（用于高亮显示）
	CursorTrace();
	AutoRun(); // 自动奔跑功能
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return; // 检查是否处于自动奔跑状态，不是则直接返回
	if (APawn* ControllePawn = GetPawn())
	{
		// 获取角色当前位置在样条上的最近位置，因为有的时候角色可能不在样条上
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControllePawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World); // 获取该位置的切线方向
		ControllePawn->AddMovementInput(Direction); // 添加移动输入，方向为切线方向，强度默认为1

		const float DistanceToDistination = (LocationOnSpline - CachedDestination).Length(); // 计算当前位置到目标位置的距离
		if (DistanceToDistination <= AutoRunAcceptanceRadius) // 检查是否到达目标位置
		{
			bAutoRunning = false; // 到达目标位置，停止自动奔跑
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	// 获取鼠标射线碰撞。参数：检测视线，简单碰撞，返回给变量
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
	*
	*	情况就是这么个情况，但是对于代码来说可以非常简单：我们只关心两个东西：ThisActor和LastActor是否有效，以及它们是否相等。我们在Tick函数中调用该函数，会一直执行这个判断。
	 */
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor(); // 上一帧的actor有效，调用去高亮接口；无效则不进入这个判断
		if (ThisActor) ThisActor->HighlightActor(); // 本帧的actor有效，调用高亮接口；无效则不进入这个判断		
	}

}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		// 通过AbilitySystemBlueprintLibrary静态函数获取AbilitySystemComponent并转换为AuraAbilitySystemComponent类型
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
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
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent); // CastCheck函数同时完成转类型与检查工作

	// 绑定动作与回调
	// 绑定动作：绑定到MoveAction，持续触发，绑定该控制器，动作函数Move
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	// 绑定技能动作：绑定到InputConfig配置，绑定该控制器，按下函数，释放函数，持续按压函数
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB)) // 判断是否为鼠标右键点击
	{
		bTargeting = ThisActor ? true : false; // 根据当前鼠标下的actor是否有效来设置锁定状态
		bAutoRunning = false; // 按下技能输入标签说明不是自动奔跑，我们还不知道是短按还是长按。短按会设置为true，长按保持false，直到松开鼠标。
	}

}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 处理非鼠标右键的持续按压事件
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag); // 将非鼠标右键的输入标签持续按压事件传递给AbilitySystemComponent处理
		return;
	}

	// 处理鼠标右键的持续按压事件
	if (bTargeting) // 鼠标下有有效Actor
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag); // 将鼠标右键的输入标签持续按压事件传递给AbilitySystemComponent处理
	}
	else
	{
		// 处理持续按住的移动行为
		FollowTime += GetWorld()->GetDeltaSeconds(); // 累加持续按压时间

		if (CursorHit.bBlockingHit) // 获取鼠标射线碰撞，返回结果给Hit。
		{
			CachedDestination = CursorHit.ImpactPoint; // 更新目标位置为鼠标点击位置
		}
		if (APawn* ControllePawn = GetPawn())
		{
			const FVector WorldDestination = (CachedDestination - ControllePawn->GetActorLocation()).GetSafeNormal(); // 计算世界目标位置方向向量，归一化。归一化是为了只保留方向信息，去掉距离信息
			ControllePawn->AddMovementInput(WorldDestination); // 添加移动输入，方向为目标位置方向，强度默认为1
		}
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// 处理非鼠标右键的释放事件
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag); // 将非鼠标右键的输入标签释放事件传递给AbilitySystemComponent处理
		return;
	}
	// 处理鼠标右键的释放事件
	if (bTargeting) // 鼠标下有有效Actor
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag); // 将鼠标右键的输入标签释放事件传递给AbilitySystemComponent处理
	}
	else // 鼠标下没有有效Actor，执行移动逻辑
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThrshold)
		{
			// 使用NavigationSystem系统，传入当前位置与目标位置计算路径，成功进入判断体
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints(); // 清除样条点
				for (const FVector& PointLoc : NavPath->PathPoints) // 遍历路径点
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World); // 添加样条点
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				// CachedDestination = NavPath->PathPoints.Last(); // 更新目标位置为路径的最后一个点
				bAutoRunning = true; // 设置为自动奔跑状态
			}
		}
		FollowTime = 0.f; // 重置持续按压时间
	}
	bTargeting = false; // 重置锁定状态
}




