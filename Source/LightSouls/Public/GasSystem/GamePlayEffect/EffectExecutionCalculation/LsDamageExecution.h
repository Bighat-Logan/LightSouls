// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "LsDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ULsDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	ULsDamageExecution();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
