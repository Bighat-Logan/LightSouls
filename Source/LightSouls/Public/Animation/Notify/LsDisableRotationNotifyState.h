// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "LsDisableRotationNotifyState.generated.h"

/**
 * 动画通知状态类：在动画播放期间禁用角色旋转
 */
UCLASS()
class LIGHTSOULS_API ULsDisableRotationNotifyState : public UAnimNotifyState
{
    GENERATED_BODY()

	UPROPERTY()
	bool OrientRotationToMovement;

	UPROPERTY()
	bool ControllerDesiredRotation;

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
}; 