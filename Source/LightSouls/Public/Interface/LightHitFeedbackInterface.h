#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LightHitFeedbackInterface.generated.h"

// 这个类不需要被编译
UINTERFACE(MinimalAPI)
class ULightHitFeedbackInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 用于处理受击反馈的接口
 */
class LIGHTSOULS_API ILightHitFeedbackInterface
{
    GENERATED_BODY()

public:
    // 处理受击反馈的函数
    UFUNCTION(BlueprintNativeEvent, Category = "Light Hit Feedback")
    void OnLightHit(const FVector& HitLocation, const FVector& HitNormal);
}; 