// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	// 遍历属性信息数组，查找匹配的属性标签
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag)
		{
			return Info;
		}
	}

	// 如果没有找到匹配的属性标签，且需要记录日志，则输出错误日志
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("找不到属性标签Attribute Tag[%s]在属性信息AttributeInfo[%s]中。"), *AttributeTag.ToString(), *GetNameSafe(this));
	}


	return FAuraAttributeInfo();
}
