#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "LsAbilitySystemGlobals.generated.h"

/**
 * 自定义的AbilitySystemGlobals类，用于重写GameplayEffectContext的分配
 */
UCLASS()
class LIGHTSOULS_API ULsAbilitySystemGlobals : public UAbilitySystemGlobals
{
    GENERATED_BODY()

public:
    virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
}; 