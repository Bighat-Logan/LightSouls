// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GasSystem/Ability/LsGameplayAbility.h"
#include "LsPlayMotionGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ULsPlayMotionGameplayAbility : public ULsGameplayAbility
{
	GENERATED_BODY()

public:
	ULsPlayMotionGameplayAbility();
	
	/** Montage to play when this ability is activated */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* MontageToPlay;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Callbacks for Montage Task */
	UFUNCTION()
	void OnCancelled();

	UFUNCTION()
	void OnCompleted();

	UFUNCTION()
	void OnInterrupted();
}; 