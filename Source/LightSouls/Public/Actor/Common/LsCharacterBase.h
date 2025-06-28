// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"
#include "Enums/LsGameplayEnums.h"
#include "GameFramework/Character.h"
#include "GasSystem/Ability/LsGameplayAbility.h"
#include "GasSystem/Common/LsAbilitySystemComponent.h"
#include "GasSystem/Common/LsCommonAttributeSet.h"
#include "LsCharacterBase.generated.h"

/** 用于定义受击动画的结构体 */
USTRUCT(BlueprintType)
struct FImpactAnimationData
{
	GENERATED_BODY()

	/** 冲击力类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	ELsImpactForce ImpactForce;

	/** 冲击向量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	FVector ImpactVector;
	

	/** 对应的受击动画 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	UAnimMontage* BeHitMontage;
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class LIGHTSOULS_API ALsCharacterBase : public ACharacter,public IAbilitySystemInterface,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ALsCharacterBase();

	virtual void Tick(float DeltaTime) override;

	/** The component used to handle ability system interactions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY()
	ULsCommonAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, Category = Abilities)
	int32 CharacterLevel;

	/** 对应的受击动画 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FGenericTeamId TeamID;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for roll input */
	void Roll(const FInputActionValue& Value);

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChange(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath(float DamageAmount, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnImpactForceChange(ELsImpactForce ImpactForce, FVector ImpactForceVector,const FHitResult& HitInfo, const struct FGameplayTagContainer& ImpactTags, ALsCharacterBase* InstigatorCharacter, AActor* ImpactCauser);

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleHealthChange(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);
	void HandleImpactForceChange(ELsImpactForce ImpactForce, FVector ImpactForceVector, const FHitResult& HitInfo,
	                             const FGameplayTagContainer& ImpactTags, ALsCharacterBase* InstigatorCharacter,
	                             AActor* ImpactCauser);

	UFUNCTION(BlueprintCallable)
	virtual void HandleDeath(float DamageAmount, const struct FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	/** 切换锁定目标的最小时间间隔（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	float MinTargetSwitchInterval;

	/** 上次切换目标的时间 */
	UPROPERTY()
	float LastTargetSwitchTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	TArray<FImpactAnimationData> BeHitConfigArray;

	virtual FGenericTeamId GetGenericTeamId() const override;


protected:

	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities;

	/** The ability to activate when rolling. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<class UGameplayAbility> RollAbility;

	/** If true we have initialized our abilities */
	UPROPERTY()
	int32 bAbilitiesInitialized;
	
	/** Cached movement input */
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector2D LastMovementInput;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/** 处理眩晕标签变化的虚函数 */
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

};
