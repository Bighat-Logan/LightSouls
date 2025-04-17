
#include "GasSystem/Ability/LsCombatGameplayAbility.h"
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GasSystem/GameplayEffect/LsGameplayEffectContext.h"

ULsCombatGameplayAbility::ULsCombatGameplayAbility()
{
}

void ULsCombatGameplayAbility::MakeGameplayEffectSpecsFromContainer(const FLsGameplayEffectContainer& Container,
    FLsGameplayEffectContainerSpec& OutSpec, int32 OverrideGameplayLevel)
{
    for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
    {
        OutSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingLsGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
    }
    UE_LOG(LogTemp, Warning, TEXT("WCNM"));
}

FGameplayEffectSpecHandle ULsCombatGameplayAbility::MakeOutgoingLsGameplayEffectSpec(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
    
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

    if (!ASC || !EffectClass) 
    {
        UE_LOG(LogTemp, Error, TEXT("SourceASC or EffectClass is null!"));
    }
    
    FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();

    if (EffectContextHandle.IsValid())
    {
        // 使用 GetMutable() 获取非 const 指针以便修改
        // 同时进行类型转换，最好加上检查确保转换是安全的
        FLsGameplayEffectContext* LsGameplayEffectContext = static_cast<FLsGameplayEffectContext*>(EffectContextHandle.Get()); // 使用 GetMutable()

        // （可选但推荐）检查转换后的指针是否为空，或进行更安全的类型检查
        // 例如：FLsGameplayEffectContext* LsGameplayEffectContext = Cast<FLsGameplayEffectContext>(EffectContextHandle.GetMutable());
        if (LsGameplayEffectContext) // 确保转换成功（如果类型不匹配，static_cast 可能不会返回 null，但访问会出错）
        {
            LsGameplayEffectContext->SetActionValue(ActionValue);
            LsGameplayEffectContext->SetImpactForce(ImpactForce);
            // 在这里设置你自定义 Context 的其他数据
        }
        else
        {
            // 如果你的项目应该总是创建 FLsGameplayEffectContext，但这里却得到了别的类型，记录错误
            UE_LOG(LogTemp, Error, TEXT("MakeOutgoingLsGameplayEffectSpec: Failed to cast GameplayEffectContext to FLsGameplayEffectContext. Is AbilitySystemGlobals configured correctly?"));
            // 可能需要决定是否继续执行或返回无效 Handle
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MakeOutgoingLsGameplayEffectSpec: MakeEffectContext returned an invalid handle!"));
        return FGameplayEffectSpecHandle(); // 返回无效 Handle
    }
    
    FGameplayEffectSpecHandle ReturnSpecHandle = ASC->MakeOutgoingSpec(EffectClass, Level, EffectContextHandle); // <--- 使用原始的 Handle

    // 调试日志：检查创建后的 Spec 中的 Context 指针是否与原始 Handle 中的一致
    UE_LOG(LogTemp, Warning, TEXT("MakeOutgoingLsGameplayEffectSpec: Original Context Ptr: %p"), EffectContextHandle.Get());
    UE_LOG(LogTemp, Warning, TEXT("MakeOutgoingLsGameplayEffectSpec: Context Ptr in ReturnSpecHandle: %p"), ReturnSpecHandle.IsValid() ? ReturnSpecHandle.Data->GetContext().Get() : nullptr);

    // （可选）更深入的即时验证：在返回前就检查 Spec 内 Context 的值
    if (ReturnSpecHandle.IsValid() && ReturnSpecHandle.Data.IsValid())
    {
        FLsGameplayEffectContext* ContextInsideSpec = static_cast<FLsGameplayEffectContext*>(ReturnSpecHandle.Data->GetContext().Get());
        if (ContextInsideSpec)
        {
            // 比如读取 ActionValue 看是否正确 (需要你有对应的 Get 方法)
            // UE_LOG(LogTemp, Log, TEXT("ActionValue inside spec immediately after creation: %f"), ContextInsideSpec->GetActionValue());
        } else {
            UE_LOG(LogTemp, Warning, TEXT("Context inside created spec is not of type FLsGameplayEffectContext or is null!"));
        }
    }

    
    return ReturnSpecHandle;
} 