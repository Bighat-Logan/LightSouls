// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/LockOnComponent/LockOnComponent.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "ActorComponent/LockOnComponent/LockOnTargetComponent.h"
#include "GameFramework/Character.h"
#include "GasSystem/Tag/LsGameplayTag.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


ULockOnComponent::ULockOnComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // 需要Tick来平滑转向
    LockOnDistance = 1000.0f;
    LockOnConeHalfAngle = 30.0f;
    TurnToTargetSpeed = 8.0f;
    LostLockOnDistance = 2500.0f;
    CurrentLockedTarget = nullptr;
    bDrawDebugCone = false;
    // **重要**: 在这里设置默认的碰撞通道，或者确保在蓝图中正确设置
    // 例如: TraceChannelToUse = ECC_GameTraceChannel1; // 替换为你的LockOn通道
}

void ULockOnComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn)
    {
        PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
        
        // 获取角色移动组件
        if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerPawn))
        {
            CharacterMovement = OwnerCharacter->GetCharacterMovement();
        }
    }

    // 确保输入已绑定
    if (OwnerPawn && OwnerPawn->IsLocallyControlled())
    {
       if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(OwnerPawn->InputComponent))
       {
           SetupPlayerInputComponent(EIC);
       }
    }
}

void ULockOnComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (LockOnAction)
        {
            EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ULockOnComponent::HandleLockOnAction);
        }
    }
}



void ULockOnComponent::HandleLockOnAction(const FInputActionValue& Value)
{
    ToggleLockOn();
}

void ULockOnComponent::ToggleLockOn()
{
    if (CurrentLockedTarget)
    {
        SetLockedTarget(nullptr); // 解除锁定
    }
    else
    {
        AActor* BestTarget = FindBestTarget();
        SetLockedTarget(BestTarget);
    }
}

AActor* ULockOnComponent::FindBestTarget() const
{
    if (!OwnerPawn || !PlayerController) return nullptr;

    FVector TraceStart = OwnerPawn->GetActorLocation();
    // 如果Pawn有摄像头组件，使用摄像头朝向；否则用Pawn的朝向。
    // 通常用摄像头的朝向更好。
    FVector TraceDirection = PlayerController->GetControlRotation().Vector(); // 使用镜头的方向
    if (APlayerCameraManager* CamManager = PlayerController->PlayerCameraManager)
    {
        TraceStart = CamManager->GetCameraLocation();
        TraceDirection = CamManager->GetCameraRotation().Vector();
        
    }
    
    FVector TraceEnd = TraceStart + TraceDirection * LockOnDistance;

    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerPawn);

    UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(),
        TraceStart,
        TraceEnd,
        LockOnDistance,
        UEngineTypes::ConvertToTraceType(TraceChannelToUse),
        false, // bTraceComplex
        ActorsToIgnore,
        bDrawDebugCone ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
        HitResults,
        true, // bIgnoreSelf
        FLinearColor::Red,
        FLinearColor::Green,
        2.0f
    );
    

    AActor* BestTarget = nullptr;
    float MinAngleToCenter = LockOnConeHalfAngle + 1.0f; // 初始化为一个比最大可能角度稍大的值

    //如果使用屏幕中心距离，则需要以下：
    FVector2D ViewportCenter;
    if (GEngine && GEngine->GameViewport)
    {
       FVector2D ViewportSize;
       GEngine->GameViewport->GetViewportSize(ViewportSize);
       ViewportCenter = ViewportSize * 0.5f;
    }
    float MinScreenDistanceSq = BIG_NUMBER;


    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor->FindComponentByClass<ULockOnTargetComponent>())
        {
            FVector DirectionToHitActor = (HitActor->GetActorLocation() - TraceStart).GetSafeNormal();
            float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TraceDirection, DirectionToHitActor)));

            if (AngleDegrees < MinAngleToCenter) // 选择与玩家/镜头正前方夹角最小的目标
            {
                MinAngleToCenter = AngleDegrees;
                BestTarget = HitActor;
            }
            
        }
    }
    return BestTarget;
}

void ULockOnComponent::SetLockedTarget(AActor* NewTarget)
{
    if (CurrentLockedTarget != NewTarget)
    {
        if (NewTarget)
        {
            UnlockCurrentTarget();
            LockNewTarget(NewTarget);
        }
        else
        {
            UnlockCurrentTarget();
        }
    }
}

void ULockOnComponent::LockNewTarget(AActor* NewTarget)
{
    if (!NewTarget) return;

    CurrentLockedTarget = NewTarget;
    
    // 显示锁定UI
    ULockOnTargetComponent* LockOnTargetComponent = CurrentLockedTarget->FindComponentByClass<ULockOnTargetComponent>();
    if (LockOnTargetComponent)
    {
        LockOnTargetComponent->ShowLockOnWidget();
    }

    // 禁用自动朝向移动方向
    if (CharacterMovement)
    {
        CharacterMovement->bOrientRotationToMovement = false;
    }

    // 添加锁定标签
    if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
    {
        ASC->AddLooseGameplayTag(FLsGameplayTags::Get().Player_State_LockingOnTarget);
    }

    // 触发蓝图事件
    OnTargetLocked(CurrentLockedTarget);
}

void ULockOnComponent::UnlockCurrentTarget()
{
    if (!CurrentLockedTarget) return;

    // 隐藏锁定UI
    ULockOnTargetComponent* LockOnTargetComponent = CurrentLockedTarget->FindComponentByClass<ULockOnTargetComponent>();
    if (LockOnTargetComponent)
    {
        LockOnTargetComponent->HideLockOnWidget();
    }
    
    // 启用自动朝向移动方向
    if (CharacterMovement)
    {
        CharacterMovement->bOrientRotationToMovement = true;
    }
    
    // 移除锁定标签
    if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
    {
        ASC->RemoveLooseGameplayTag(FLsGameplayTags::Get().Player_State_LockingOnTarget);
    }

    // 触发蓝图事件
    OnTargetUnlocked();
    
    // 清除当前目标
    CurrentLockedTarget = nullptr;
}

void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CurrentLockedTarget && OwnerPawn && PlayerController)
    {
        // 验证目标是否仍然有效
        if (CurrentLockedTarget->IsPendingKillPending() || !CurrentLockedTarget->FindComponentByClass<ULockOnTargetComponent>())
        {
            SetLockedTarget(nullptr);
            return;
        }

        // 检查距离和角度是否仍在锁定范围内 (可选，看设计需求)
        FVector StartCheckLocation = PlayerController->PlayerCameraManager ? PlayerController->PlayerCameraManager->GetCameraLocation() : OwnerPawn->GetActorLocation();
        FVector DirToTarget = (CurrentLockedTarget->GetActorLocation() - StartCheckLocation);
        float DistToTargetSq = DirToTarget.SizeSquared();

        if (DistToTargetSq > FMath::Square(LostLockOnDistance * 1.1f)) // 增加一个小的容差避免频繁解除
        {
            SetLockedTarget(nullptr);
            return;
        }
        
        // 角度检查 (可选)
        FVector ForwardCheckDir = PlayerController->GetControlRotation().Vector();
        DirToTarget.Normalize();
        float AngleToTargetRad = FMath::Acos(FVector::DotProduct(ForwardCheckDir, DirToTarget));
        if (FMath::RadiansToDegrees(AngleToTargetRad) > LockOnConeHalfAngle * 1.2f) // 1.2f 是容差
        {
            // SetLockedTarget(nullptr); // 如果超出扇区则解除锁定
            // return;
        }
        
        TurnTowardsTarget(DeltaTime);
    }
}

void ULockOnComponent::TurnTowardsTarget(float DeltaTime)
{
    if (!CurrentLockedTarget || !OwnerPawn || !PlayerController) return;

    // 获取目标身上最近的锁定点 (如果目标有 ULockOnTargetComponent)
    FVector TargetLocation = CurrentLockedTarget->GetActorLocation();
    ULockOnTargetComponent* TargetComp = CurrentLockedTarget->FindComponentByClass<ULockOnTargetComponent>();
    if (TargetComp)
    {
        TargetLocation = TargetComp->GetClosestLockOnPointLocation(OwnerPawn->GetActorLocation());
    }

    // 1. 调整角色朝向 (通常只调整Yaw)
    FGameplayTagContainer GameplayTagContainer;
    if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
    {
        ASC->GetOwnedGameplayTags(GameplayTagContainer);
    }
    if (!GameplayTagContainer.HasTag(FLsGameplayTags::Get().Player_State_Running))
    {
        FRotator OwnerRotation = OwnerPawn->GetActorRotation();
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerPawn->GetActorLocation(), TargetLocation);
        // 只影响Yaw，Pitch和Roll保持角色自身
        FRotator TargetPawnRotation = FRotator(OwnerRotation.Pitch, LookAtRotation.Yaw, OwnerRotation.Roll);
        OwnerPawn->SetActorRotation(FMath::RInterpTo(OwnerRotation, TargetPawnRotation, DeltaTime, TurnToTargetSpeed));
    }
    
    // 2. 调整摄像机朝向 (影响ControlRotation)
    FRotator ControlRotation = PlayerController->GetControlRotation();
    
    FVector CameraTargetLocation = (TargetLocation + OwnerPawn->GetActorLocation()) / 2.0f;
    // 摄像机的起始点一般是摄像机的位置，而不是Pawn的位置
    FVector CameraLocation = PlayerController->PlayerCameraManager ? PlayerController->PlayerCameraManager->GetCameraLocation() : OwnerPawn->GetActorLocation();
    FRotator TargetControlRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, CameraTargetLocation);
    
    PlayerController->SetControlRotation(FMath::RInterpTo(ControlRotation, TargetControlRotation, DeltaTime, TurnToTargetSpeed));
}

void ULockOnComponent::SwitchLockOnTarget(FVector2D LookAxisVector)
{
    
    // 根据CurrentLockedTarget在屏幕中的位置和LookAxisVector构建一个向量SwitchDirection（CurrentLockedTarget为原点，LookAxisVector为方向）
    
    // 在HitResults中遍历所有碰撞结果，将其投射到屏幕空间，获取其二维坐标ScreenLocation，用CurrentLockedTarget的屏幕坐标和ScreenLocation构建一个向量ActorDirection（从CurrentLockedTarget指向ScreenLocation）
    // 计算ActorDirection与SwitchDirection的夹角，将所有小于 夹角宽容度(在头文件中定义，蓝图可修改)的碰撞结果加入结果集
    // 计算结果集中所有Actor到CurrentLockedTarget的距离，选择距离最短的一个actor  调用SetLockedTarget(Actor*)


    if (!CurrentLockedTarget || !OwnerPawn || !PlayerController)
    {
        return;
    }

    // 获取当前锁定目标的屏幕坐标
    FVector2D CurrentTargetScreenLocation;
    if (!UGameplayStatics::ProjectWorldToScreen(PlayerController, CurrentLockedTarget->GetActorLocation(), CurrentTargetScreenLocation))
    {
        return;
    }
    
    FVector TraceStart = OwnerPawn->GetActorLocation();
    
    FVector TraceDirection = PlayerController->GetControlRotation().Vector(); 
    if (APlayerCameraManager* CamManager = PlayerController->PlayerCameraManager)
    {
        TraceStart = CamManager->GetCameraLocation();
        TraceDirection = CamManager->GetCameraRotation().Vector();
    }
    
    FVector TraceEnd = TraceStart + TraceDirection * LockOnDistance;

    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerPawn);
    ActorsToIgnore.Add(CurrentLockedTarget);

    UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(),
        TraceStart,
        TraceEnd,
        LockOnDistance,
        UEngineTypes::ConvertToTraceType(TraceChannelToUse),
        false, // bTraceComplex
        ActorsToIgnore,
        bDrawDebugCone ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
        HitResults,
        true, // bIgnoreSelf
        FLinearColor::Red,
        FLinearColor::Green,
        2.0f
    );

    // 遍历所有碰撞结果，找出与输入方向夹角最小的目标
    float MinAngle = 180.f;
    float MinScreenDist = BIG_NUMBER;
    AActor* BestTarget = nullptr;

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor || !HitActor->FindComponentByClass<ULockOnTargetComponent>())
            continue;

        // 投射到屏幕空间
        FVector2D HitScreenLocation;
        if (!UGameplayStatics::ProjectWorldToScreen(PlayerController, HitActor->GetActorLocation(), HitScreenLocation))
            continue;

        // 构建ActorDirection（从当前锁定目标指向候选目标）
        FVector2D ActorDirection = (HitScreenLocation - CurrentTargetScreenLocation).GetSafeNormal();

        // 计算与输入方向的夹角
        float Angle = FMath::Acos(FVector2D::DotProduct(ActorDirection, LookAxisVector));
        Angle = FMath::RadiansToDegrees(Angle);

        // 可选：设置一个最大夹角容忍度，比如60度
        float MaxAngleTolerance = 60.f;
        if (Angle < MaxAngleTolerance)
        {
            // 夹角最小优先，如果夹角相同则选屏幕距离最近的
            float ScreenDist = FVector2D::DistSquared(HitScreenLocation, CurrentTargetScreenLocation);
            if (Angle < MinAngle || (FMath::IsNearlyEqual(Angle, MinAngle) && ScreenDist < MinScreenDist))
            {
                MinAngle = Angle;
                MinScreenDist = ScreenDist;
                BestTarget = HitActor;
            }
        }
    }

    if (BestTarget)
    {
        SetLockedTarget(BestTarget);
    }
}



