// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "LsMainMenuCAWidget.h"
#include "Interface/UI/WidgetManager.h"
#include "LsInGameWidgetManager.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ULsInGameWidgetManager : public UCommonActivatableWidget, public IWidgetManager
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetSwitcher> MainPanelSwitcher;

	UPROPERTY()
	TObjectPtr<ULsMainMenuCAWidget> PanelA;

	UPROPERTY()
	TObjectPtr<ULsMainMenuCAWidget> PanelB;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<ULsMainMenuCAWidget> PanelAClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<ULsMainMenuCAWidget> PanelBClass;
	
	virtual void PushWidget_Implementation(TSubclassOf<UCommonUserWidget> Widget) override;

	virtual void PopWidget_Implementation() override; 
	
};
