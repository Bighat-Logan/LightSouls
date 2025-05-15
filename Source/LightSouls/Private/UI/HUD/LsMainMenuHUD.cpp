// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LsMainMenuHUD.h"

#include "Blueprint/UserWidget.h"

void ALsMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		// 创建 MainMenuWidget 实例 
		ULmMainMenuStack = CreateWidget<ULsMainMenuCAWidget>(GetWorld(), MainMenuWidgetClass);
		if (ULmMainMenuStack)
		{
			// 将 MainMenuWidget 添加到视口中 
			ULmMainMenuStack->AddToViewport();
            
			// 初始化逻辑（如果有特定初始化需求，可以在这里设置）
			ULmMainMenuStack->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create MainMenuWidget instance."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainMenuWidgetClass is not set in ALmMainMenuHUD."));
	}
}
