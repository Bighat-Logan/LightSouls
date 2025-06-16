#pragma once

#include "CoreMinimal.h"
#include "Actor/Common/AttackVolume.h"
#include "UObject/Interface.h"
#include "CanAttackInterface.generated.h"

// 这个类不需要被编译
UINTERFACE(MinimalAPI, Blueprintable)
class UCanAttackInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 可攻击接口
 * 用于判断一个Actor是否可以被攻击
 */
class LIGHTSOULS_API ICanAttackInterface
{
    GENERATED_BODY()

public:
    /** 判断是否可以攻击 */
    UFUNCTION(BlueprintNativeEvent, Category = "Attack")
    TArray<AAttackVolume*> GetAllAttackVolume() const;

}; 