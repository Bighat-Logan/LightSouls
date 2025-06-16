#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "GasSystem/Common/LsCommonAttributeSet.h"
#include "GasSystem/GameplayEffect/EffectExecutionCalculation/MMC_StaminaRegen.h"
#include "GE_StaminaRegen.generated.h"

/**
 * 持续回复精力值的GameplayEffect
 */
UCLASS()
class LIGHTSOULS_API UGE_StaminaRegen : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_StaminaRegen()
	{
		// 设置为持续效果
		DurationPolicy = EGameplayEffectDurationType::Infinite;
	}

	virtual void PostLoad() override
	{
		Super::PostLoad();
		
		// 设置堆叠类型
		StackingType = EGameplayEffectStackingType::AggregateBySource;
		StackLimitCount = 1;

		// 设置效果组
		FGameplayModifierInfo ModInfo;
		ModInfo.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(ULsCommonAttributeSet::StaticClass(), TEXT("Stamina")));
		ModInfo.ModifierOp = EGameplayModOp::Additive;
		
		//ModInfo.ModifierMagnitude = FScalableFloat(DefaultRegenAmount);
		// 1. 设置Modifier的数值计算方式为“自定义类计算” (Custom Calculation)
		
		FCustomCalculationBasedFloat CustomCalcBasedFloat;

		// 2. 将我们的MMC类设置到这个结构体的对应成员上。
		//    这个结构体的成员是 public 的，可以直接访问。
		CustomCalcBasedFloat.CalculationClassMagnitude = UMMC_StaminaRegen::StaticClass();

		// 3. 最后，使用这个准备好的结构体，通过构造函数来创建 FGameplayEffectModifierMagnitude。
		ModInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(CustomCalcBasedFloat);

		Modifiers.Add(ModInfo);

		
	}
	
	
}; 