// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LsAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ULsAssetManager : public UAssetManager
{
	GENERATED_BODY()


	static ULsAssetManager& Get();
protected:
	
	virtual void StartInitialLoading() override;
};
