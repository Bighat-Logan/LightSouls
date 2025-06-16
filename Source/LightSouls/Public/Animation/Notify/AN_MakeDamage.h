#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AN_MakeDamage.generated.h"

/**
 * 伤害通知状态类
 * 用于在动画播放过程中创建伤害区域
 */
UCLASS()
class LIGHTSOULS_API UAN_MakeDamage : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    UAN_MakeDamage();

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
}; 