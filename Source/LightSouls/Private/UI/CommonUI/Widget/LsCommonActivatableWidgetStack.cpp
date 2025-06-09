// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Widget/LsCommonActivatableWidgetStack.h"

void ULsCommonActivatableWidgetStack::PushWidget_Implementation(TSubclassOf<UUserWidget> Widget,const FCanvasPanelSlotConfig& SlotConfig)
{
	if (Widget && Widget->IsChildOf(UCommonActivatableWidget::StaticClass()))
	{
		TSubclassOf<UCommonActivatableWidget> WidgetClass = TSubclassOf<UCommonActivatableWidget>(Widget);
		this->AddWidget(WidgetClass);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Widget is not a child of UCommonActivatableWidget"));
	}	
}

void ULsCommonActivatableWidgetStack::PopWidget_Implementation()
{
	IWidgetManager::PopWidget_Implementation();
}
