// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/UI/WidgetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "LsCommonActivatableWidgetStack.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ULsCommonActivatableWidgetStack : public UCommonActivatableWidgetStack,public IWidgetManager
{
	GENERATED_BODY()

	
	virtual void PushWidget_Implementation(TSubclassOf<UUserWidget> Widget,const FCanvasPanelSlotConfig& SlotConfig = FCanvasPanelSlotConfig()) override;

	virtual void PopWidget_Implementation() override; 
};
