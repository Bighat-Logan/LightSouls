#include "Utility/LsUtilityLibrary.h"
#include "Kismet/KismetMathLibrary.h"

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