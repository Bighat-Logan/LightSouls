// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Actor/Common/LsCharacterBase.h"
#include "LsAIController.generated.h"

UCLASS()
class LIGHTSOULS_API ALsAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALsAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId)  override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UPROPERTY()
	TObjectPtr<ALsCharacterBase> ControlledPawn;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
