// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LsTestStruct.generated.h"



USTRUCT(BlueprintType)
struct FMyTestStruct
{
	GENERATED_BODY()

	UPROPERTY()
	int32 TestInt;
};