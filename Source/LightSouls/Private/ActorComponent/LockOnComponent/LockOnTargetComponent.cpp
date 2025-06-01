// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/LockOnComponent/LockOnTargetComponent.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"


// Sets default values for this component's properties
ULockOnTargetComponent::ULockOnTargetComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // 不需要Tick
    DefaultBoxExtent = FVector(15.0f, 15.0f, 15.0f);
    bDrawDebugBoxes = false;
    // **重要**: 在这里设置默认的碰撞通道，或者确保在蓝图中正确设置
    // 例如: CollisionChannelToUse = ECC_GameTraceChannel1; // 替换为你的LockOn通道
    // 最好是在蓝图的默认值中设置此项，或者通过构造函数参数传入
}

void ULockOnTargetComponent::CreateLockOnBoxes()
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    USkeletalMeshComponent* OwnerSkelMesh = OwnerActor->FindComponentByClass<USkeletalMeshComponent>();

    for (int32 i = 0; i < LockOnPointsConfig.Num(); ++i)
    {
        const FLockOnPointInfo& PointInfo = LockOnPointsConfig[i];

        UBoxComponent* LockOnBox = NewObject<UBoxComponent>(OwnerActor, FName(*FString::Printf(TEXT("LockOnBox_%d"), i)));
        if (!LockOnBox) continue;

        // 设置碰撞属性
        LockOnBox->SetCollisionObjectType(CollisionChannelToUse);
        LockOnBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 只用于查询
        LockOnBox->SetCollisionResponseToAllChannels(ECR_Ignore);     // 忽略所有
        LockOnBox->SetCollisionResponseToChannel(CollisionChannelToUse, ECR_Overlap); // 只对LockOn通道响应Overlap

        FVector ActualBoxExtent = PointInfo.BoxExtent.IsNearlyZero() ? DefaultBoxExtent : PointInfo.BoxExtent;
        LockOnBox->SetBoxExtent(ActualBoxExtent);

        // 附加和定位
        if (OwnerSkelMesh && PointInfo.SocketName != NAME_None && OwnerSkelMesh->DoesSocketExist(PointInfo.SocketName))
        {
            LockOnBox->AttachToComponent(OwnerSkelMesh, FAttachmentTransformRules::KeepRelativeTransform, PointInfo.SocketName);
            LockOnBox->SetRelativeLocation(PointInfo.RelativeOffset); // Socket上的附加偏移
        }
        else
        {
            // 如果没有骨骼网格体，或者SocketName为None，则附加到Owner的根组件或此组件
            // 附加到Owner的根组件通常更稳定
            USceneComponent* AttachParent = OwnerActor->GetRootComponent() ;
            LockOnBox->AttachToComponent(AttachParent, FAttachmentTransformRules::KeepRelativeTransform);
            LockOnBox->SetRelativeLocation(PointInfo.RelativeOffset);
        }

        LockOnBox->RegisterComponent(); // 必须注册动态创建的组件
        CreatedCollisionBoxes.Add(LockOnBox);

        if (bDrawDebugBoxes)
        {
            LockOnBox->SetHiddenInGame(false); // 使其在游戏中可见以进行调试 (会使用组件的形状绘制)
        }
        else
        {
            LockOnBox->SetHiddenInGame(true);
        }
    }

}

void ULockOnTargetComponent::CreateLockOnWidget()
{
    // Check if a widget class has been assigned.
    // 检查是否已分配Widget类。
    if (!LockOnWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("LockOnWidgetClass is not set in ULockOnTargetComponent. Cannot create widgets."));
        return;
    }

    // Get the owner actor of this component.
    // 获取此组件的所有者Actor。
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("ULockOnTargetComponent has no Owner. Cannot create widgets."));
        return;
    }

    // Iterate through all the created collision boxes.
    // 遍历所有已创建的碰撞框。
    for (UBoxComponent* BoxComponent : CreatedCollisionBoxes)
    {
        if (BoxComponent)
        {
            // Create a unique name for the widget component.
            // 为Widget组件创建一个唯一的名称。
            FName WidgetComponentName = MakeUniqueObjectName(OwnerActor, UWidgetComponent::StaticClass(), FName(TEXT("LockOnWidget")));

            // Create the Widget Component.
            // 创建Widget组件。
            UWidgetComponent* LockOnWidgetComponent = NewObject<UWidgetComponent>(OwnerActor, WidgetComponentName);

            if (LockOnWidgetComponent)
            {
                // Register the component.
                // 注册组件。
                LockOnWidgetComponent->RegisterComponent();

                // Set the widget class to use.
                // 设置要使用的Widget类。
                LockOnWidgetComponent->SetWidgetClass(LockOnWidgetClass);

                // Set the draw size if needed, otherwise it will use the widget's desired size.
                // 如果需要，设置绘制大小，否则它将使用Widget的期望大小。
                LockOnWidgetComponent->SetDrawSize(DefaultWidgetSize);

                // Set the widget space to Screen for it to be an overlay, or World for it to be in 3D space.
                // 将Widget空间设置为Screen，使其成为覆盖层；或设置为World，使其位于3D空间中。
                LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

                // Attach the widget component to the box component.
                // 将Widget组件附加到Box组件。
                // The EAttachmentRule::SnapToTarget ensures it snaps to the target location, rotation, and scale.
                // EAttachmentRule::SnapToTarget 确保它吸附到目标位置、旋转和缩放。
                LockOnWidgetComponent->AttachToComponent(BoxComponent, FAttachmentTransformRules::KeepRelativeTransform);
                // Or keep relative offset if you've set one:
                // LockOnWidgetComponent->AttachToComponent(BoxComponent, FAttachmentTransformRules::KeepRelativeTransform);

                LockOnWidgetComponent->SetVisibility(false);

                LockOnWidgetComponent->SetRelativeLocation(DefaultOffsetOfWidget);


                // You might want to store these widget components if you need to access them later.
                // 如果以后需要访问这些Widget组件，您可能需要存储它们。
                CreatedWidgets.Add(LockOnWidgetComponent);

                UE_LOG(LogTemp, Log, TEXT("Created and attached LockOnWidgetComponent to %s"), *BoxComponent->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create LockOnWidgetComponent for %s"), *BoxComponent->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Encountered a null UBoxComponent in CreatedCollisionBoxes."));
        }
    }
}

void ULockOnTargetComponent::BeginPlay()
{
    Super::BeginPlay();
}

void ULockOnTargetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    for (UBoxComponent* Box : CreatedCollisionBoxes)
    {
        if (Box)
        {
            Box->DestroyComponent();
        }
    }
    CreatedCollisionBoxes.Empty();
}

TArray<FVector> ULockOnTargetComponent::GetLockOnBoxWorldLocations() const
{
    TArray<FVector> Locations;
    for (const UBoxComponent* Box : CreatedCollisionBoxes)
    {
        if (Box)
        {
            Locations.Add(Box->GetComponentLocation());
        }
    }
    return Locations;
}

TArray<UWidgetComponent*> ULockOnTargetComponent::GetLockOnWidget() const
{
    return CreatedWidgets;
}

FVector ULockOnTargetComponent::GetClosestLockOnPointLocation(const FVector& FromLocation) const
{
    FVector ClosestPoint = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector; // Default to owner location
    float MinDistSq = BIG_NUMBER;

    for (const UBoxComponent* Box : CreatedCollisionBoxes)
    {
        if (Box)
        {
            const FVector PointLocation = Box->GetComponentLocation();
            const float DistSq = FVector::DistSquared(FromLocation, PointLocation);
            if (DistSq < MinDistSq)
            {
                MinDistSq = DistSq;
                ClosestPoint = PointLocation;
            }
        }
    }
    return ClosestPoint;
}

void ULockOnTargetComponent::ShowLockOnWidget()
{
    for (auto Element : CreatedWidgets)
    {
        Element->SetVisibility(true);
    }
}

void ULockOnTargetComponent::HideLockOnWidget()
{
    for (auto Element : CreatedWidgets)
    {
        Element->SetVisibility(false);
    }
}
