// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Interface/CanHoldSouls.h"
#include "LsPlayerController.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateController, Log, All);

/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ALsPlayerController : public APlayerController, public ICanHoldSouls
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
	ALsPlayerController();

	//~ Begin ICanHoldSouls Interface
	virtual void AddSouls_Implementation(int32 Amount) override;
	virtual void RemoveSouls_Implementation(int32 Amount) override;
	//~ End ICanHoldSouls Interface
	
	UFUNCTION(BlueprintImplementableEvent)
	void HandleOptionAction();
	
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttckAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OptionAction;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Souls")
	int32 SoulsCount;
};
