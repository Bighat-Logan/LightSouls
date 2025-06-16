#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "Interface/UI/WidgetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "LsWidgetStackSubsystem.generated.h"

/**
 * 用于管理UI堆栈的子系统
 */
UCLASS()
class LIGHTSOULS_API ULsWidgetStackSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    ULsWidgetStackSubsystem();

    // 重写初始化函数
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // 重写关闭函数
    virtual void Deinitialize() override;

    // 存储注册的堆栈，用 Gameplay Tag 作为 Key
    UPROPERTY(Transient) // Transient so it doesn't get saved
    TMap<FGameplayTag, TScriptInterface<IWidgetManager>> RegisteredStacks;

    // 蓝图可调用的注册函数
    UFUNCTION(BlueprintCallable, Category = "Widget Stack Management")
    void RegisterWidgetStack(FGameplayTag LayerTag, TScriptInterface<IWidgetManager> WidgetManager);

    // 蓝图可调用的注销函数
    UFUNCTION(BlueprintCallable, Category = "Widget Stack Management")
    void UnregisterWidgetStack(TScriptInterface<IWidgetManager> WidgetStack);

    // 蓝图可调用的获取函数
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Widget Stack Management")
    TScriptInterface<IWidgetManager> GetWidgetStack(FGameplayTag LayerTag);
}; 