#include "GameBase//LsGameInstance.h"
#include "Engine/Engine.h"

ULsGameInstance::ULsGameInstance()
{
    // 构造函数
}

void ULsGameInstance::Init()
{
    Super::Init();

    // 游戏初始化时的逻辑
    OnGameInitialized();
}

void ULsGameInstance::Shutdown()
{
    // 游戏关闭时的逻辑
    OnGameShutdown();

    Super::Shutdown();
}

void ULsGameInstance::StartGameInstance()
{
    Super::StartGameInstance();

    // 游戏实例开始时的逻辑
}

FGameplayTagContainer& ULsGameInstance::GetGlobalGameTags() 
{
    return GlobalGameTags;
}

void ULsGameInstance::OnGameInitialized()
{
    // 在这里添加游戏初始化完成时的逻辑
    UE_LOG(LogTemp, Log, TEXT("Game Initialized"));
}

void ULsGameInstance::OnGameShutdown()
{
    // 在这里添加游戏关闭时的逻辑
    UE_LOG(LogTemp, Log, TEXT("Game Shutdown"));
} 