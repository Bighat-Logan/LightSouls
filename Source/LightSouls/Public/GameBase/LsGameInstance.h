#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "LsGameInstance.generated.h"

/**
 * 游戏实例类，用于管理全局游戏状态
 */
UCLASS()
class LIGHTSOULS_API ULsGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    ULsGameInstance();

    // 重写初始化函数
    virtual void Init() override;

    // 重写关闭函数
    virtual void Shutdown() override;

    // 重写开始游戏函数
    virtual void StartGameInstance() override;
    
    // 全局游戏标签
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Tags")
    FGameplayTagContainer GlobalGameTags;

    UFUNCTION(BlueprintCallable, Category = "Game Tags")
	FGameplayTagContainer& GetGlobalGameTags();
    
  

protected:
    // 游戏初始化完成时的回调
    virtual void OnGameInitialized();

    // 游戏关闭时的回调
    virtual void OnGameShutdown();
}; 