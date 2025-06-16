#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LsCustomEventData.generated.h"

/**
 * 自定义事件数据类
 */
UCLASS(Blueprintable)
class LIGHTSOULS_API ULsCustomEventData : public UObject
{
    GENERATED_BODY()

public:
    ULsCustomEventData();

    /** 在蓝图中创建LsCustomEventData实例 */
    UFUNCTION(BlueprintPure, Category = "Event")
    static ULsCustomEventData* MakeLsCustomEventData();

    /** 可编辑的Vector变量 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    FVector VectorValue;
}; 