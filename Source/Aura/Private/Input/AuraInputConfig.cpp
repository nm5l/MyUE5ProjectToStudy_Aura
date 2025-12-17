// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound == true)
	{
		UE_LOG(LogTemp, Error, TEXT("没有找到GameplayTag：[%s] 对应的InputAction 在[%s]中"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;

}
