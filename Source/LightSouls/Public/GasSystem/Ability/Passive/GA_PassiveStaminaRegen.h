#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GasSystem/GameplayEffect/Passive/GE_StaminaRegen.h"
#include "GA_PassiveStaminaRegen.generated.h"

/**
 * 被动技能：自动施加精力回复效果
 */
UCLASS()
class LIGHTSOULS_API UGA_PassiveStaminaRegen : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_PassiveStaminaRegen()
	{
		// 设置为被动技能
		InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
		NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
		bServerRespectsRemoteAbilityCancellation = false;
		bRetriggerInstancedAbility = true;

		// 设置技能标签
		AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Passive.StaminaRegen")));
		ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Passive.StaminaRegen")));
	}

	// 精力回复效果类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaminaRegen")
	TSubclassOf<UGE_StaminaRegen> StaminaRegenEffectClass;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override
	{
		Super::OnAvatarSet(ActorInfo, Spec);

		// 当技能被赋予时，施加精力回复效果
		if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid() && StaminaRegenEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
				StaminaRegenEffectClass, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}

	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override
	{
		Super::OnRemoveAbility(ActorInfo, Spec);

		// 当技能被移除时，移除精力回复效果
		if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid() && StaminaRegenEffectClass)
		{
			ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(
				StaminaRegenEffectClass, ActorInfo->AbilitySystemComponent.Get());
		}
	}
}; 