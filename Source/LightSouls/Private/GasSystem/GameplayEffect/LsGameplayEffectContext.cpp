// Fill out your copyright notice in the Description page of Project Settings.


#include "GasSystem/GameplayEffect/LsGameplayEffectContext.h"


UScriptStruct* FLsGameplayEffectContext::GetScriptStruct() const
{
	return FLsGameplayEffectContext::StaticStruct();
}

FLsGameplayEffectContext* FLsGameplayEffectContext::Duplicate() const
{
	// 创建一个新的你自定义类型的实例
	FLsGameplayEffectContext* NewContext = new FLsGameplayEffectContext();
	// 调用基类的复制操作来复制所有基类成员
	*NewContext = *this;
	// 设置复制标志（如果需要，尽管基类通常会处理）
	NewContext->AddActors(Actors);
	if (GetHitResult())
	{
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	// 返回新创建的副本指针
	return NewContext;
}

bool FLsGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	// 1. 首先调用基类的 NetSerialize 来序列化所有基类数据
	Super::NetSerialize(Ar, Map, bOutSuccess);

	// 2. 序列化你添加的自定义成员
	// Ar << CustomDataType;
	// Ar << AdditionalData;

	bOutSuccess = true; // 标记序列化成功
	return true;
}
