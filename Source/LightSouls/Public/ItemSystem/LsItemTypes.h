#pragma once

#include "CoreMinimal.h"
#include "LsItemTypes.generated.h"

/**
 * 物品类型枚举
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
    None UMETA(DisplayName = "无"),
    Consumable UMETA(DisplayName = "消耗品"),
    Equipment UMETA(DisplayName = "装备"),
    Material UMETA(DisplayName = "材料"),
    Weapon UMETA(DisplayName = "武器"),
    Armor UMETA(DisplayName = "护甲"),
    Quest UMETA(DisplayName = "任务物品")
}; 