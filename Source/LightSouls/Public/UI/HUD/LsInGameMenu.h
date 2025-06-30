// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/HUD.h"
#include "UI/CommonUI/Widget/LsCommonActivatableWidget.h"
#include "UI/Widget/LsUserWidget.h"
#include "UI/WidgetController/PlayStatsWidgetController.h"
#include "UI/WidgetController/SoulsAccountWidgetController.h"
#include "LsInGameMenu.generated.h"

class UPlayStatsWidgetController;
class USoulsAccountWidgetController;

/**
 * 游戏内HUD
 */
UCLASS()
class LIGHTSOULS_API ALsInGameMenu : public AHUD
{
	GENERATED_BODY()

public:
	UPlayStatsWidgetController* GetPlayStatsWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	USoulsAccountWidgetController* GetSoulsAccountWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	/** 初始化Overlay */
	void InitPlayStatsWidget(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void InitSoulsAccountWidget(APlayerController* PC);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PlayStatsWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SoulsAccountWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayStatsWidgetController> PlayStatsWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USoulsAccountWidgetController> SoulsAccountWidgetControllerClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULsCommonActivatableWidget> ULmMainMenuStack;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULsUserWidget> PlayStatsWidget;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULsUserWidget> SoulsAccountWidget;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPlayStatsWidgetController> PlayStatsWidgetController;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoulsAccountWidgetController> SoulsAccountWidgetController;

protected:
	virtual void BeginPlay() override;
};
