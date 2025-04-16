#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GasSystem/Ability/LsCombatGameplayAbility.h"
#include "LsUtilityLibrary.generated.h"

/**
 * 工具函数库，提供可以在蓝图中调用的静态函数
 */
UCLASS()
class LIGHTSOULS_API ULsUtilityLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** 获取冲击力对应的浮点数值 */
    UFUNCTION(BlueprintPure, Category = "LightSouls|Utility")
    static float GetImpactForceValueFromEnum(ELsImpactForce ImpactForce);

	UFUNCTION(BlueprintPure, Category = "LightSouls|Utility")
	static ELsImpactForce GetImpactForceEnumFromValue(float ImpactForceValue);

    /** 获取冲击力对应的显示名称 */
    UFUNCTION(BlueprintPure, Category = "LightSouls|Utility")
    static FText GetImpactForceDisplayName(ELsImpactForce ImpactForce);

    /** 计算两个向量之间的水平角度（忽略Z轴） */
    UFUNCTION(BlueprintPure, Category = "LightSouls|Utility")
    static float CalculateHorizontalAngle(const FVector& From, const FVector& To);

    /** 检查两个向量是否在指定的角度范围内 */
    UFUNCTION(BlueprintPure, Category = "LightSouls|Utility")
    static bool IsInAngleRange(const FVector& Forward, const FVector& Target, float MaxAngle);

    /** 将世界坐标转换为相对于角色的局部坐标 */
    UFUNCTION(BlueprintPure, Category = "LightSouls|Utility")
    static FVector WorldToLocalSpace(const FVector& WorldLocation, const AActor* ReferenceActor);

    /** 将局部坐标转换为世界坐标 */
    UFUNCTION(BlueprintPure, Category = "LightSouls|Utility")
    static FVector LocalToWorldSpace(const FVector& LocalLocation, const AActor* ReferenceActor);
}; 