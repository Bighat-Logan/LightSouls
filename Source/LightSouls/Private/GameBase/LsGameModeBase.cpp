// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/LsGameModeBase.h"
#include "GameBase/LsPlayerController.h"


ALsGameModeBase::ALsGameModeBase()
{
	GameStateClass = ALsGameModeBase::StaticClass();
	PlayerControllerClass = ALsPlayerController::StaticClass();
	bGameOver = false;
}

void ALsGameModeBase::ResetLevel() 
{
	K2_DoRestart();
}

bool ALsGameModeBase::HasMatchEnded() const 
{
	return bGameOver;
}

void ALsGameModeBase::GameOver()
{
	if (bGameOver == false)
	{
		K2_OnGameOver();
		bGameOver = true;
	}
}