#include "ActorComponent/InteractComponent/InteractableComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Utility/LsGameTraceChannels.h"

UInteractableComponent::UInteractableComponent()
{
	// 设置默认的交互提示文本
	InteractionPromptText = FText::FromString("Interact");
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
    
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    InteractableVolume = NewObject<UBoxComponent>(this, TEXT("InteractableVolume"));
    if (!InteractableVolume) return;

    // 直接附加到此组件
    InteractableVolume->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
    
    InteractableVolume->SetRelativeLocation(InteractableVolumeLocation);
    InteractableVolume->SetBoxExtent(InteractableVolumeSize);

    // 设置碰撞，使其能被InteractorComponent检测到
    InteractableVolume->SetCollisionObjectType(COLLISION_INTERACT); 
    InteractableVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractableVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractableVolume->SetCollisionResponseToChannel(COLLISION_INTERACT, ECR_Overlap); 

    InteractableVolume->RegisterComponent();
}

void UInteractableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (InteractableVolume)
    {
        InteractableVolume->DestroyComponent();
        InteractableVolume = nullptr;
    }
}

void UInteractableComponent::OnInteraction(AActor* Interactor)
{
	// 基类实现 - 打印一条日志用于调试
	if (GetOwner() && Interactor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s was interacted with by %s."), *GetOwner()->GetName(), *Interactor->GetName());
	}
} 