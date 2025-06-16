// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_StaminaRegen.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API UMMC_StaminaRegen : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_StaminaRegen();

	// 核心计算函数
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// 定义需要捕获的属性，以便在计算时使用
	FGameplayEffectAttributeCaptureDefinition StaminaDef;
	FGameplayEffectAttributeCaptureDefinition MaxStaminaDef;

	// 你可以在这里也定义一个默认回复量，或者直接在GE中设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StaminaRegen", meta=(AllowPrivateAccess="true"))
	float DefaultRegenAmount = 1.0f;
};
