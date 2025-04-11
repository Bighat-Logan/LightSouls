// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/LsUserWidget.h"

void ULsUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
