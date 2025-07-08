#include "ActorComponent/InteractComponent/InteractorComponent.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Interface/CanInteract.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/LsGameTraceChannels.h"

// 为了方便，这里直接包含了InputComponent，如果项目结构复杂，推荐使用接口转发
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"

UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractorComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;
	
	InteractionVolume = NewObject<UBoxComponent>(OwnerActor, TEXT("InteractionVolume"));
	if (!InteractionVolume) return;

	// 附加到根组件
	if (USceneComponent* OwnerRoot = OwnerActor->GetRootComponent())
	{
		InteractionVolume->AttachToComponent(OwnerRoot, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// 设置属性
	InteractionVolume->SetRelativeLocation(InteractionVolumeLocation);
	InteractionVolume->SetBoxExtent(InteractionVolumeSize);
	InteractionVolume->SetCollisionObjectType(COLLISION_INTERACT);
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(COLLISION_INTERACT, ECR_Overlap);

	// 注册组件
	InteractionVolume->RegisterComponent();
}

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());

	// 确保碰撞体有效再绑定事件
	if (InteractionVolume)
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &UInteractorComponent::OnOverlapBegin);
		InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &UInteractorComponent::OnOverlapEnd);
	}

	// 确保输入已绑定
	if (OwnerPawn && OwnerPawn->IsLocallyControlled())
	{
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(OwnerPawn->InputComponent))
		{
			SetupPlayerInput(EIC);
		}
	}
}

void UInteractorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (InteractionVolume)
	{
		InteractionVolume->DestroyComponent();
		InteractionVolume = nullptr;
	}
}

void UInteractorComponent::SetupPlayerInput(UInputComponent* PlayerInputComponent)
{
	if (PlayerInputComponent)
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &UInteractorComponent::OnInteractPressed);
	}
}

void UInteractorComponent::OnInteractPressed()
{
	if (CurrentInteractableTarget.IsValid())
	{
		AActor* Target = CurrentInteractableTarget.Get();
		// 检查接口是否存在，以防万一
		if (Target && Target->Implements<UCanInteract>())
		{
			// Execute_Interact是UFUNCTION接口的调用方式
			ICanInteract::Execute_Interact(Target, GetOwner<APawn>());
		}
	}
}

void UInteractorComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner() && OtherActor->Implements<UCanInteract>())
	{
		CurrentInteractableTarget = OtherActor;

		if (InteractionPromptWidgetClass)
		{
			if (!InteractionPromptWidgetInstance)
			{
				InteractionPromptWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), InteractionPromptWidgetClass);
			}
			if (InteractionPromptWidgetInstance && !InteractionPromptWidgetInstance->IsInViewport())
			{
				InteractionPromptWidgetInstance->AddToViewport();
			}
		}
	}
}

void UInteractorComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CurrentInteractableTarget.Get())
	{
		CurrentInteractableTarget.Reset();

		if (InteractionPromptWidgetInstance && InteractionPromptWidgetInstance->IsInViewport())
		{
			InteractionPromptWidgetInstance->RemoveFromParent();
		}
	}
} 