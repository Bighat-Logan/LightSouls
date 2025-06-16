#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AISelectMeleeAttackAbility.generated.h"

/**
 * AI近战攻击选择能力
 * 根据条件选择合适的近战攻击能力
 */
UCLASS()
class LIGHTSOULS_API UAISelectMeleeAttackAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UAISelectMeleeAttackAbility();
    virtual void PostInitProperties() override;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

    /** 目标Actor的黑板键名 */
    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Attack Selection")
    FName TargetActorBlackboardKeyName;


    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Attack Selection")
    FGameplayTag TriggerTag;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Attack Selection")
    FGameplayTag DefaultSelectTag;

private:

    /** 从候选列表中随机选择一个攻击能力 */
    TSubclassOf<UGameplayAbility> SelectRandomAttackAbility(FGameplayTag SelectTag) const;
}; 