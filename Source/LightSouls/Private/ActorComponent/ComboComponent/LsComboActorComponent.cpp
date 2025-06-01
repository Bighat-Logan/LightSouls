// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ComboComponent/LsComboActorComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "ActorComponent/ComboComponent/DataAsset/LsCombatStateTransition.h"
#include "GasSystem/Tag/LsGameplayTag.h"


// Sets default values for this component's properties
ULsComboActorComponent::ULsComboActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void ULsComboActorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!CombatStateTable || !CombatTransitionTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing required components or data tables"));
		return;
	}
	TArray<FLsCombatStateRow*> CombatStateRows;
	TArray<FLsCombatStateTransitionRow*> CombatStateTransitionRows;
	CombatStateTable->GetAllRows("",CombatStateRows);
	CombatTransitionTable->GetAllRows("",CombatStateTransitionRows);

	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC) {
		// 如果ASC未找到，可能需要检查角色类是否继承自正确的父类（如ACharacter）
		UE_LOG(LogTemp, Error, TEXT("ASC not found on owner actor!"));
		return;
	}
	
	if (CombatStateTable)
	{
		// 获取DataTable所有行的键名集合
		TArray<FName> RowNames = CombatStateTable->GetRowNames();
        
		// 遍历所有状态行
		for (const FName& RowName : RowNames)
		{
			// 通过反射获取行数据
			FLsCombatStateRow* StateRow = CombatStateTable->FindRow<FLsCombatStateRow>(RowName, TEXT(""));
			if (StateRow && !CombatFSM.Contains(RowName))
			{
				// 插入新的状态条目
				CombatFSM.Add(RowName, FCombatStateTransitionArray());
			}
			if (StateRow->AbilityToInvoke && ASC ) { 
				// 仅在服务器端执行赋予操作
				FGameplayAbilitySpec AbilitySpec(StateRow->AbilityToInvoke);
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}

	if (CombatTransitionTable)
	{
		// 获取过渡表的行映射
		TMap<FName, uint8*> TransitionRows = CombatTransitionTable->GetRowMap();
        
		// 遍历所有过渡行
		for (const auto& TransitionPair : TransitionRows)
		{
			// 通过类型转换获取行数据
			FLsCombatStateTransitionRow* TransitionRow = 
				reinterpret_cast<FLsCombatStateTransitionRow*>(TransitionPair.Value);
            
			if (TransitionRow)
			{
				// 查找对应的状态机条目
				if (FCombatStateTransitionArray* TransitionArray = CombatFSM.Find(TransitionRow->CurrentStateName))
				{
					// 添加过渡关系到数组
					TransitionArray->TransitionArray.Add(*TransitionRow);
				}
			}
		}
	}
}

TSubclassOf<ULsGameplayAbility> ULsComboActorComponent::EvaluateCombatState(AActor* TargetActor, EConditionType InputCondition) 
{
	if (!TargetActor || !CombatStateTable || !CombatTransitionTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing required components or data tables"));
		return NULL;
	}

	// 步骤1：检查ActionState标签
	FGameplayTag CurrentActionTag;
	if (CheckActionStateTag(TargetActor))
	{
		CurrentActionTag = FindCurrentStateData(TargetActor);
	}
	else
	{
		CurrentActionTag = FLsGameplayTags::Get().Player_Action_State_Entry;
	}
	FName NewState = FindNextState(CurrentActionTag , InputCondition);
	if (NewState == NAME_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid transition found"));
		return NULL;
	}
	FLsCombatStateRow* NewStateData = CombatStateTable->FindRow<FLsCombatStateRow>(NewState, TEXT(""));
	return NewStateData->AbilityToInvoke;
}

bool ULsComboActorComponent::CheckActionStateTag(AActor* TargetActor) const
{
	if (UAbilitySystemComponent* ASC = TargetActor->FindComponentByClass<UAbilitySystemComponent>())
	{
		FGameplayTagContainer PalyerTagContainer;
		ASC->GetOwnedGameplayTags(PalyerTagContainer);
		return PalyerTagContainer.HasTag(FLsGameplayTags::Get().Player_Action_State);
	}
	UE_LOG(LogTemp, Error, TEXT("Can't get UAbilitySystemComponent !!!!"));
	return false;
}

const FGameplayTag ULsComboActorComponent::FindCurrentStateData(AActor* TargetActor) const
{
	if (UAbilitySystemComponent* ASC = TargetActor->FindComponentByClass<UAbilitySystemComponent>())
	{
		FGameplayTagContainer GameplayTagContainer;
		ASC->GetOwnedGameplayTags(GameplayTagContainer);
		// GameplayTagContainer.HasAny(FLsGameplayTags::Get().Player_Action_State);

		// 精确匹配检查
		if (GameplayTagContainer.Num() == 1)
		{
			return GameplayTagContainer.GetByIndex(0);
		}
		UE_LOG(LogTemp, Error, TEXT("Player has more than 1 Action tag !!!"));
	}
	return FGameplayTag::EmptyTag;
}

FName ULsComboActorComponent::FindNextState(FGameplayTag CurrentActionTag,EConditionType Condition) const
{
	if (const FCombatStateTransitionArray* Transitions = CombatFSM.Find(CurrentActionTag.GetTagName()))
	{
		for (auto Element : Transitions->TransitionArray)
		{
			if(Element.ConditionType == Condition)
			{
				return Element.TargetStateName;
			}
		
		}
	}
	return NAME_None;
}

void ULsComboActorComponent::PlayStateAbility(const FLsCombatStateRow* StateData)
{
	
	if (StateData && StateData->AbilityToInvoke)
	{
		if (ULsGameplayAbility* AbilityInstance = NewObject<ULsGameplayAbility>(this, StateData->AbilityToInvoke))
		{
			// 这里需要根据实际能力系统实现激活逻辑
			// 示例伪代码：
			// AbilityInstance->ActivateAbility();
			// AbilityInstance->PlayMontage();
		}
	}
}



