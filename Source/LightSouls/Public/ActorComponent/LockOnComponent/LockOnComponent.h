// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LockOnComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetLocked, AActor*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetUnlocked);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTSOULS_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    ULockOnComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY()
    APlayerController* PlayerController;

    UPROPERTY()
    APawn* OwnerPawn;

    UPROPERTY()
    UCharacterMovementComponent* CharacterMovement;

public:
    // Enhanced Input Action 用于触发锁定
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LockOn|Input")
    TObjectPtr<UInputAction> LockOnAction;

    // 锁定检测的距离
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Detection")
    float LockOnDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|State")
    float LostLockOnDistance;

    // 锁定检测的扇形（锥形）的半角 (角度制)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Detection")
    float LockOnConeHalfAngle;

    // 锁定检测使用的碰撞通道 (应设置为你创建的 LockOn 通道)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Detection")
    TEnumAsByte<ECollisionChannel> TraceChannelToUse;

    // 角色和镜头转向目标的速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Behavior")
    float TurnToTargetSpeed;

    // 当目标距离小于此值时，摄像机不再调整俯仰角
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Behavior")
    float CameraPitchLockDistance;

    // 当前锁定的目标Actor
    UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "LockOn|State")
    TObjectPtr<AActor> CurrentLockedTarget;

    // 用于选择目标时，视野中心点筛选的权重 (0-1, 0表示纯距离最近，1表示纯角度最近)
    // 但这里我们直接用角度最小的。如果需要屏幕中心，逻辑会更复杂。
    // 这里我们选择与角色正前方夹角最小的目标。

    UFUNCTION(BlueprintCallable, Category = "LockOn")
    void ToggleLockOn();

    UFUNCTION(BlueprintCallable, Category = "LockOn")
    bool IsTargetLocked() const { return CurrentLockedTarget != nullptr; }


    UFUNCTION(BlueprintCallable, Category = "LockOn")
    void SwitchLockOnTarget(FVector2D LookAxisVector);


protected:
    // 绑定输入
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

    // 处理输入事件
    void HandleLockOnAction(const FInputActionValue& Value);

    // 查找最佳目标
    AActor* FindBestTarget() const;

    // 设置或清除锁定目标
    void SetLockedTarget(AActor* NewTarget);

    // 锁定新目标
    void LockNewTarget(AActor* NewTarget);

    // 解除当前目标的锁定
    void UnlockCurrentTarget();

    // 转向当前目标
    void TurnTowardsTarget(float DeltaTime);

    // 蓝图可实现的事件，用于触发视觉效果
    UFUNCTION(BlueprintImplementableEvent, Category = "LockOn|Visuals")
    void OnTargetLocked(AActor* LockedActor);

    UFUNCTION(BlueprintImplementableEvent, Category = "LockOn|Visuals")
    void OnTargetUnlocked();

    // 调试：是否显示检测锥形
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Debug")
    bool bDrawDebugCone;

private:
    // 查找指定方向最近的目标
    AActor* FindNearestTargetInDirection(const FVector& Direction) const;
    
};
