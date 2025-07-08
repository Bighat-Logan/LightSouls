// InteractorComponent.h

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

class UBoxComponent;
class UUserWidget;
class ICanInteract; // 前向声明接口

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTSOULS_API UInteractorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractorComponent();
    
    // 公开函数，用于从玩家角色蓝图或C++中调用，以绑定输入
    void SetupPlayerInput(UInputComponent* PlayerInputComponent);

protected:
    virtual void OnComponentCreated() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY()
    APawn* OwnerPawn;

    // 用于在蓝图中调整即将创建的碰撞体的大小
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    FVector InteractionVolumeSize = FVector(100.f);

    // 用于在蓝图中调整即将创建的碰撞体的相对位置
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    FVector InteractionVolumeLocation = FVector::ZeroVector;

    /** Jump Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* InteractAction;
    
    // 组件的核心检测体积 (运行时动态创建)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    UBoxComponent* InteractionVolume;

    // 可在蓝图中设置的交互提示UI类
    UPROPERTY(EditDefaultsOnly, Category = "Interaction|UI")
    TSubclassOf<UUserWidget> InteractionPromptWidgetClass;

    // 保存当前交互提示UI的实例
    UPROPERTY()
    UUserWidget* InteractionPromptWidgetInstance;

    // 当前检测到的可交互对象。使用弱指针防止对象销毁后指针悬空。
    UPROPERTY()
    TWeakObjectPtr<AActor> CurrentInteractableTarget;

    // 当输入按键被按下时调用的函数
    UFUNCTION()
    void OnInteractPressed();

    // 绑定到InteractionVolume的重叠事件
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
}; 