// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GasSystem/Ability/LsGameplayAbility.h"
#include "LsCombatState.generated.h"

class ULsCombatState;

USTRUCT(BlueprintType)
struct FLsCombatStateRow : public FTableRowBase 
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StateName;

	// 关联的状态类（蓝图类）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ULsGameplayAbility> AbilityToInvoke;
	

	// 该状态的转换条件列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Transitions;
	
};

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ULsCombatState : public UDataAsset
{
	GENERATED_BODY()
	
};
