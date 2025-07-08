// InteractableComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractableComponent.generated.h"

class UBoxComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTSOULS_API UInteractableComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UInteractableComponent();

    // 可以在蓝图中为每个实例单独设置的交互提示文本
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    FText InteractionPromptText;

    // 当交互发生时被调用的核心函数。
    // 设为virtual和BlueprintCallable，允许在C++子类或蓝图中被重写和调用。
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual void OnInteraction(AActor* Interactor);

protected:
    virtual void OnComponentCreated() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // 用于在蓝图中调整即将创建的碰撞体的大小
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    FVector InteractableVolumeSize = FVector(100.f);

    // 用于在蓝图中调整即将创建的碰撞体的相对位置
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    FVector InteractableVolumeLocation = FVector::ZeroVector;
    
    // 交互碰撞体 (运行时动态创建)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    UBoxComponent* InteractableVolume;
}; 