#pragma once

#include "CoreMinimal.h"
#include "LsGameplayEnums.generated.h"

/**
 * 冲击力类型
 */
UENUM(BlueprintType)
enum class ELsImpactForce : uint8
{
    None UMETA(DisplayName = "无"),
    Light UMETA(DisplayName = "轻击"),
    Medium UMETA(DisplayName = "中击"),
    Heavy UMETA(DisplayName = "重击")
};


/**
 * 条件类型
 */
UENUM(BlueprintType)
enum class EConditionType : uint8
{
    IA_Attack_Short UMETA(DisplayName = "短按攻击"),
    IA_Attack_Long UMETA(DisplayName = "长按攻击")
}; 