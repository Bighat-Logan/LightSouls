#include "GasSystem/Common/LsAbilitySystemGlobals.h"
#include "GasSystem/GameplayEffect/LsGameplayEffectContext.h"

FGameplayEffectContext* ULsAbilitySystemGlobals::AllocGameplayEffectContext() const
{
    return new FLsGameplayEffectContext();
} 