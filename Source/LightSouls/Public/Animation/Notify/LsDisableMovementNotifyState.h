// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "LsDisableMovementNotifyState.generated.h"

/**
 * 动画通知状态类：在动画播放期间禁用角色移动
 */
UCLASS()
class LIGHTSOULS_API ULsDisableMovementNotifyState : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
}; 