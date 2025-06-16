#include "GasSystem/Ability/AISelectMeleeAttackAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GasSystem/Tag/LsGameplayTag.h"

UAISelectMeleeAttackAbility::UAISelectMeleeAttackAbility()
{
}


void UAISelectMeleeAttackAbility::PostInitProperties()
{
    Super::PostInitProperties(); // 非常重要，调用父类的实现

    AbilityTags.AddTag(FLsGameplayTags::Get().Ability_AISelectMeleeAttack);

        
    // 设置激活标签
    FAbilityTriggerData DefaultTriggerData;
    DefaultTriggerData.TriggerTag = FLsGameplayTags::Get().Ability_AISelectMeleeAttack; // 替换成你需要的Tag
    DefaultTriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent; // 这个其实在构造函数里默认设置了

    AbilityTriggers.Empty();
    AbilityTriggers.Add(DefaultTriggerData);

    // 设置能力触发策略
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UAISelectMeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 获取目标Actor
    AActor* OwningActor = GetOwningActorFromActorInfo();
    if (!OwningActor)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 从可用攻击列表中选择一个攻击能力
    TSubclassOf<UGameplayAbility> SelectedAbility = SelectRandomAttackAbility(DefaultSelectTag);
    if (!SelectedAbility)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 尝试激活选中的攻击能力
    bool bSuccess = ActorInfo->AbilitySystemComponent->TryActivateAbilityByClass(SelectedAbility);
    if (!bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to activate selected attack ability"));
    }

    // 结束当前能力
    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}


TSubclassOf<UGameplayAbility> UAISelectMeleeAttackAbility::SelectRandomAttackAbility(FGameplayTag SelectTag) const
{
    // 定义候选能力列表
    TArray<TSubclassOf<UGameplayAbility>> CandidateAbilities;

    // 获取拥有者的ASC
    AActor* OwningActor = GetOwningActorFromActorInfo();
    if (!OwningActor)
    {
        return nullptr;
    }

    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);
    if (!ASC)
    {
        return nullptr;
    }

    // 获取所有符合SelectTag的能力
    TArray<FGameplayAbilitySpec*> AbilitySpecs;
    ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(SelectTag), AbilitySpecs);

    // 将符合条件的能力类添加到候选列表中
    CandidateAbilities.Empty();
    for (const FGameplayAbilitySpec* AbilitySpec : AbilitySpecs)
    {
        if (AbilitySpec && AbilitySpec->Ability)
        {
            CandidateAbilities.Add(AbilitySpec->Ability->GetClass());
        }
    }

    // 随机选择一个攻击能力
    int32 RandomIndex = FMath::RandRange(0, CandidateAbilities.Num() - 1);
    return CandidateAbilities[RandomIndex];
} 