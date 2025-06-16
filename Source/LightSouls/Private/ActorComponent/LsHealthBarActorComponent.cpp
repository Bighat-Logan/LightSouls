#include "ActorComponent/LsHealthBarActorComponent.h"
#include "CommonActivatableWidget.h"
#include "CommonTextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"

ULsHealthBarActorComponent::ULsHealthBarActorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.1f; // 每0.1秒更新一次位置

    DisplayDuration = 3.0f;
    FadeOutDuration = 0.5f;
    DisplayTimer = 0.0f;
    bIsHealthBarVisible = false;

    // 默认值设置
    DrawSize = FVector2D(200.0f, 30.0f);
    bDrawAtDesiredSize = true;
    LocationOffset = FVector(0.0f, 0.0f, 100.0f);
    Scale = FVector3d(1.0f, 1.0f,1.0f);
    
}


void ULsHealthBarActorComponent::BeginPlay()
{
    Super::BeginPlay();

    InitializeHealthBar();
}

void ULsHealthBarActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsHealthBarVisible)
    {
        UpdateHealthBarPosition();

        // 更新显示计时器
        DisplayTimer += DeltaTime;
        if (DisplayTimer >= DisplayDuration)
        {
            HideHealthBar();
        }
    }
}

void ULsHealthBarActorComponent::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
    if (!HealthBarWidgetComponent )
    {
        return;
    }

    // 更新进度条
    float HealthPercent = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);



    
    
    ShowHealthBar();
}

void ULsHealthBarActorComponent::ShowHealthBar()
{
    if (!HealthBarWidgetComponent)
    {
        return;
    }

    bIsHealthBarVisible = true;
    DisplayTimer = 0.0f;
    HealthBarWidgetComponent->SetVisibility(true);
}

void ULsHealthBarActorComponent::HideHealthBar()
{
    if (!HealthBarWidgetComponent)
    {
        return;
    }

    bIsHealthBarVisible = false;
    HealthBarWidgetComponent->SetVisibility(false);
}

void ULsHealthBarActorComponent::InitializeHealthBar()
{
    if (!HealthBarWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthBarWidgetClass is not set"));
        return;
    }

    // 创建WidgetComponent
    HealthBarWidgetComponent = NewObject<UWidgetComponent>(GetOwner(), TEXT("HealthBarWidget"));
    if (!HealthBarWidgetComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create HealthBarWidgetComponent"));
        return;
    }

    // 设置WidgetComponent属性
    HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetClass);
    HealthBarWidgetComponent->SetDrawSize(DrawSize);
    HealthBarWidgetComponent->SetDrawAtDesiredSize(bDrawAtDesiredSize);
    
    HealthBarWidgetComponent->SetWorldScale3D(FVector(Scale.X, Scale.Y, Scale.Z));
    HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    HealthBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HealthBarWidgetComponent->SetVisibility(true);

    if (GetOwner() && GetOwner()->GetRootComponent())
    {
        HealthBarWidgetComponent->SetupAttachment(GetOwner()->GetRootComponent());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner or RootComponent is null, cannot attach HealthBarWidgetComponent."));
        return;
    }
    
    HealthBarWidgetComponent->SetRelativeLocation(LocationOffset);
    

    // 注册组件
    GetOwner()->AddInstanceComponent(HealthBarWidgetComponent);
    HealthBarWidgetComponent->RegisterComponent();

    // 获取UI组件引用
    UUserWidget* Widget = HealthBarWidgetComponent->GetWidget();
    if (Widget)
    {
        HealthProgressBar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("HealthProgressBar")));

        if (!HealthProgressBar )
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to get UI components"));
            return;
        }
    }
}

void ULsHealthBarActorComponent::UpdateHealthBarPosition()
{
    if (!HealthBarWidgetComponent || !GetOwner())
    {
        return;
    }


    // 2. 获取玩家摄像机管理器 (Get Player Camera Manager)
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0); // 'this' 通常是Actor本身，获取世界上下文
    if (!PlayerController)
    {
        // UE_LOG(LogTemp, Warning, TEXT("UpdateHealthBarRotation: PlayerController not found."));
        return;
    }

    APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;
    if (!PlayerCameraManager)
    {
        // UE_LOG(LogTemp, Warning, TEXT("UpdateHealthBarRotation: PlayerCameraManager not found."));
        return;
    }

    // 3. 获取摄像机的位置 (Get World Location of Camera's Root Component)
    // 蓝图中使用的是摄像机管理器的根组件位置。
    // PlayerCameraManager->GetCameraLocation() 是一个更直接获取摄像机实际位置的方法。
    FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

    // 4. 获取 Health Bar 的世界位置 (Get World Location of Health Bar)
    FVector HealthBarLocation = HealthBarWidgetComponent->GetComponentLocation();

    // 5. 计算朝向摄像机的旋转 (Find Look at Rotation)
    // Start 是 Health Bar 的位置，Target 是摄像机的位置。
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HealthBarLocation, CameraLocation);

    // 6. 设置 Health Bar 的世界旋转 (Set World Rotation)
    // 蓝图中的 Sweep 为 false，Teleport 为 true。
    // 在C++中，SetWorldRotation 的 bSweep 参数对应 Sweep，
    // ETeleportType 参数用于处理 Teleport 行为。
    bool bSweep = false;
    FHitResult SweepHitResult; // 如果 bSweep 为 true，则需要这个
    HealthBarWidgetComponent->SetWorldRotation(
        LookAtRotation,
        bSweep,
        bSweep ? &SweepHitResult : nullptr, // 仅当 bSweep 为 true 时传递 SweepHitResult 的地址
        ETeleportType::TeleportPhysics      // 对应蓝图中的 Teleport (勾选状态)
    );

} 