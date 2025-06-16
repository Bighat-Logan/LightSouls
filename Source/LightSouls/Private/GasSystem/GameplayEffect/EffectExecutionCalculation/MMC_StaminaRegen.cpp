// Fill out your copyright notice in the Description page of Project Settings.


#include "GasSystem/GameplayEffect/EffectExecutionCalculation/MMC_StaminaRegen.h"

#include "GasSystem/Common/LsCommonAttributeSet.h"


UMMC_StaminaRegen::UMMC_StaminaRegen()
{
	// 定义我们要捕获（Capture）的属性：Stamina 和 MaxStamina
	// 捕获目标（Target）身上的属性值
	// EGameplayEffectAttributeCaptureSource::Target 表示从应用该GE的目标身上获取属性
	// true 表示在GE应用时就快照（Snapshot）属性值，对于周期性效果，每次执行时都会重新快照
    
	// 使用现代的GetAttribute()方法，更安全
	StaminaDef = FGameplayEffectAttributeCaptureDefinition(ULsCommonAttributeSet::GetStaminaAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	MaxStaminaDef = FGameplayEffectAttributeCaptureDefinition(ULsCommonAttributeSet::GetMaxStaminaAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);

	// 将定义的属性添加到RelevantAttributesToCapture列表中，这样系统才知道要去捕获它们
	RelevantAttributesToCapture.Add(StaminaDef);
	RelevantAttributesToCapture.Add(MaxStaminaDef);
}

float UMMC_StaminaRegen::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 从Spec中获取捕获到的属性值
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Stamina = 0.f;
	GetCapturedAttributeMagnitude(StaminaDef, Spec, EvaluationParameters, Stamina);

	float MaxStamina = 0.f;
	GetCapturedAttributeMagnitude(MaxStaminaDef, Spec, EvaluationParameters, MaxStamina);

	// 核心判断逻辑
	if (Stamina >= MaxStamina)
	{
		// 如果当前精力已满或溢出，则本次回复量为0
		return 0.0f;
	}

	// 否则，返回默认的回复量
	// 你也可以在这里做更复杂的计算，比如根据其他属性来调整回复量
	return DefaultRegenAmount;
}