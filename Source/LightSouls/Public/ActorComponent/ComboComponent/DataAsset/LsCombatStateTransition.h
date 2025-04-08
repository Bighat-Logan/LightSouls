// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LsCombatStateTransition.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EConditionType : uint8
{
	IA_Attack_Short,
	IA_Attack_Long,
};

class ULsCombatStateTransition;

USTRUCT(BlueprintType)
struct FLsCombatStateTransitionRow : public FTableRowBase 
{
	GENERATED_BODY()

	// 目标状态名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CurrentStateName;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConditionType ConditionType;
	
	// 目标状态名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetStateName;
    
	// 可添加其他字段，如ID、描述等
};


UCLASS()
class LIGHTSOULS_API ULsCombatStateTransition : public UDataAsset
{
	GENERATED_BODY()

};
