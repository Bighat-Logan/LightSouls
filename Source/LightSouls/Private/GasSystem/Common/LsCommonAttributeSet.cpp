// Fill out your copyright notice in the Description page of Project Settings.


#include "GasSystem/Common/LsCommonAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Actor/Common/LsCharacterBase.h"
#include "Enums/LsGameplayEnums.h"
#include "GasSystem/GameplayEffect/LsGameplayEffectContext.h"
#include "Net/UnrealNetwork.h"
#include "Utility/LsUtilityLibrary.h"

ULsCommonAttributeSet::ULsCommonAttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
	, Mana(0.f)
	, MaxMana(0.f)
	, Stamina(100.f)
	, MaxStamina(100.f)
	, AttackPower(1.0f)
	, DefensePower(1.0f)
	, MoveSpeed(1.0f)
	, Damage(0.0f)
{
}

void ULsCommonAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	

	DOREPLIFETIME(ULsCommonAttributeSet, Health);
	DOREPLIFETIME(ULsCommonAttributeSet, MaxHealth);
	DOREPLIFETIME(ULsCommonAttributeSet, Mana);
	DOREPLIFETIME(ULsCommonAttributeSet, MaxMana);
	DOREPLIFETIME(ULsCommonAttributeSet, Stamina);
	DOREPLIFETIME(ULsCommonAttributeSet, MaxStamina);
	DOREPLIFETIME(ULsCommonAttributeSet, AttackPower);
	DOREPLIFETIME(ULsCommonAttributeSet, DefensePower);
	DOREPLIFETIME(ULsCommonAttributeSet, MoveSpeed);
}

void ULsCommonAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void ULsCommonAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Compute the delta between old and new, if it is available
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ALsCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ALsCharacterBase>(TargetActor);
	}

	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ALsCharacterBase* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<ALsCharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ALsCharacterBase>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}


	FHitResult HitResult;
	if (Context.GetHitResult())
	{
		HitResult = *Context.GetHitResult();
	}

	if (Data.EvaluatedData.Attribute == GetImpactForceAttribute())
	{
		const ELsImpactForce LocalImpactForce = ULsUtilityLibrary::GetImpactForceEnumFromValue(GetImpactForce());
		const FLsGameplayEffectContext* EffectContext = static_cast<const FLsGameplayEffectContext*>(Context.Get());
		if (LocalImpactForce != ELsImpactForce::None && EffectContext)
		{
			TargetCharacter->HandleImpactForceChange(LocalImpactForce,EffectContext->GetImpactVector(),HitResult, SourceTags, SourceCharacter, SourceActor);
			SetImpactForce(0.f);
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		
		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			if (TargetCharacter)
			{
				// This is proper damage
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);
				TargetCharacter->HandleHealthChange(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);
				
			}
		}
	}
}

void ULsCommonAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, Health, OldValue);
}

void ULsCommonAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, MaxHealth, OldValue);
}

void ULsCommonAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, Mana, OldValue);
}

void ULsCommonAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, MaxMana, OldValue);
}

void ULsCommonAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, Stamina, OldValue);
}

void ULsCommonAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, MaxStamina, OldValue);
}

void ULsCommonAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, AttackPower, OldValue);
}

void ULsCommonAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, DefensePower, OldValue);
}

void ULsCommonAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, MoveSpeed, OldValue);
}

void ULsCommonAttributeSet::OnRep_ImpactForce(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULsCommonAttributeSet, ImpactForce, OldValue);
}
