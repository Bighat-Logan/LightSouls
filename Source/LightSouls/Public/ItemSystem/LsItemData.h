#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "ItemSystem/LsItemTypes.h"
#include "LsItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase // 必须继承 FTableRowBase 才能用于DataTable
{
    GENERATED_BODY()

    // 物品唯一ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    FName ItemID;

    // 物品类型
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    EItemType ItemType;

    // 显示信息
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    FText Description;

    // UI图标
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    TSoftObjectPtr<UTexture2D> Icon;

    // 世界中的模型
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    TSoftObjectPtr<UStaticMesh> WorldMesh;

    // 最大堆叠数量
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    int32 MaxStackSize;

    // 物品的Gameplay Tag，用于分类和逻辑判断
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    FGameplayTag ItemTag;

    // 使用物品时授予的Gameplay Ability (如果使用GAS)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (EditCondition = "ItemType == EItemType::Consumable || ItemType == EItemType::Equipment"))
    TSubclassOf<UGameplayAbility> UseAbility;

    // 装备物品时应用的Gameplay Effect (例如: +10攻击力)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (EditCondition = "ItemType == EItemType::Equipment"))
    TSubclassOf<UGameplayEffect> EquipEffect;

    // 构造函数
    FItemData() : MaxStackSize(1) {}
}; 