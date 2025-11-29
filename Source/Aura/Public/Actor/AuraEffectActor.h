// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAuraEffectActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AppliedEffectsToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	// 这个变量用于承接实时效果的类，该类一般是蓝图类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "应用效果Applied Effects")
	TSubclassOf<UGameplayEffect> InstandGameplayEffectClass;

	// 承接有持续时间的类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "应用效果Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

};
