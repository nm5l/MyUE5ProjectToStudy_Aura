// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "PlayerState/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <PlayerController/AuraPlayerController.h>
#include <UI/HUD/AuraHUD.h>


APlayerCharacter::APlayerCharacter()
{
	// 角色默认设置
	GetCharacterMovement()->bOrientRotationToMovement = true;			// 视角朝向移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);	// 转向速度400
	GetCharacterMovement()->bConstrainToPlane = true;					// 限制Z轴移动
	GetCharacterMovement()->bSnapToPlaneAtStart = true;					// 自动调整对齐到平面

	// 关闭所有控制器转向
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 为服务器初始化角色能力信息
	InitAbilityActorInfo();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 为客户端初始化角色能力信息
	InitAbilityActorInfo();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();  // 获取玩家状态
	check(AuraPlayerState);
	//AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this); // 通过玩家状态获取能力系统组件调用信息初始化函数
	//Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();	// 初始化后调用AbilityActorInfoSet
	//AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent(); // 获取玩家
	//AttributeSet = AuraPlayerState->GetAttributeSet();

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// 这里获取到玩家控制器 多人模式可能会获取到空，所以这里只操作不为空的情况，即获取到本地玩家控制器的情况
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController())) {
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD())) {
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
