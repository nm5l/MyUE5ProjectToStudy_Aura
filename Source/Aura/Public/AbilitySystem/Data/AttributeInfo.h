// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

// 定义一个结构体用于存储属性信息
USTRUCT(BlueprintType) // 使其在蓝图中可用
struct FAuraAttributeInfo {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag(); // 属性标签

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText(); // 属性名称

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText(); // 属性描述

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f; // 属性值
};

/**
 *
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	// 根据属性标签查找对应的属性信息
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformation; // 属性信息数组
};
