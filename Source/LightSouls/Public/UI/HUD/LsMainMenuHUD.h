// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/CommonUI/Widget/LsCommonActivatableWidget.h"
#include "LsMainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ALsMainMenuHUD : public AHUD
{
	GENERATED_BODY()


public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULsCommonActivatableWidget> ULmMainMenuStack;


protected:
	virtual void BeginPlay() override;
};
