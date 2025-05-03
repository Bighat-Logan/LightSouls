#pragma once

#include "CoreMinimal.h"
#include "GasSystem/Ability/LsGameplayAbility.h"
#include "GasSystem/Common/LsAbilityTypes.h"
#include "Enums/LsGameplayEnums.h"
#include "LsCombatGameplayAbility.generated.h"

/**
 * 战斗能力基类，提供动作值和冲击力属性
 */
UCLASS()
class LIGHTSOULS_API ULsCombatGameplayAbility : public ULsGameplayAbility
{
    GENERATED_BODY()

public:
    ULsCombatGameplayAbility();

    /** 动作值 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    float ActionValue = 1.0f;

    /** 冲击力 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    ELsImpactForce ImpactForce = ELsImpactForce::Light;


protected:
    virtual void MakeGameplayEffectSpecsFromContainer(const FLsGameplayEffectContainer& Container, const FGameplayEventData& EventData,
        FLsGameplayEffectContainerSpec& OutSpec, int32 OverrideGameplayLevel) override;
    
    FGameplayEffectSpecHandle MakeOutgoingLsGameplayEffectSpec(TSubclassOf<UGameplayEffect> EffectClass,
        const FGameplayEventData& EventData,float Level = 1.0f) const;
}; 