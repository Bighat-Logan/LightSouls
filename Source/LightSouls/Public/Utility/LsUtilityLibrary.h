#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GasSystem/Ability/LsCombatGameplayAbility.h"
#include "GameBase/LsGameInstance.h"
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
    
    /**
     * Scales the components of a vector proportionally based on the maximum absolute component value,
     * ensuring all components fall within the range [-1, 1].
     * If the input vector is the zero vector, it returns the zero vector.
     *
     * @param InVector The vector to scale.
     * @return A new vector with components proportionally scaled to the [-1, 1] range.
     * @warning This is NOT the same as standard vector normalization (scaling to unit length).
     */
    UFUNCTION(BlueprintCallable, Category = "LsUtility|Math|Vector", meta = (DisplayName = "Normalize Vector by Max Component (-1 to 1)", Keywords = "scale vector component range limit max absolute proportional"))
    static FVector NormalizeVector(FVector InVector);


	// (之前的 ApplyGenericDissolveMaterialToSkeletalMesh 函数声明)

	/**
	 * 设置骨骼网格体组件上所有动态材质实例 (MID) 中具有指定名称的标量参数的值。
	 * 只会影响当前已经是动态材质实例的材质槽。
	 *
	 * @param MeshComponent 要修改的骨骼网格体组件。
	 * @param ParameterName 要设置的标量参数的名称。
	 * @param ParameterValue 要设置的浮点数值。
	 */
	UFUNCTION(BlueprintCallable, Category = "LS Utility Library | Materials", meta = (DisplayName = "Set Scalar Parameter On Skeletal Mesh MIDs"))
	static void SetScalarParameterOnSkeletalMeshMIDs(
		USkeletalMeshComponent* MeshComponent,
		FName ParameterName,
		float ParameterValue
	);



	/**
	 * Applies a generic dissolve material to all slots of a Skeletal Mesh Component,
	 * creates Dynamic Material Instances (MIDs), and attempts to copy parameters
	 * (Texture, Vector, Scalar) from the original materials to the new MIDs.
	 *
	 * @param MeshComponent The Skeletal Mesh Component to modify.
	 * @param GenericDissolveMaterial The master material (or instance) to use for dissolving. Must contain parameters matching originals + a dissolve control parameter.
	 * @param DissolveAmountParameterName The exact name of the Scalar Parameter within GenericDissolveMaterial used to control the dissolve amount (e.g., "DissolveAmount").
	 * @return An array containing the newly created Dynamic Material Instances for each slot. Empty if inputs are invalid or no materials were processed.
	 */
	UFUNCTION(BlueprintCallable, Category = "LS Utility Library | Materials", meta = (DisplayName = "Apply Generic Dissolve Material To Skeletal Mesh"))
	static TArray<UMaterialInstanceDynamic*> ApplyGenericDissolveMaterialToSkeletalMesh(
		USkeletalMeshComponent* MeshComponent,
		UMaterialInterface* GenericDissolveMaterial,
		FName DissolveAmountParameterName = "DissolveAmount" // Provide a sensible default
	);

	static void CopyMaterialParameters(UMaterialInterface* SourceMaterial, UMaterialInstanceDynamic* TargetMID, FName ExcludeScalarParameterName);


	// (之前的函数声明) ...

	/**
	 * 确保骨骼网格体组件的所有材质槽都使用动态材质实例 (MID)。
	 * 它会遍历所有槽位，如果槽位上的材质不是 MID，则基于原始材质创建一个新的 MID。
	 * 如果槽位上已经是 MID，则直接使用现有的 MID。
	 *
	 * @param MeshComponent 要处理的骨骼网格体组件。
	 * @return 一个包含所有槽位对应 MID 的数组（新创建的或已存在的）。如果输入无效或处理失败，可能为空或不完整。
	 */
	UFUNCTION(BlueprintCallable, Category = "LS Utility Library | Materials", meta = (DisplayName = "Ensure Skeletal Mesh Materials Are Dynamic"))
	static TArray<UMaterialInstanceDynamic*> EnsureSkeletalMeshMaterialsAreDynamic(
		USkeletalMeshComponent* MeshComponent
	);

    // 切换游戏内菜单
    UFUNCTION(BlueprintCallable, Category = "UI")
    static void ToggleInGameMenu(UCommonActivatableWidgetStack* WidgetStack);

    // 获取游戏实例
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game Instance")
    static ULsGameInstance* GetLsGameInstance(const UObject* WorldContextObject);

    /**
     * 计算一个点到平面的距离
     * @param Point 要计算距离的点
     * @param PlaneOrigin 平面上的一个点
     * @param PlaneNormal 平面的法向量（必须是单位向量）
     * @return 点到平面的距离（正值表示点在平面法向量方向，负值表示在反方向）
     */
    UFUNCTION(BlueprintPure, Category = "LightSouls|Utility|Math")
    static float GetDistanceToPlane(const FVector& Point, const FVector& PlaneOrigin, const FVector& PlaneNormal);
}; 