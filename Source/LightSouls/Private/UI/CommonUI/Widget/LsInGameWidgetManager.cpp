// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonUI/Widget/LsInGameWidgetManager.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Widget.h"

void ULsInGameWidgetManager::NativeConstruct()
{
	Super::NativeConstruct();
	
}

UCanvasPanelSlot* ULsInGameWidgetManager::PushToStack(UUserWidget* Widget, const FCanvasPanelSlotConfig& SlotConfig)
{
	if (!Widget)
	{
		return nullptr;
	}

	UCanvasPanelSlot* CanvasSlot = nullptr;

	// 计算新的Z-Order
	int32 NewZOrder = 0;
	if (WidgetStack.Num() > 0)
	{
		if (UWidget* TopWidget = Cast<UWidget>(WidgetStack.Last()))
		{
			CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TopWidget);
			NewZOrder = CanvasSlot->GetZOrder() + 1;
		}
	}
	
	UCanvasPanelSlot* NewSlot = MainCanvas->AddChildToCanvas(Widget);
	NewSlot->SetZOrder(NewZOrder);

	// 应用布局配置
	NewSlot->SetAnchors(SlotConfig.Anchors);
	NewSlot->SetAlignment(SlotConfig.Alignment);
	NewSlot->SetPosition(SlotConfig.Position);
	if (!SlotConfig.bAutoSize)
	{
		NewSlot->SetSize(SlotConfig.Size);
	}
	
	// 如果是可激活的Widget
	if (UCommonActivatableWidget* ActivatableWidget = Cast<UCommonActivatableWidget>(Widget))
	{
		ActivatableWidget->ActivateWidget();
	}
	// 入栈
	WidgetStack.Add(Widget);
	return NewSlot;
}

void ULsInGameWidgetManager::PopFromStack()
{
	if (WidgetStack.Num() == 0)
	{
		return;
	}

	// 获取栈顶元素
	UUserWidget* TopWidget = WidgetStack.Last();
	
	// 如果是可激活的Widget
	if (UCommonActivatableWidget* ActivatableWidget = Cast<UCommonActivatableWidget>(TopWidget))
	{
		// 反激活
		ActivatableWidget->DeactivateWidget();
	}

	// 从视口移除
	TopWidget->RemoveFromParent();
	
	// 出栈
	WidgetStack.Pop();
}

void ULsInGameWidgetManager::PushWidget_Implementation(TSubclassOf<UUserWidget> Widget, const FCanvasPanelSlotConfig& SlotConfig)
{
	if (Widget)
	{
		UCommonUserWidget* NewWidget = CreateWidget<UCommonUserWidget>(this, Widget);
		PushToStack(NewWidget, SlotConfig);
	}
}

void ULsInGameWidgetManager::PopWidget_Implementation()
{
	PopFromStack();
}
