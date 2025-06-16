// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GasSystem/Common/LsAbilitySystemComponent.h"
#include "GasSystem/Common/LsCommonAttributeSet.h"
#include "LsPlayerState.generated.h"

/**
 * 玩家状态类
 */
UCLASS()
class LIGHTSOULS_API ALsPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ALsPlayerState();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    ULsCommonAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
    /** 能力系统组件 */
    UPROPERTY()
    ULsAbilitySystemComponent* AbilitySystemComponent;

    /** 属性集 */
    UPROPERTY()
    ULsCommonAttributeSet* AttributeSet;
}; 