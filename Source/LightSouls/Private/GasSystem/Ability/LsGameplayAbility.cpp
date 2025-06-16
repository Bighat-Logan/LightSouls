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

bool ULsGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	// 首先调用父类的检查，确保例如冷却（Cooldown）和蓝图中的其他检查通过
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	UAbilitySystemComponent* const ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return false;
	}

	// 遍历我们的自定义花费数组
	for (const FCustomCost& Cost : Costs)
	{
		const UAttributeSet* AttributeSet = ASC->GetAttributeSet(Cost.CostAttribute.GetAttributeSetClass());
		if (!AttributeSet)
		{
			// 如果找不到对应的属性集，则认为检查失败
			return false;
		}

		// 获取当前属性值并与花费比较
		const float CurrentValue = Cost.CostAttribute.GetNumericValue(AttributeSet);
		if (CurrentValue < Cost.CostValue)
		{
			// 资源不足，检查失败
			// 你可以在这里广播一个失败的GameplayEvent，例如 "Ability.Fail.Cost"
			// FGameplayTagContainer FailureTags;
			// FailureTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Fail.Cost")));
			// ASC->HandleGameplayEvent(GetAbilitySystemComponentFromActorInfo()->GenericCancelInterface->GetGameplayCancelTag(), 
			// 						&FGameplayEventData());
			return false;
		}
	}

	return true;
}

void ULsGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// 父类的ApplyCost会应用那个单一的GE，我们不需要它，但调用一下确保逻辑完整
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	UAbilitySystemComponent* const ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return;
	}
    
	// 遍历花费数组，并通过GE来扣除属性值
	for (const FCustomCost& Cost : Costs)
	{
		// 创建一个临时的、即时的GameplayEffect来扣除属性
		UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("TempCostEffect")));
		Effect->DurationPolicy = EGameplayEffectDurationType::Instant;

		// 添加一个Modifier
		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.Attribute = Cost.CostAttribute;
		ModifierInfo.ModifierOp = EGameplayModOp::Additive; // 使用加法，数值为负
		ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-Cost.CostValue));
        
		Effect->Modifiers.Add(ModifierInfo);

		// 将这个临时的GE应用到目标身上
		if (Effect)
		{
			FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectToSelf(Effect, 1.0f, ASC->MakeEffectContext());
		}
	}
}
