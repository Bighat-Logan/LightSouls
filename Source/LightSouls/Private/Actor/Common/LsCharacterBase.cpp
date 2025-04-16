// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Common/LsCharacterBase.h"


#include "EnhancedInputSubsystems.h"
#include "Enums/LsGameplayEnums.h"
#include "LightSouls/LightSoulsCharacter.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Sets default values
ALsCharacterBase::ALsCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<ULsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	

	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<ULsCommonAttributeSet>(TEXT("AttributeSet"));
	
	bAbilitiesInitialized = false;
	
}

// Called when the game starts or when spawned
void ALsCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALsCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	check(AbilitySystemComponent);
	
	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		// Grant abilities, but only on the server	
		for (TSubclassOf<ULsGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetCharacterLevel(), INDEX_NONE, this));
		}

		// Now apply passives
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}
		
		bAbilitiesInitialized = true;
	}
}

UAbilitySystemComponent* ALsCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ALsCharacterBase::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
                                    const FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);	
}

void ALsCharacterBase::HandleHealthChange(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, ALsCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	OnHealthChange(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ALsCharacterBase::HandleImpactForceChange(ELsImpactForce ImpactForce, const FHitResult& HitInfo, const struct FGameplayTagContainer& ImpactTags, ALsCharacterBase* InstigatorCharacter, AActor* ImpactCauser)
{
    // 调用蓝图事件
    OnImpactForceChange(ImpactForce, HitInfo, ImpactTags, InstigatorCharacter, ImpactCauser);
	
}

// Called every frame
void ALsCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ALsCharacterBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALsCharacterBase::Look(const FInputActionValue& Value)
{

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

int32 ALsCharacterBase::GetCharacterLevel() const
{
	return CharacterLevel;
}

