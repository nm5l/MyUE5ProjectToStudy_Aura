// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// 设置要捕获的属性，这里是Vigor属性
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute(); // 指定要捕获的属性，这里是Vigor属性
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source; // 指定属性的来源，这里是来源于效果的施加者
	VigorDef.bSnapshot = false; // 不进行快照，实时捕获属性值。快照指的是是否在效果应用时捕获属性值：如果为true，则在效果应用时捕获属性值并在整个效果持续期间使用该值（锁面板）；如果为false，则每次计算时都获取当前属性值（动态计算）。
	RelevantAttributesToCapture.Add(VigorDef); // 将捕获定义添加到相关属性列表中。RelevantAttributesToCapture是一个数组，存储了所有需要捕获的属性定义

}
float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 从Spec中获取源与目标的标签
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 这一段是用来设置评估参数的，评估参数是用来在计算属性值时传递额外信息的，比如：源标签和目标标签
	FAggregatorEvaluateParameters EvaluationParameters; // 创建评估参数对象
	EvaluationParameters.SourceTags = SourceTags; // 设置源标签
	EvaluationParameters.TargetTags = TargetTags; // 设置目标标签

	float Vigor = 0.0f; // 初始化Vigor变量
	// 尝试从Spec中获取捕获的Vigor属性值
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.0f); // 确保Vigor不为负值

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject()); // 获取效果的源对象并尝试转换为ICombatInterface。其实目标和源对象都是自己，相当于自己给自己加Buff
	const int32 PlayerLevel = CombatInterface ? CombatInterface->GetPlayerLevel() : 1; // 获取源对象的战斗等级，如果转换失败则默认为1。AI牛逼，给我加了个三目运算增加健壮性
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel; // 计算最大生命值的公式
}
