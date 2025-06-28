 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CanHoldSouls.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanHoldSouls : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIGHTSOULS_API ICanHoldSouls
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Souls")
	void AddSouls(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Souls")
	void RemoveSouls(int32 Amount);
}; 