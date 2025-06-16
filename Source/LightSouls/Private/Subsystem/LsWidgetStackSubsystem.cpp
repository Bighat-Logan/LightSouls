#include "Subsystem/LsWidgetStackSubsystem.h"

ULsWidgetStackSubsystem::ULsWidgetStackSubsystem()
{
    // 构造函数
}

void ULsWidgetStackSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    RegisteredStacks.Empty();
    Super::Initialize(Collection);
}

void ULsWidgetStackSubsystem::Deinitialize()
{
    // 清理所有注册的堆栈
    RegisteredStacks.Empty();
    
    Super::Deinitialize();
}

void ULsWidgetStackSubsystem::RegisterWidgetStack(FGameplayTag LayerTag, TScriptInterface<IWidgetManager> WidgetManager)
{
    if (!WidgetManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempting to register null widget stack"));
        return;
    }

    if (LayerTag.IsValid())
    {
        RegisteredStacks.Add(LayerTag, WidgetManager);
        UE_LOG(LogTemp, Log, TEXT("Registered widget stack for layer: %s"), *LayerTag.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempting to register widget stack with invalid layer tag"));
    }
}

void ULsWidgetStackSubsystem::UnregisterWidgetStack(TScriptInterface<IWidgetManager> WidgetStack)
{
    if (!WidgetStack)
    {
        return;
    }

    // 查找并移除对应的堆栈
    for (auto It = RegisteredStacks.CreateIterator(); It; ++It)
    {
        if (It.Value() == WidgetStack)
        {
            UE_LOG(LogTemp, Log, TEXT("Unregistered widget stack for layer: %s"), *It.Key().ToString());
            It.RemoveCurrent();
            break;
        }
    }
}

TScriptInterface<IWidgetManager> ULsWidgetStackSubsystem::GetWidgetStack(FGameplayTag LayerTag)
{
    if (LayerTag.IsValid())
    {
        if (TScriptInterface<IWidgetManager>* FoundStack = RegisteredStacks.Find(LayerTag))
        {
            return *FoundStack;
        }
    }
    
    return nullptr;
} 