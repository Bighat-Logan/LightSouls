// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UObject/Interface.h"
#include "WidgetManager.generated.h"

/** CanvasPanelSlot配置数据结构 */
USTRUCT(BlueprintType)
struct FCanvasPanelSlotConfig
{
	GENERATED_BODY()

	/** 锚点设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	FAnchors Anchors;

	/** 对齐方式 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	FVector2D Alignment;

	/** 位置偏移 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	FVector2D Position;

	/** 尺寸 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	FVector2D Size;

	/** 是否自动调整大小 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	bool bAutoSize;

	/** 默认构造函数 */
	FCanvasPanelSlotConfig()
		: Anchors(0.5f, 0.5f, 0.5f, 0.5f)
		, Alignment(0.5f, 0.5f)
		, Position(0.0f, 0.0f)
		, Size(100.0f, 100.0f)
		, bAutoSize(false)
	{}
};

// This class does not need to be modified.
UINTERFACE()
class UWidgetManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIGHTSOULS_API IWidgetManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,meta = (AutoCreateRefTerm = "SlotConfig"))
	void PushWidget(TSubclassOf<UUserWidget> Widget, const FCanvasPanelSlotConfig& SlotConfig = FCanvasPanelSlotConfig());
	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PopWidget(); 
	
};
