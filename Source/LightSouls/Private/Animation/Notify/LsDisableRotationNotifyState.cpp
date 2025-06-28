// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/Notify/LsDisableRotationNotifyState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULsDisableRotationNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
    {
        if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
        {
            OrientRotationToMovement = MovementComponent->bOrientRotationToMovement;
            ControllerDesiredRotation = MovementComponent->bUseControllerDesiredRotation;
            
            MovementComponent->bOrientRotationToMovement = false;
            MovementComponent->bUseControllerDesiredRotation = false;
        }
    }
}

void ULsDisableRotationNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
    {
        if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
        {
            MovementComponent->bOrientRotationToMovement = OrientRotationToMovement;
            MovementComponent->bUseControllerDesiredRotation = ControllerDesiredRotation;
        }
    }
} 