// Fill out your copyright notice in the Description page of Project Settings.


#include "GasSystem/Ability/LsRollGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GasSystem/Tag/LsGameplayTag.h"

ULsRollGameplayAbility::ULsRollGameplayAbility()
{
	RollMontage_Forward = nullptr;
	RollMontage_Backward = nullptr;
	RollMontage_Left = nullptr;
	RollMontage_Right = nullptr;
	BackstepMontage = nullptr;
}

void ULsRollGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector InputDirection = Character->GetLastMovementInputVector();

	if (InputDirection.IsNearlyZero())
	{
		// No input, play backstep
		MontageToPlay = BackstepMontage;
	}
	else
	{
		
		FGameplayTagContainer GameplayTagContainer;
		if (UAbilitySystemComponent* ASC = ActorInfo->AvatarActor->FindComponentByClass<UAbilitySystemComponent>())
		{
			ASC->GetOwnedGameplayTags(GameplayTagContainer);
		}
		if (!GameplayTagContainer.HasTag(FLsGameplayTags::Get().Player_State_LockingOnTarget))
		{
			// Has input, play roll montage and rotate character
			MontageToPlay = RollMontage_Forward;
			// We can rotate the character towards the input direction here if needed
			Character->SetActorRotation(InputDirection.Rotation());
			
		}
		else
		{
			MontageToPlay = RollMontage_Forward;
			Character->SetActorRotation(InputDirection.Rotation());
			//todo
		}
		
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
} 