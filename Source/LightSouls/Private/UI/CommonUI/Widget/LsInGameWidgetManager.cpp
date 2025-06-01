// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Widget/LsInGameWidgetManager.h"

void ULsInGameWidgetManager::NativeConstruct()
{
	Super::NativeConstruct();

	if (MainPanelSwitcher && PanelAClass && PanelBClass)
	{
		PanelA = CreateWidget<ULsMainMenuCAWidget>(this, PanelAClass);
		if (PanelA)
		{
			MainPanelSwitcher->AddChild(PanelA);
		}

		PanelB = CreateWidget<ULsMainMenuCAWidget>(this, PanelBClass);
		if (PanelB)
		{
			MainPanelSwitcher->AddChild(PanelB);
		}

		// Optionally activate a default panel
		if (PanelA)
		{
			MainPanelSwitcher->SetActiveWidget(PanelA);
		}
	}
}

void ULsInGameWidgetManager::PopWidget_Implementation()
{
	IWidgetManager::PopWidget_Implementation();
}

void ULsInGameWidgetManager::PushWidget_Implementation(TSubclassOf<UCommonUserWidget> Widget)
{
	IWidgetManager::PushWidget_Implementation(Widget);
}
