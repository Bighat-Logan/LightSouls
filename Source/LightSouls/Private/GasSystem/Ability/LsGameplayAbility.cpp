// Fill out your copyright notice in the Description page of Project Settings.


#include "GasSystem/Ability/LsGameplayAbility.h"

#include "Actor/Common/LsCharacterBase.h"
#include "GasSystem/Common/LsAbilitySystemComponent.h"
#include "GasSystem/GameplayEffect/LsGameplayEffectContext.h"


ULsGameplayAbility::ULsGameplayAbility() {}

void ULsGameplayAbility::MakeGameplayEffectSpecsFromContainer(const FLsGameplayEffectContainer& Container, const FGameplayEventData& EventData,
	FLsGameplayEffectContainerSpec& OutSpec, int32 OverrideGameplayLevel)
{
	// 默认实现：为每个效果类创建规格
	for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
	{
		OutSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
	}
}

FLsGameplayEffectContainerSpec ULsGameplayAbility::MakeEffectContainerSpecFromContainer(const FLsGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FLsGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	ALsCharacterBase* OwningCharacter = Cast<ALsCharacterBase>(OwningActor);
	ULsAbilitySystemComponent* OwningASC = ULsAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const ULsTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		// If we don't have an override level, use the default on the ability itself
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel();
		}

		// 使用新的虚函数创建效果规格
		MakeGameplayEffectSpecsFromContainer(Container, EventData,ReturnSpec, OverrideGameplayLevel);
	}
	return ReturnSpec;
}

FLsGameplayEffectContainerSpec ULsGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FLsGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FLsGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> ULsGameplayAbility::ApplyEffectContainerSpec(const FLsGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyEffectContainerSpec Loop: Context Ptr before Apply: %p"), SpecHandle.Data->GetContext().IsValid() ? SpecHandle.Data->GetContext().Get() : nullptr); // 记录应用前的地址
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> ULsGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FLsGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}