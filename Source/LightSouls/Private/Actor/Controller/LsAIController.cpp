// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Controller/LsAIController.h"

#include "Perception/AIPerceptionSystem.h"


// Sets default values
ALsAIController::ALsAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALsAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALsAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 尝试将被控制的Pawn转换为你的目标类型
	ControlledPawn = Cast<ALsCharacterBase>(InPawn);

	if (ControlledPawn)
	{
		// 赋值成功！现在可以安全地使用 ControlledPawn 了
		UE_LOG(LogTemp, Log, TEXT("Controller successfully possessed and cached: %s"), *ControlledPawn->GetName());
	}
	else
	{
		// 如果Pawn不是ALsCharacterBase或其子类，转换会失败
		UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn is not of type ALsCharacterBase."));
	}
	
}

void ALsAIController::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	Super::SetGenericTeamId(NewTeamId);
	UAIPerceptionSystem::GetCurrent(GetWorld())->UpdateListener(*GetAIPerceptionComponent());
}

FGenericTeamId ALsAIController::GetGenericTeamId() const
{
	if (ControlledPawn)
	{
		return ControlledPawn->GetGenericTeamId();
	}
	else
	{
		return Super::GetGenericTeamId();
	}
}

ETeamAttitude::Type ALsAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
		if (const APawn* OtherPawn = Cast<APawn>(&Other))
		{
			if (const IGenericTeamAgentInterface* TeamAgentA = Cast<IGenericTeamAgentInterface>(&Other))
			{
				// 如果对方控制器没有实现接口，再检查对方Actor本身是否实现
				FGenericTeamId OtherTeamId = TeamAgentA->GetGenericTeamId();
				FGenericTeamId testA = OtherTeamId.GetId();
				FGenericTeamId testB = GetGenericTeamId().GetId();
				
				if (OtherTeamId.GetId() != FGenericTeamId::NoTeam && GetGenericTeamId().GetId() == OtherTeamId.GetId())
				{
					return ETeamAttitude::Friendly;
				}
				else
				{
					return ETeamAttitude::Hostile;
				}
			}
			// 尝试从对方获取其队伍接口
			else if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
			{
				// 获取对方的TeamId
				FGenericTeamId OtherTeamId = TeamAgent->GetGenericTeamId();
	
				// 如果对方的TeamId和自己的TeamId有效且相同
				if (OtherTeamId.GetId() != FGenericTeamId::NoTeam && GetGenericTeamId().GetId() == OtherTeamId.GetId())
				{
					// 是友军
					return ETeamAttitude::Friendly;
				}
				else
				{
					// 是敌人
					return ETeamAttitude::Hostile;
				}
			}
			//OtherPawn->GetClass()->ImplementsInterface(UGenericTeamAgentInterface::StaticClass())
			
		}
	
		// 默认情况下，如果无法判断，则视为中立或敌对
		return ETeamAttitude::Neutral;
}


// Called every frame
void ALsAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

