// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Enemy/LsEnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/Controller/LsAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GasSystem/Tag/LsGameplayTag.h"

ALsEnemyBase::ALsEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 初始化敌人属性
	DetectionRange = 800.0f;
	AttackRange = 200.0f;
	PatrolRadius = 500.0f;
	bIsPlayerDetected = false;
	bIsAttacking = false;

	// 初始化AIController指针
	EnemyAIController = nullptr;
}

void ALsEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void ALsEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 将新的控制器转换为ALsAIController
	EnemyAIController = Cast<ALsAIController>(NewController);
	
}

void ALsEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 在这里可以添加敌人的AI逻辑
	// 例如：检测玩家、巡逻、攻击等
}

float ALsEnemyBase::GetDistanceToPlayer(ALsCharacterBase* Player) const
{
	if (!Player)
	{
		return BIG_NUMBER;
	}

	return FVector::Distance(GetActorLocation(), Player->GetActorLocation());
}

bool ALsEnemyBase::IsInAttackRange(ALsCharacterBase* Player) const
{
	if (!Player)
	{
		return false;
	}

	float Distance = GetDistanceToPlayer(Player);
	return Distance <= AttackRange;
}

bool ALsEnemyBase::IsInDetectionRange(ALsCharacterBase* Player) const
{
	if (!Player)
	{
		return false;
	}

	float Distance = GetDistanceToPlayer(Player);
	return Distance <= DetectionRange;
}

void ALsEnemyBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	// 获取BlackboardComponent并设置Stunned变量
	if (EnemyAIController)
	{
		if (UBlackboardComponent* BlackboardComponent = EnemyAIController->GetBlackboardComponent())
		{
			// 如果NewCount大于0，设置为true，否则设置为false
			bool bIsStunned = NewCount > 0;
			BlackboardComponent->SetValueAsBool(TEXT("Stunned"), bIsStunned);
		}
	}
} 