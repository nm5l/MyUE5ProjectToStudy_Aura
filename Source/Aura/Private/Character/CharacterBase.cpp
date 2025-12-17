// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystem/AuraAbilitySystemComponent.h>

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 给所有的骨骼网格体设置一个武器插槽
	Weapons = CreateDefaultSubobject<USkeletalMeshComponent>("Weapons");	// 获取插槽
	Weapons->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));		// 附着到骨骼网格体
	Weapons->SetCollisionEnabled(ECollisionEnabled::NoCollision);		// 设置为无碰撞

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::InitAbilityActorInfo()
{

}

void ACharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(GameplayEffectClass); // 确保传入的效果类存在
	check(IsValid(GetAbilitySystemComponent())); // 确保能力系统组件有效
	// 应用默认的属性效果到自己身上，这一段是AI写的，默认就是这么写的
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext(); // 创建效果上下文
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContextHandle); // 创建效果规格
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent()); // 这个Data里面存储了效果规格，应用到自己身上
}

void ACharacterBase::AddCharacterAbilities()
{
	// 只在服务器上添加初始能力
	if (!HasAuthority()) return; // 检查是否有权限
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	AuraASC->AddCharacterAbilities(StartupAbilities); // 调用AbilitySystemComponent的方法添加能力



}

void ACharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}


