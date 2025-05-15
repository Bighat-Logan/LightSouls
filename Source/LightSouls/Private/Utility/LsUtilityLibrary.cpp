#include "Utility/LsUtilityLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameBase/LsGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "CommonHardwareVisibilityBorder.h"
#include "GameFramework/PlayerController.h"
#include "GasSystem/Tag/LsGameplayTag.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

// Define a log category for easier debugging
DEFINE_LOG_CATEGORY_STATIC(LogLsUtilityLibrary, Log, All);

float ULsUtilityLibrary::GetImpactForceValueFromEnum(ELsImpactForce ImpactForce)
{
    switch (ImpactForce)
    {
    case ELsImpactForce::None:   return 0.0f;
    case ELsImpactForce::Light:  return 1.0f;
    case ELsImpactForce::Medium: return 2.0f;
    case ELsImpactForce::Heavy:  return 3.0f;
    default:
        // 处理未知或无效的枚举值，这里返回 0.0f 作为默认值
            UE_LOG(LogTemp, Warning, TEXT("EnumToFloat: Received unexpected EImpactStrength value. Returning 0.0f."));
        return 0.0f;
    }
}

ELsImpactForce ULsUtilityLibrary::GetImpactForceEnumFromValue(float ImpactForceValue)
{
    // 将 float 值四舍五入到最近的整数，以处理可能的浮点误差
    const int32 RoundedValue = FMath::RoundToInt(ImpactForceValue);

    switch (RoundedValue)
    {
    case 0:  return ELsImpactForce::None;
    case 1:  return ELsImpactForce::Light;
    case 2:  return ELsImpactForce::Medium;
    case 3:  return ELsImpactForce::Heavy;
    default:
        // 如果四舍五入后的值不在 0-3 范围内，返回 None 作为默认值
            // 你也可以根据需要记录一个警告
                if (ImpactForceValue != 0.0f) // 避免完全为0时也报警
                {
                    UE_LOG(LogTemp, Warning, TEXT("FloatToEnum: Received value %f which rounds to %d, outside expected range 0-3. Returning None."), ImpactForceValue, RoundedValue);
                }
        return ELsImpactForce::None;
    }
}

FText ULsUtilityLibrary::GetImpactForceDisplayName(ELsImpactForce ImpactForce)
{
    UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ELsImpactForce"), true);
    if (EnumPtr)
    {
        return EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(ImpactForce));
    }
    return FText::GetEmpty();
}

float ULsUtilityLibrary::CalculateHorizontalAngle(const FVector& From, const FVector& To)
{
    // 忽略Z轴
    FVector From2D = FVector(From.X, From.Y, 0.0f);
    FVector To2D = FVector(To.X, To.Y, 0.0f);
    
    // 归一化向量
    From2D.Normalize();
    To2D.Normalize();
    
    // 计算点积
    float DotProduct = FVector::DotProduct(From2D, To2D);
    // 确保点积在有效范围内
    DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);
    
    // 计算角度（弧度）
    float AngleRad = FMath::Acos(DotProduct);
    // 转换为角度
    float AngleDeg = FMath::RadiansToDegrees(AngleRad);
    
    return AngleDeg;
}

bool ULsUtilityLibrary::IsInAngleRange(const FVector& Forward, const FVector& Target, float MaxAngle)
{
    float CurrentAngle = CalculateHorizontalAngle(Forward, Target);
    return CurrentAngle <= MaxAngle;
}

FVector ULsUtilityLibrary::WorldToLocalSpace(const FVector& WorldLocation, const AActor* ReferenceActor)
{
    if (!ReferenceActor)
    {
        return FVector::ZeroVector;
    }
    
    return ReferenceActor->GetTransform().InverseTransformPosition(WorldLocation);
}

FVector ULsUtilityLibrary::LocalToWorldSpace(const FVector& LocalLocation, const AActor* ReferenceActor)
{
    if (!ReferenceActor)
    {
        return FVector::ZeroVector;
    }
    
    return ReferenceActor->GetTransform().TransformPosition(LocalLocation);
}


FVector ULsUtilityLibrary::NormalizeVector(FVector InVector)
{
    // Find the absolute values of the components
    const float AbsX = FMath::Abs(InVector.X);
    const float AbsY = FMath::Abs(InVector.Y);
    const float AbsZ = FMath::Abs(InVector.Z);

    // Find the maximum absolute value among the components
    const float MaxAbsValue = FMath::Max3(AbsX, AbsY, AbsZ);

    // Check if the maximum absolute value is significant (greater than a very small number)
    // Avoid division by zero or near-zero values.
    if (MaxAbsValue > KINDA_SMALL_NUMBER) // KINDA_SMALL_NUMBER is a small float constant defined in UnrealMathUtility.h
    {
        // Divide the original vector by the maximum absolute value.
        // This scales the component with the largest magnitude to 1 or -1,
        // and other components proportionally within [-1, 1].
        return InVector / MaxAbsValue;
    }
    else
    {
        // If the maximum absolute value is zero (or very close to it),
        // the input vector is essentially the zero vector. Return the zero vector.
        return FVector::ZeroVector;
    }
}

TArray<UMaterialInstanceDynamic*> ULsUtilityLibrary::ApplyGenericDissolveMaterialToSkeletalMesh(
    USkeletalMeshComponent* MeshComponent,
    UMaterialInterface* GenericDissolveMaterial,
    FName DissolveAmountParameterName)
{
    TArray<UMaterialInstanceDynamic*> CreatedMIDs;

    if (!IsValid(MeshComponent))
    {
        UE_LOG(LogLsUtilityLibrary, Warning, TEXT("ApplyGenericDissolveMaterialToSkeletalMesh: Invalid MeshComponent provided."));
        return CreatedMIDs;
    }

    if (!IsValid(GenericDissolveMaterial))
    {
        UE_LOG(LogLsUtilityLibrary, Warning, TEXT("ApplyGenericDissolveMaterialToSkeletalMesh: Invalid GenericDissolveMaterial provided."));
        return CreatedMIDs;
    }

    if (DissolveAmountParameterName == NAME_None)
    {
         UE_LOG(LogLsUtilityLibrary, Warning, TEXT("ApplyGenericDissolveMaterialToSkeletalMesh: Invalid DissolveAmountParameterName provided (cannot be None)."));
        return CreatedMIDs;
    }

    const int32 NumMaterials = MeshComponent->GetNumMaterials();
    CreatedMIDs.Reserve(NumMaterials); // Optimize array allocation

    UE_LOG(LogLsUtilityLibrary, Log, TEXT("Applying generic dissolve material to %d slots for component %s."), NumMaterials, *MeshComponent->GetName());

    for (int32 MaterialIndex = 0; MaterialIndex < NumMaterials; ++MaterialIndex)
    {
        UMaterialInterface* OriginalMaterial = MeshComponent->GetMaterial(MaterialIndex);

        // Create the Dynamic Instance based on the Generic Dissolve Material
        // This function automatically sets the material on the component slot
        UMaterialInstanceDynamic* DissolveMID = MeshComponent->CreateDynamicMaterialInstance(MaterialIndex, GenericDissolveMaterial);

        if (IsValid(DissolveMID))
        {
             UE_LOG(LogLsUtilityLibrary, Verbose, TEXT("Slot %d: Created MID '%s'."), MaterialIndex, *DissolveMID->GetName());

            // Attempt to copy parameters from the original material (if valid)
            if (IsValid(OriginalMaterial))
            {
                 UE_LOG(LogLsUtilityLibrary, Verbose, TEXT("Slot %d: Copying parameters from original material '%s'."), MaterialIndex, *OriginalMaterial->GetName());
                CopyMaterialParameters(OriginalMaterial, DissolveMID, DissolveAmountParameterName);
            }
            else
            {
                UE_LOG(LogLsUtilityLibrary, Warning, TEXT("Slot %d: Original material was invalid, skipping parameter copy."), MaterialIndex);
            }

            CreatedMIDs.Add(DissolveMID);
        }
        else
        {
            UE_LOG(LogLsUtilityLibrary, Error, TEXT("Slot %d: Failed to create Dynamic Material Instance from GenericDissolveMaterial '%s'."), MaterialIndex, *GenericDissolveMaterial->GetName());
        }
    }

     UE_LOG(LogLsUtilityLibrary, Log, TEXT("Finished applying generic dissolve material. Created %d MIDs."), CreatedMIDs.Num());
    return CreatedMIDs;
}


void ULsUtilityLibrary::CopyMaterialParameters(UMaterialInterface* SourceMaterial, UMaterialInstanceDynamic* TargetMID, FName ExcludeScalarParameterName)
{
    if (!IsValid(SourceMaterial) || !IsValid(TargetMID))
    {
        return;
    }

    // --- Copy Texture Parameters ---
    TArray<FMaterialParameterInfo> TextureParamInfos;
    TArray<FGuid> TextureParamGuids;
    SourceMaterial->GetAllTextureParameterInfo(TextureParamInfos, TextureParamGuids);

    for (const FMaterialParameterInfo& ParamInfo : TextureParamInfos)
    {
        UTexture* SourceTexture = nullptr;
        if (SourceMaterial->GetTextureParameterValue(ParamInfo, SourceTexture) && IsValid(SourceTexture))
        {
            // Check if the target MID *actually has* this parameter before setting
             // Note: This check isn't strictly necessary with MIDs derived from a known parent,
             // but adds safety if the generic material might change or lack a parameter.
             // We'll rely on SetTextureParameterValue handling missing params gracefully for now.
             UE_LOG(LogLsUtilityLibrary, VeryVerbose, TEXT("Copying Texture Param '%s'."), *ParamInfo.Name.ToString());
            TargetMID->SetTextureParameterValue(ParamInfo.Name, SourceTexture);
        }
    }

    // --- Copy Vector Parameters ---
    TArray<FMaterialParameterInfo> VectorParamInfos;
    TArray<FGuid> VectorParamGuids;
    SourceMaterial->GetAllVectorParameterInfo(VectorParamInfos, VectorParamGuids);

    for (const FMaterialParameterInfo& ParamInfo : VectorParamInfos)
    {
        FLinearColor SourceVector;
        if (SourceMaterial->GetVectorParameterValue(ParamInfo, SourceVector))
        {
             UE_LOG(LogLsUtilityLibrary, VeryVerbose, TEXT("Copying Vector Param '%s'."), *ParamInfo.Name.ToString());
            TargetMID->SetVectorParameterValue(ParamInfo.Name, SourceVector);
        }
    }

    // --- Copy Scalar Parameters ---
    TArray<FMaterialParameterInfo> ScalarParamInfos;
    TArray<FGuid> ScalarParamGuids;
    SourceMaterial->GetAllScalarParameterInfo(ScalarParamInfos, ScalarParamGuids);

    for (const FMaterialParameterInfo& ParamInfo : ScalarParamInfos)
    {
        // IMPORTANT: Do not copy the dissolve control parameter itself!
        if (ParamInfo.Name == ExcludeScalarParameterName)
        {
             UE_LOG(LogLsUtilityLibrary, VeryVerbose, TEXT("Skipping excluded Scalar Param '%s'."), *ParamInfo.Name.ToString());
            continue;
        }

        float SourceScalar;
        if (SourceMaterial->GetScalarParameterValue(ParamInfo, SourceScalar))
        {
            UE_LOG(LogLsUtilityLibrary, VeryVerbose, TEXT("Copying Scalar Param '%s'."), *ParamInfo.Name.ToString());
            TargetMID->SetScalarParameterValue(ParamInfo.Name, SourceScalar);
        }
    }
     UE_LOG(LogLsUtilityLibrary, Verbose, TEXT("Parameter copy complete for MID '%s'."), *TargetMID->GetName());
}

void ULsUtilityLibrary::SetScalarParameterOnSkeletalMeshMIDs(
	USkeletalMeshComponent* MeshComponent,
	FName ParameterName,
	float ParameterValue)
{
	// --- 输入有效性检查 ---
	if (!IsValid(MeshComponent))
	{
		UE_LOG(LogLsUtilityLibrary, Warning, TEXT("SetScalarParameterOnSkeletalMeshMIDs: 传入的 MeshComponent 无效。"));
		return;
	}

	if (ParameterName == NAME_None)
	{
		UE_LOG(LogLsUtilityLibrary, Warning, TEXT("SetScalarParameterOnSkeletalMeshMIDs: 传入的 ParameterName 无效 (不能是 None)。"));
		return;
	}

	const int32 NumMaterials = MeshComponent->GetNumMaterials(); // 获取材质槽数量
	int32 MIDsModifiedCount = 0; // 计数器，记录修改了多少个 MID
	
	// --- 遍历所有材质槽 ---
	for (int32 MaterialIndex = 0; MaterialIndex < NumMaterials; ++MaterialIndex)
	{
		UMaterialInterface* CurrentMaterial = MeshComponent->GetMaterial(MaterialIndex); // 获取当前槽位的材质接口

		if (IsValid(CurrentMaterial))
		{
			// 尝试将当前材质接口转换为动态材质实例 (MID)
			// 只有 MID 才能在运行时动态设置参数
			UMaterialInstanceDynamic* CurrentMID = Cast<UMaterialInstanceDynamic>(CurrentMaterial);

			if (IsValid(CurrentMID))
			{
				// 成功转换为 MID，直接尝试设置参数值
				// SetScalarParameterValue 如果参数不存在，通常不会报错，只是没有效果
				UE_LOG(LogLsUtilityLibrary, VeryVerbose, TEXT("槽位 %d: 正在 MID '%s' 上设置参数 '%s'。"),
					MaterialIndex, *CurrentMID->GetName(), *ParameterName.ToString());

				CurrentMID->SetScalarParameterValue(ParameterName, ParameterValue);
				MIDsModifiedCount++; // 增加计数
			}
			else
			{
				// 当前材质不是 MID，无法动态修改
				UE_LOG(LogLsUtilityLibrary, VeryVerbose, TEXT("槽位 %d: 材质 '%s' 不是动态材质实例(MID)，无法动态设置参数。请先为其创建 MID。"),
					MaterialIndex, *CurrentMaterial->GetName());
				// 注意：我们不在这里自动创建 MID，因为这可能不是用户的意图
				// 用户应该先使用 CreateDynamicMaterialInstance 或之前的 ApplyGeneric... 函数确保材质是 MID
			}
		}
		else
		{
			UE_LOG(LogLsUtilityLibrary, VeryVerbose, TEXT("槽位 %d: 没有有效的材质。"), MaterialIndex);
		}
	}
}


TArray<UMaterialInstanceDynamic*> ULsUtilityLibrary::EnsureSkeletalMeshMaterialsAreDynamic(
	USkeletalMeshComponent* MeshComponent)
{
	TArray<UMaterialInstanceDynamic*> ResultMIDs; // 初始化用于存储最终 MID 的数组

	// --- 输入有效性检查 ---
	if (!IsValid(MeshComponent))
	{
		UE_LOG(LogLsUtilityLibrary, Warning, TEXT("EnsureSkeletalMeshMaterialsAreDynamic: 传入的 MeshComponent 无效。"));
		return ResultMIDs; // 返回空数组
	}

	const int32 NumMaterials = MeshComponent->GetNumMaterials(); // 获取材质槽数量
	ResultMIDs.Reserve(NumMaterials); // 预分配数组内存

	UE_LOG(LogLsUtilityLibrary, Log, TEXT("正在确保组件 %s 的 %d 个材质槽使用动态材质实例(MID)。"), *MeshComponent->GetName(), NumMaterials);

	// --- 遍历所有材质槽 ---
	for (int32 MaterialIndex = 0; MaterialIndex < NumMaterials; ++MaterialIndex)
	{
		UMaterialInterface* CurrentMaterial = MeshComponent->GetMaterial(MaterialIndex); // 获取当前槽位的材质接口
		UMaterialInstanceDynamic* ResultMID = nullptr; // 用于存储当前槽位最终的 MID

		// 尝试将当前材质转换为 MID，检查是否已经是动态的
		UMaterialInstanceDynamic* ExistingMID = Cast<UMaterialInstanceDynamic>(CurrentMaterial);

		if (IsValid(ExistingMID))
		{
			// 如果已经是 MID，则直接重用
			UE_LOG(LogLsUtilityLibrary, Verbose, TEXT("槽位 %d: 材质 '%s' 已经是动态实例(MID)，直接重用。"), MaterialIndex, *ExistingMID->GetName());
			ResultMID = ExistingMID;
		}
		else
		{
			// 如果不是 MID，需要基于当前材质创建新的 MID
			if (IsValid(CurrentMaterial))
			{
				// CreateDynamicMaterialInstance 会基于 CurrentMaterial 创建新的 MID，并自动设置到该槽位
				ResultMID = MeshComponent->CreateDynamicMaterialInstance(MaterialIndex, CurrentMaterial);

				if (IsValid(ResultMID))
				{
					UE_LOG(LogLsUtilityLibrary, Verbose, TEXT("槽位 %d: 基于材质 '%s' 成功创建了新的动态实例(MID) '%s'。"), MaterialIndex, *CurrentMaterial->GetName(), *ResultMID->GetName());
				}
				else
				{
					// 创建 MID 失败
					UE_LOG(LogLsUtilityLibrary, Error, TEXT("槽位 %d: 基于材质 '%s' 创建动态实例(MID) 失败。"), MaterialIndex, *CurrentMaterial->GetName());
					// ResultMID 此时为 nullptr
				}
			}
			else
			{
				// 原始材质无效，无法创建 MID
				UE_LOG(LogLsUtilityLibrary, Warning, TEXT("槽位 %d: 原始材质无效，无法创建动态实例(MID)。"), MaterialIndex);
				// ResultMID 此时为 nullptr
			}
		}

		// 如果当前槽位成功获得或创建了 MID，则将其添加到结果数组中
		if (IsValid(ResultMID))
		{
			ResultMIDs.Add(ResultMID);
		}
        else
        {
            // 可选：如果创建或获取失败，可以考虑添加一个 nullptr 到数组，或者不添加
            // 不添加可能更符合预期，返回的数组只包含有效的 MID
             UE_LOG(LogLsUtilityLibrary, Warning, TEXT("槽位 %d: 未能获取或创建有效的 MID。"), MaterialIndex);
        }
	}

	UE_LOG(LogLsUtilityLibrary, Log, TEXT("为组件 %s 完成材质动态化处理。最终获得 %d 个有效 MID。"), *MeshComponent->GetName(), ResultMIDs.Num());
	return ResultMIDs; // 返回包含所有有效 MID 的数组
}

ULsGameInstance* ULsUtilityLibrary::GetLsGameInstance(const UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return nullptr;
    }

    return Cast<ULsGameInstance>(World->GetGameInstance());
}

void ULsUtilityLibrary::ToggleInGameMenu(UCommonActivatableWidgetStack* WidgetStack)
{
    if (!WidgetStack)
    {
        UE_LOG(LogTemp, Warning, TEXT("ToggleInGameMenu: WidgetStack is null"));
        return;
    }

    // 获取游戏实例
    ULsGameInstance* GameInstance = GetLsGameInstance(WidgetStack);
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("ToggleInGameMenu: Failed to get GameInstance"));
        return;
    }

    // 获取玩家控制器
    APlayerController* PC = Cast<APlayerController>(WidgetStack->GetOwningPlayer());
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("ToggleInGameMenu: Failed to get PlayerController"));
        return;
    }

    // 定义菜单标签
    static const FGameplayTag MenuTag = FGameplayTag::RequestGameplayTag(FLsGameplayTags::Get().Player_State_InOptionMenu.GetTagName());

    // 检查是否已经显示菜单
    bool bIsMenuVisible = GameInstance->GlobalGameTags.HasTag(MenuTag);

    if (!bIsMenuVisible)
    {
        // 显示菜单
        WidgetStack->SetVisibility(ESlateVisibility::Visible);
        
        // 暂停游戏
        PC->SetPause(true);
        
        // 显示光标
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeGameAndUI());
        
        // 添加标签
        GameInstance->GlobalGameTags.AddTag(MenuTag);
        
        UE_LOG(LogTemp, Log, TEXT("ToggleInGameMenu: Menu shown"));
    }
    else
    {
        // 隐藏菜单
        WidgetStack->SetVisibility(ESlateVisibility::Hidden);
        
        // 继续游戏
        PC->SetPause(false);
        
        // 隐藏光标
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
        
        // 移除标签
        GameInstance->GlobalGameTags.RemoveTag(MenuTag);
        
        UE_LOG(LogTemp, Log, TEXT("ToggleInGameMenu: Menu hidden"));
    }
}