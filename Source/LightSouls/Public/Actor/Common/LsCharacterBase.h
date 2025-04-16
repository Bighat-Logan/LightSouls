// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "Enums/LsGameplayEnums.h"
#include "GameFramework/Character.h"
#include "GasSystem/Ability/LsGameplayAbility.h"
#include "GasSystem/Common/LsAbilitySystemComponent.h"
#include "GasSystem/Common/LsCommonAttributeSet.h"
#include "LsCharacterBase.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class LIGHTSOULS_API ALsCharacterBase : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALsCharacterBase();

	virtual void Tick(float DeltaTime) override;

	/** The component used to handle ability system interactions */
	UPROPERTY()
	ULsAbilitySystemComponent* AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY()
	ULsCommonAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, Category = Abilities)
	int32 CharacterLevel;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChange(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnImpactForceChange(ELsImpactForce ImpactForce, const FHitResult& HitInfo, const struct FGameplayTagContainer& ImpactTags, ALsCharacterBase* InstigatorCharacter, AActor* ImpactCauser);

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleHealthChange(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleImpactForceChange(ELsImpactForce ImpactForce, const FHitResult& HitInfo, const struct FGameplayTagContainer& ImpactTags, ALsCharacterBase* InstigatorCharacter, AActor* ImpactCauser);

protected:

	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<ULsGameplayAbility>> GameplayAbilities;

	/** If true we have initialized our abilities */
	UPROPERTY()
	int32 bAbilitiesInitialized;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	

};
