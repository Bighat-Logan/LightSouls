// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/CommonUI/Widget/LsMainMenuCAWidget.h"
#include "LsInGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ALsInGameMenu : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULsMainMenuCAWidget> ULmMainMenuStack;


protected:
	virtual void BeginPlay() override;
};
