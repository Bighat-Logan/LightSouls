// Fill out your copyright notice in the Description page of Project Settings.


#include "GasSystem/Ability/LsPlayMotionGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"

ULsPlayMotionGameplayAbility::ULsPlayMotionGameplayAbility()
{
	MontageToPlay = nullptr;
}

void ULsPlayMotionGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!MontageToPlay)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
	if (MontageTask)
	{
		
		MontageTask->OnCancelled.AddDynamic(this, &ULsPlayMotionGameplayAbility::OnCancelled);
		MontageTask->OnInterrupted.AddDynamic(this, &ULsPlayMotionGameplayAbility::OnInterrupted);
		MontageTask->OnCompleted.AddDynamic(this, &ULsPlayMotionGameplayAbility::OnCompleted);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void ULsPlayMotionGameplayAbility::OnCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void ULsPlayMotionGameplayAbility::OnCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULsPlayMotionGameplayAbility::OnInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
} 