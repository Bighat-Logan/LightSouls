// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/HUD.h"
#include "UI/CommonUI/Widget/LsCommonActivatableWidget.h"
#include "UI/Widget/LsUserWidget.h"
#include "UI/WidgetController/PlayStatsWidgetController.h"
#include "LsInGameMenu.generated.h"

/**
 * 游戏内HUD
 */
UCLASS()
class LIGHTSOULS_API ALsInGameMenu : public AHUD
{
	GENERATED_BODY()

public:
	UPlayStatsWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	/** 初始化Overlay */
	void InitPlayStatsWidget(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PlayStatsWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayStatsWidgetController> PlayStatsWidgetControllerClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULsCommonActivatableWidget> ULmMainMenuStack;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULsUserWidget> PlayStatsWidget;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPlayStatsWidgetController> PlayStatsWidgetController;

protected:
	virtual void BeginPlay() override;
};
