// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/LsPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/Player/LsPlayerCharacter.h"


DEFINE_LOG_CATEGORY(LogTemplateController);

ALsPlayerController::ALsPlayerController()
{
	SoulsCount = 0;
}

void ALsPlayerController::AddSouls_Implementation(int32 Amount)
{
	SoulsCount += Amount;
}

void ALsPlayerController::RemoveSouls_Implementation(int32 Amount)
{
	SoulsCount = FMath::Max(0, SoulsCount - Amount);
}

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
	Super::SetupInputComponent(); // 调用父类实现

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    if (!EnhancedInputComponent)
    {
        UE_LOG(LogTemplateController, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
        return;
    }

    EnhancedInputComponent->ClearActionBindings();
    UE_LOG(LogTemplateController, Log, TEXT("'%s' Cleared existing action bindings."), *GetNameSafe(this));
	
	
    ALsPlayerCharacter* PlayerCharacter = Cast<ALsPlayerCharacter>(GetPawn());

	 if (OptionAction)
	{
	 	EnhancedInputComponent->BindAction(OptionAction, ETriggerEvent::Started, this, &ALsPlayerController::HandleOptionAction);
	 	UE_LOG(LogTemplateController, Log, TEXT("'%s' Bound OptionAction."), *GetNameSafe(this));
	}
	 else
	 {
	 	UE_LOG(LogTemplateController, Warning, TEXT("'%s' OptionAction is not set!"), *GetNameSafe(this));
	 }
    
    if (PlayerCharacter)
    {
       UE_LOG(LogTemplateController, Log, TEXT("'%s' Found PlayerCharacter '%s'. Binding character actions."), *GetNameSafe(this), *GetNameSafe(PlayerCharacter));
       // 绑定输入事件
       if (JumpAction)
       {
           EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, PlayerCharacter, &ALsCharacterBase::Jump);
           EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, PlayerCharacter, &ALsCharacterBase::StopJumping);
       }

       // Moving
       if (MoveAction)
       {
           EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, PlayerCharacter, &ALsCharacterBase::Move);
       }

       // Looking
       if (LookAction)
       {
           EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, PlayerCharacter, &ALsCharacterBase::Look);
       }

    	if (RollAction)
    	{
    		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, PlayerCharacter, &ALsCharacterBase::Roll);
    	}
    }
    else
    {
       UE_LOG(LogTemplateController, Warning, TEXT("'%s' PlayerCharacter is NULL when trying to bind character actions. Pawn is: '%s'"), *GetNameSafe(this), *GetNameSafe(GetPawn()));
    }
}

void ALsPlayerController::OnPossess(APawn* InPawn)
{
	UE_LOG(LogTemplateController, Log, TEXT("'%s' OnPossess called with Pawn: '%s'"), *GetNameSafe(this), *GetNameSafe(InPawn));
	Super::OnPossess(InPawn); // 调用父类，父类可能会处理一些输入的初始化逻辑
	
	SetupInputComponent();
	UE_LOG(LogTemplateController, Log, TEXT("'%s' Explicitly called SetupInputComponent from OnPossess."), *GetNameSafe(this));
}

void ALsPlayerController::OnUnPossess()
{
	UE_LOG(LogTemplateController, Log, TEXT("'%s' OnUnPossess called."), *GetNameSafe(this));
	Super::OnUnPossess();

}
