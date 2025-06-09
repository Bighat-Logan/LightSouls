// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "Interface/UI/WidgetManager.h"
#include "Components/CanvasPanel.h"
#include "LsInGameWidgetManager.generated.h"

/**
 * 游戏内UI管理器
 */
UCLASS()
class LIGHTSOULS_API ULsInGameWidgetManager : public UCommonActivatableWidget, public IWidgetManager
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	/** 主画布面板 */
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	/** Widget栈 */
	UPROPERTY()
	TArray<UUserWidget*> WidgetStack;

	/** 进栈方法 */
	UCanvasPanelSlot* PushToStack(UUserWidget* Widget, const FCanvasPanelSlotConfig& SlotConfig);

	/** 出栈方法 */
	void PopFromStack();

public:
	virtual void PushWidget_Implementation(TSubclassOf<UUserWidget> Widget,const FCanvasPanelSlotConfig& SlotConfig = FCanvasPanelSlotConfig()) override;

	virtual void PopWidget_Implementation() override; 
	
};
