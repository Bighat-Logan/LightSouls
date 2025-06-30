// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LsInGameMenu.h"

#include "UI/WidgetController/LsWidgetController.h"
#include "UI/WidgetController/PlayStatsWidgetController.h"
#include "UI/WidgetController/SoulsAccountWidgetController.h"

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

UPlayStatsWidgetController* ALsInGameMenu::GetPlayStatsWidgetController(
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

USoulsAccountWidgetController* ALsInGameMenu::GetSoulsAccountWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (SoulsAccountWidgetController == nullptr)
	{
		SoulsAccountWidgetController = NewObject<USoulsAccountWidgetController>(this, SoulsAccountWidgetControllerClass);
		SoulsAccountWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		SoulsAccountWidgetController->BindCallbacksToDependencies();
	}
	return SoulsAccountWidgetController;
}

void ALsInGameMenu::InitPlayStatsWidget(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(PlayStatsWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_LsInGameMenu"));
	checkf(PlayStatsWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_LsInGameMenu"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), PlayStatsWidgetClass);
	PlayStatsWidget = Cast<ULsUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);
	UPlayStatsWidgetController* WidgetController = GetPlayStatsWidgetController(WidgetControllerParams);

	PlayStatsWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}

void ALsInGameMenu::InitSoulsAccountWidget(APlayerController* PC)
{
	checkf(SoulsAccountWidgetClass, TEXT("Souls Account Widget Class uninitialized, please fill out BP_LsInGameMenu"));
	checkf(SoulsAccountWidgetControllerClass, TEXT("Souls Account Widget Controller Class uninitialized, please fill out BP_LsInGameMenu"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), SoulsAccountWidgetClass);
	SoulsAccountWidget = Cast<ULsUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, nullptr, nullptr);
	USoulsAccountWidgetController* WidgetController = GetSoulsAccountWidgetController(WidgetControllerParams);

	SoulsAccountWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}
