// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/LsPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/Player/LsPlayerCharacter.h"


DEFINE_LOG_CATEGORY(LogTemplateController);

void ALsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ALsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ALsPlayerCharacter* PlayerCharacter = Cast<ALsPlayerCharacter>(GetCharacter());
	
	if (EnhancedInputComponent && PlayerCharacter) {
		// 绑定输入事件
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, PlayerCharacter, &ALsCharacterBase::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, PlayerCharacter, &ALsCharacterBase::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, PlayerCharacter, &ALsCharacterBase::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, PlayerCharacter, &ALsCharacterBase::Look);
	}
	else
	{
		UE_LOG(LogTemplateController, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ALsPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetupInputComponent();
}
