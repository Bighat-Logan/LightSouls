// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ALsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	/** Overriding this function is not essential to this sample since this function 
	 *	is not being called in normal circumstances. Added just to streamline 
	 *	dev-time activities (like automated testing). The default ResetLevel 
	 *	implementation doesn't work all that well with how things are set up in 
	 *	ActionRPG (and that's ok, this is exactly why we override functions!). 
	 */
	ALsGameModeBase();
	
	virtual void ResetLevel() override;

	/** Returns true if GameOver() has been called, false otherwise */
	virtual bool HasMatchEnded() const override;

	/** Called when the game is over i.e. the player dies, the time runs out or the 
	 *	game is finished*/
	UFUNCTION(BlueprintCallable, Category=Game)
	virtual void GameOver();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Game, meta=(DisplayName="DoRestart", ScriptName="DoRestart"))
	void K2_DoRestart();

	UFUNCTION(BlueprintImplementableEvent, Category=Game, meta=(DisplayName="OnGameOver", ScriptName="OnGameOver"))
	void K2_OnGameOver();

	UPROPERTY(BlueprintReadOnly, Category=Game)
	uint32 bGameOver : 1;
};
