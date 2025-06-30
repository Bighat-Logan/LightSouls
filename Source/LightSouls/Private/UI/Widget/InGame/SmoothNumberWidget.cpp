#include "UI/Widget/InGame/SmoothNumberWidget.h"
#include "TimerManager.h"

void USmoothNumberWidget::NativeConstruct()
{
    Super::NativeConstruct();
    SetValueInstantly(0.0f);
}

void USmoothNumberWidget::SetValueInstantly(float NewValue)
{
    GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
    CurrentValue = NewValue;
    TargetValue = NewValue;
    StartValue = NewValue;
    UpdateText();
}

void USmoothNumberWidget::AddValue(float Amount)
{
    if (Amount == 0.0f) return;

    StartValue = CurrentValue;
    TargetValue += Amount;
    AnimationElapsedTime = 0.0f;

    GetWorld()->GetTimerManager().SetTimer(
        UpdateTimerHandle,
        this,
        &USmoothNumberWidget::UpdateValueAnimation,
        UpdateInterval,
        true
    );
}

void USmoothNumberWidget::UpdateValueAnimation()
{
    AnimationElapsedTime += UpdateInterval;
    float Alpha = FMath::Clamp(AnimationElapsedTime / AnimationDuration, 0.0f, 1.0f);
    CurrentValue = FMath::Lerp(StartValue, TargetValue, Alpha);

    UpdateText();

    if (Alpha >= 1.0f)
    {
        GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
        // Ensure final value is accurate
        CurrentValue = TargetValue;
        UpdateText();
    }
}

void USmoothNumberWidget::UpdateText()
{
    if (ValueText)
    {
        // Format as an integer. Change formatting here if needed.
        FString DisplayString = FString::FromInt(FMath::RoundToInt(CurrentValue));
        ValueText->SetText(FText::FromString(DisplayString));
    }
}

float USmoothNumberWidget::GetCurrentValue() const
{
    return CurrentValue;
} 