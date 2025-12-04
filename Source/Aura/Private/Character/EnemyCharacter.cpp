// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AEnemyCharacter::AEnemyCharacter()
{
	// 之前在蓝图中设置的碰撞预设，现在在这里设置为自定义，这样可以应用于全部敌人
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// 构建GAS组件，初始化能力系统组件的两个指针用于敌人的属性构建
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // 设置：复制为真，？可能是用于客户端同步的
	// 敌人类将会使用Minimal模式
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// 构建属性集
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

}

void AEnemyCharacter::HighlightActor()
{
	// 如同在actor细节面板勾选“渲染自定义深度” 轮廓处理就是这样用的，记得在细节面板中选择好材质
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapons->SetRenderCustomDepth(true);
	Weapons->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapons->SetRenderCustomDepth(false);


}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	// 初始化敌人类能力角色信息
	// 参数：初始化的能力所有者，化身角色，
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// 初始化后调用AbilityActorInfoSet
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
