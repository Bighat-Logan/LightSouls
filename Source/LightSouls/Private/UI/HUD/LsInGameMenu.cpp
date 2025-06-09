// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LsInGameMenu.h"

#include "UI/WidgetController/LsWidgetController.h"
#include "UI/WidgetController/PlayStatsWidgetController.h"

void ALsInGameMenu::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		// 创建 MainMenuWidget 实例 
		ULmMainMenuStack = CreateWidget<ULsCommonActivatableWidget>(GetWorld(), MainMenuWidgetClass);
		if (ULmMainMenuStack)
		{
			// 将 MainMenuWidget 添加到视口中 
			ULmMainMenuStack->AddToViewport();
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

UPlayStatsWidgetController* ALsInGameMenu::GetOverlayWidgetController(
	const FWidgetControllerParams& WidgetControllerParams)
{
	if (PlayStatsWidgetController == nullptr)
	{
		PlayStatsWidgetController = NewObject<UPlayStatsWidgetController>(this, PlayStatsWidgetControllerClass);
		PlayStatsWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		PlayStatsWidgetController->BindCallbacksToDependencies();
	}
	return PlayStatsWidgetController;
}

void ALsInGameMenu::InitPlayStatsWidget(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(PlayStatsWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_LsInGameMenu"));
	checkf(PlayStatsWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_LsInGameMenu"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), PlayStatsWidgetClass);
	PlayStatsWidget = Cast<ULsUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);
	UPlayStatsWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	PlayStatsWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();

}
