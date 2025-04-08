// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Common/LsCharacterBase.h"
#include "ActorComponent/ComboComponent/LsComboActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "LsPlayerCharacter.generated.h"

UCLASS()
class LIGHTSOULS_API ALsPlayerCharacter : public ALsCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALsPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ULsComboActorComponent* ComboComponent;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
