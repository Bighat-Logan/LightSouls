// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAsset/LsCombatState.h"
#include "DataAsset/LsCombatStateTransition.h"
#include "LsComboActorComponent.generated.h"



USTRUCT()
struct FCombatStateTransitionArray
{
	GENERATED_BODY()
	
	TArray<FLsCombatStateTransitionRow> TransitionArray;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTSOULS_API ULsComboActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULsComboActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TMap<FName,FCombatStateTransitionArray> CombatFSM;

public:

	// 新增DataTable变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat System")
	UDataTable* CombatStateTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat System")
	UDataTable* CombatTransitionTable;

	UFUNCTION(BlueprintCallable, Category = "Combat System")
	TSubclassOf<ULsGameplayAbility> EvaluateCombatState(AActor* TargetActor, EConditionType InputCondition) ;
	
	bool CheckActionStateTag(AActor* TargetActor) const;
	
	const FGameplayTag FindCurrentStateData(AActor* TargetActor) const;
	
	FName FindNextState(FGameplayTag CurrentActionTag,EConditionType Condition) const;
	
	void PlayStateAbility(const FLsCombatStateRow* StateData);
};
