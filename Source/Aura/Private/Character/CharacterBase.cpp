// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "AbilitySystemComponent.h"

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


