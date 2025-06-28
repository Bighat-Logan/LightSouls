// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Common/LsCharacterBase.h"
#include "Actor/Controller/LsAIController.h"
#include "LsEnemyBase.generated.h"

/**
 * 敌人基类
 * 继承自LsCharacterBase，提供敌人特有的功能
 */
UCLASS()
class LIGHTSOULS_API ALsEnemyBase : public ALsCharacterBase
{
	GENERATED_BODY()

public:
	ALsEnemyBase();

protected:
	virtual void BeginPlay() override;

	/** 重写PossessedBy函数来赋值AIController */
	virtual void PossessedBy(AController* NewController) override;

	/** AI控制器引用 */
	UPROPERTY()
	ALsAIController* EnemyAIController;

public:
	virtual void Tick(float DeltaTime) override;

	// 敌人特有的属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float DetectionRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float PatrolRadius;

	// 敌人状态
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	bool bIsPlayerDetected;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	bool bIsAttacking;
	

	// 获取到玩家的距离
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	float GetDistanceToPlayer(ALsCharacterBase* Player) const;

	// 检查是否在攻击范围内
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool IsInAttackRange(ALsCharacterBase* Player) const;

	// 检查是否在检测范围内
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool IsInDetectionRange(ALsCharacterBase* Player) const;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

}; 