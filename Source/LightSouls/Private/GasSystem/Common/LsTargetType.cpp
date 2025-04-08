// Fill out your copyright notice in the Description page of Project Settings.


#include "GasSystem/Common/LsTargetType.h"
#include "Actor/Common/LsCharacterBase.h"


void ULsTargetType::GetTargets_Implementation(ALsCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

void ULsTargetType_UseOwner::GetTargets_Implementation(ALsCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void ULsTargetType_UseEventData::GetTargets_Implementation(ALsCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		AActor* RawTarget = const_cast<AActor*>(EventData.Target.Get());
		OutActors.Add(RawTarget);
	}
}