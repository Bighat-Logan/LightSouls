// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "AttackVolume.generated.h"

UCLASS()
class LIGHTSOULS_API AAttackVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAttackVolume();

	/** 胶囊体碰撞组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleCollision;

	// BlueprintCallable functions to be triggered like the custom events
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void BeginWeaponDamageEvent();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EndWeaponDamageEvent();

	// Function to be bound to OnComponentBeginOverlap
	UFUNCTION() // Needs to be UFUNCTION to be used as a delegate
	void OnAttackOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool bIsAttacking;

	// Array to store actors that have been hit during an attack
	// In Blueprints, "Hit Actor" seems to be cleared. If it's a single actor, use AActor*.
	// If it's meant to store multiple actors hit in one swing (common for melee), use TArray.
	// For this example, let's assume it's an array of actors hit in the current attack.
	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	TArray<AActor*> HitActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|GameplayEvents", meta = (DisplayName = "EventTag"))
	FGameplayTag EventTag;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
