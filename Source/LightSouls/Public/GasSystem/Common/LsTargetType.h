// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Common/LsCharacterBase.h"
#include "UObject/Object.h"
#include "LsTargetType.generated.h"


UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class LIGHTSOULS_API ULsTargetType : public UObject
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	ULsTargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(ALsCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class LIGHTSOULS_API ULsTargetType_UseOwner : public ULsTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	ULsTargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ALsCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class LIGHTSOULS_API ULsTargetType_UseEventData : public ULsTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	ULsTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ALsCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};
