// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"
#include <AbilitySystemInterface.h>
#include <AbilitySystemBlueprintLibrary.h>

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 创建一个场景组件作为根组件
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();


}

void AAuraEffectActor::AppliedEffectsToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 获取目标单位的 ASC
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	// 创建 GameplayEffectContext（作用环境) EffectContext 是用来“记录伤害/治疗来源”等上下文信息的结构。
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	// 创建 GameplayEffectSpec（效果规格）规格Spec = “效果的实例化数据”，比如：10点伤害，5秒持续时间，是否暴击，Buff/DeBuff数值等
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	//施加 GameplayEffect，将刚创建的 GameplayEffectSpec 应用到 TargetASC
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}



