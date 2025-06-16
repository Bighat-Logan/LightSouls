#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CommonActivatableWidget.h"
#include "LsHealthBarActorComponent.generated.h"

class UWidgetComponent;
class UCommonTextBlock;
class UProgressBar;

/**
 * 血条组件类，用于在Actor上显示血条UI
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTSOULS_API ULsHealthBarActorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULsHealthBarActorComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** 更新血条显示 */
    UFUNCTION(BlueprintCallable, Category = "Health Bar")
    void UpdateHealthBar(float CurrentHealth, float MaxHealth);

    /** 显示血条 */
    UFUNCTION(BlueprintCallable, Category = "Health Bar")
    void ShowHealthBar();

    /** 隐藏血条 */
    UFUNCTION(BlueprintCallable, Category = "Health Bar")
    void HideHealthBar();

    /** 获取WidgetComponent */
    UFUNCTION(BlueprintCallable, Category = "Health Bar")
    UWidgetComponent* GetHealthBarWidgetComponent() const { return HealthBarWidgetComponent; }

protected:
    /** 血条UI类 */
    UPROPERTY(EditDefaultsOnly, Category = "Health Bar")
    TSubclassOf<UUserWidget> HealthBarWidgetClass;

    /** 血条Widget组件 */
    UPROPERTY()
    UWidgetComponent* HealthBarWidgetComponent;

    /** 血条进度条 */
    UPROPERTY()
    UProgressBar* HealthProgressBar;
    

    /** 血条显示时间 */
    UPROPERTY(EditDefaultsOnly, Category = "Health Bar")
    float DisplayDuration;

    /** 血条淡出时间 */
    UPROPERTY(EditDefaultsOnly, Category = "Health Bar")
    float FadeOutDuration;

    /** 血条显示计时器 */
    float DisplayTimer;

    /** 是否正在显示血条 */
    bool bIsHealthBarVisible;

    /** 血条绘制大小 */
    UPROPERTY(EditDefaultsOnly, Category = "Health Bar")
    FVector2D DrawSize;

    /** 是否按期望大小绘制 */
    UPROPERTY(EditDefaultsOnly, Category = "Health Bar")
    bool bDrawAtDesiredSize;

    /** 血条位置偏移 */
    UPROPERTY(EditDefaultsOnly, Category = "Health Bar")
    FVector LocationOffset;

    /** 血条缩放 */
    UPROPERTY(EditDefaultsOnly, Category = "Health Bar")
    FVector3d Scale;

    /** 初始化血条UI */
    void InitializeHealthBar();

    /** 更新血条位置 */
    void UpdateHealthBarPosition();
}; 