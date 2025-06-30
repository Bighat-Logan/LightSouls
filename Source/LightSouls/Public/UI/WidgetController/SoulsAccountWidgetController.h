// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/LsWidgetController.h"
#include "SoulsAccountWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoulsCountChanged, int32, NewSoulsCount);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LIGHTSOULS_API USoulsAccountWidgetController : public ULsWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnSoulsCountChanged OnSoulsCountChanged;

protected:
	UFUNCTION()
	void HandleSoulsCountChanged(int32 NewValue);
}; 