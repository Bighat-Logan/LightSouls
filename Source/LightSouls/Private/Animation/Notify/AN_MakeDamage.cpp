#include "Animation/Notify/AN_MakeDamage.h"
#include "Actor/Common/AttackVolume.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Interface/CanAttackInterface.h"

UAN_MakeDamage::UAN_MakeDamage()
{
}

void UAN_MakeDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp )
    {
        return;
    }
    
    // 检查是否实现了CanAttackInterface接口
    if (MeshComp->GetOwner() && MeshComp->GetOwner()->GetClass()->ImplementsInterface(UCanAttackInterface::StaticClass()))
    {
        ICanAttackInterface* InterfaceInstance = Cast<ICanAttackInterface>(MeshComp->GetOwner());
        TArray<AAttackVolume*> AttackVolumes = InterfaceInstance->Execute_GetAllAttackVolume(MeshComp->GetOwner());
        
        // 遍历每个攻击体积并开始伤害事件
        for (AAttackVolume* AttackVolume : AttackVolumes)
        {
            if (AttackVolume)
            {
                AttackVolume->BeginWeaponDamageEvent();
            }
        }
        
    }

    
}


void UAN_MakeDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp )
    {
        return;
    }

    // 获取角色
    ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
    if (!Character)
    {
        return;
    }
    // 检查是否实现了CanAttackInterface接口
    
    if (Character->GetClass()->ImplementsInterface(UCanAttackInterface::StaticClass()))
    {
        ICanAttackInterface* AttackInterface = Cast<ICanAttackInterface>(Character);
        // 获取所有攻击体积
        TArray<AAttackVolume*> AttackVolumes = AttackInterface->Execute_GetAllAttackVolume(Character);
        
        // 遍历每个攻击体积并开始伤害事件
        for (AAttackVolume* AttackVolume : AttackVolumes)
        {
            if (AttackVolume)
            {
                AttackVolume->EndWeaponDamageEvent();
            }
        }
    }
    

} 