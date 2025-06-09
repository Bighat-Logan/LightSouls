// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/LsWidgetController.h"

void ULsWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void ULsWidgetController::BroadcastInitialValues()
{
}

void ULsWidgetController::BindCallbacksToDependencies()
{
}

ALsPlayerController* ULsWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<ALsPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

ULsAbilitySystemComponent* ULsWidgetController::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<ULsAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

ULsCommonAttributeSet* ULsWidgetController::GetAuraAS()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<ULsCommonAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
