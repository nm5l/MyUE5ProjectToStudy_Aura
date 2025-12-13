// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	// 引擎的AssetManger是唯一的，负责管理所有的资源。我们通过这个静态函数获取它的实例，并将其转换为UAuraAssetManager类型，以便访问自定义的资源管理功能。
	static UAuraAssetManager& Get();
protected:
	virtual void StartInitialLoading() override; // 重写初始加载函数，该函数用于在引擎启动时加载必要的资源。

};
