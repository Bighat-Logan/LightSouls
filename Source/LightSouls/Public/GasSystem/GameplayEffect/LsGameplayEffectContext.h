// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameplayEffectTypes.h"
#include "GasSystem/Ability/LsCombatGameplayAbility.h"
#include "Enums/LsGameplayEnums.h"
#include "LsGameplayEffectContext.generated.h"


USTRUCT(BlueprintType) // 标记为 USTRUCT
struct FLsGameplayEffectContext : public FGameplayEffectContext 
{
	GENERATED_BODY()

public:
	/** 动作值 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	float ActionValue = 0.0f;

	/** 冲击力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	ELsImpactForce ImpactForce = ELsImpactForce::Light;

	/** 设置动作值 */
	void SetActionValue(float Value) { ActionValue = Value; }

	/** 设置冲击力 */
	void SetImpactForce(ELsImpactForce Force) { ImpactForce = Force; }

	/** 获取动作值 */
	float GetActionValue() const { return ActionValue; }

	/** 获取冲击力 */
	ELsImpactForce GetImpactForce() const { return ImpactForce; }

	// --- 必须重写的虚函数 ---

	/** 返回此结构体的 UScriptStruct，用于反射 */
	virtual UScriptStruct* GetScriptStruct() const override;

	/** 创建此 Context 的一个深拷贝副本 */
	virtual FLsGameplayEffectContext* Duplicate() const override;

	/** 处理网络序列化（复制） */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
};

// 为你的自定义结构体添加一个模板特化，用于类型转换（可选但推荐）
template<>
struct TStructOpsTypeTraits<FLsGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FLsGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true, // 需要网络序列化
		WithCopy = true // 需要 Duplicate/Copy 支持
	};
};
