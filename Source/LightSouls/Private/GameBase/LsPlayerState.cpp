// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBase/LsPlayerState.h"

ALsPlayerState::ALsPlayerState()
{
    // 创建能力系统组件
    AbilitySystemComponent = CreateDefaultSubobject<ULsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // 创建属性集
    AttributeSet = CreateDefaultSubobject<ULsCommonAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ALsPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
} 