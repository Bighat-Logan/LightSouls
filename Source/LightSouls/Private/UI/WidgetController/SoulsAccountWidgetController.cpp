// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SoulsAccountWidgetController.h"

#include "GameBase/LsPlayerController.h"

void USoulsAccountWidgetController::BroadcastInitialValues()
{
	if (ALsPlayerController* LsPlayerController = Cast<ALsPlayerController>(PlayerController))
	{
		OnSoulsCountChanged.Broadcast(LsPlayerController->GetSoulsCount());
	}
}

void USoulsAccountWidgetController::BindCallbacksToDependencies()
{
	if (ALsPlayerController* LsPlayerController = Cast<ALsPlayerController>(PlayerController))
	{
		LsPlayerController->OnSoulsCountChanged.AddDynamic(this, &USoulsAccountWidgetController::HandleSoulsCountChanged);
	}
}

void USoulsAccountWidgetController::HandleSoulsCountChanged(int32 NewValue)
{
	OnSoulsCountChanged.Broadcast(NewValue);
} 