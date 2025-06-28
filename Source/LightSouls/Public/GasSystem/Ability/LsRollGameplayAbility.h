// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GasSystem/Ability/LsPlayMotionGameplayAbility.h"
#include "LsRollGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ULsRollGameplayAbility : public ULsPlayMotionGameplayAbility
{
	GENERATED_BODY()

public:
	ULsRollGameplayAbility();

	/** Montage to play when rolling with an input direction */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* RollMontage_Forward;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* RollMontage_Backward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* RollMontage_Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* RollMontage_Right;

	/** Montage to play when backstepping (no input direction) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* BackstepMontage;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
}; 