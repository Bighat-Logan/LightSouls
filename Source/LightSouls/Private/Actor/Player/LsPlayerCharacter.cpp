// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/LsPlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameBase/LsPlayerController.h"
#include "GameBase/LsPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/HUD/LsInGameMenu.h"


// Sets default values
ALsPlayerCharacter::ALsPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ComboComponent = CreateDefaultSubobject<ULsComboActorComponent>(TEXT("ComboComponent"));
}

// Called when the game starts or when spawned
void ALsPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALsPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALsPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALsPlayerCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);

	OnAscRegistered.Broadcast(AbilitySystemComponent);

	if (ALsPlayerController* LsPlayerController = Cast<ALsPlayerController>(GetController()))
	{
		if (ALsInGameMenu* LsInGameHUD = Cast<ALsInGameMenu>(LsPlayerController->GetHUD()))
		{
			LsInGameHUD->InitPlayStatsWidget(LsPlayerController, AbilitySystemComponent, AttributeSet);
			LsInGameHUD->InitSoulsAccountWidget(LsPlayerController);
		}
	}
}

void ALsPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilityActorInfo();
}

