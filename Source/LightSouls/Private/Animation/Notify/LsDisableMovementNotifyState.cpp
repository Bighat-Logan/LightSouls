// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/Notify/LsDisableMovementNotifyState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULsDisableMovementNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
    {
        if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
        {
            MovementComponent->DisableMovement();
        }
    }
}

void ULsDisableMovementNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
    {
        if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
        {
            MovementComponent->SetMovementMode(MOVE_Walking);
        }
    }
} 