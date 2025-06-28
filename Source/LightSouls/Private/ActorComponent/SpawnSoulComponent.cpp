// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/SpawnSoulComponent.h"
#include "Interface/CanHoldSouls.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USpawnSoulComponent::USpawnSoulComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SpawnDistance = 100.0f;
	// ...
}


// Called when the game starts
void USpawnSoulComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpawnSoulComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpawnSoulComponent::SpawnSoul(int32 Amount, const FVector& FromLocation)
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	AController* Controller = Owner->GetInstigatorController();
	if (Controller && Controller->GetClass()->ImplementsInterface(UCanHoldSouls::StaticClass()))
	{
		ICanHoldSouls::Execute_AddSouls(Controller, Amount);
	}

	if (SoulsActorClass)
	{
		FVector OwnerLocation = Owner->GetActorLocation();
		FVector Direction = OwnerLocation - FromLocation;

		if (Direction.IsNearlyZero())
		{
			Direction = Owner->GetActorForwardVector();
		}
		else
		{
			Direction.Normalize();
		}

		FVector SpawnLocation = OwnerLocation + Direction * SpawnDistance;
		FRotator SpawnRotation = Direction.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		GetWorld()->SpawnActor<AActor>(SoulsActorClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
} 