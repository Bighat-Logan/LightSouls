#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/LsUserWidget.h"
#include "Components/TextBlock.h"
#include "SmoothNumberWidget.generated.h"

UCLASS()
class LIGHTSOULS_API USmoothNumberWidget : public ULsUserWidget
{
    GENERATED_BODY()

public:
    /**
     * Call from Blueprint to start a smooth animation to change the value.
     * @param Amount The value to add (can be negative).
     */
    UFUNCTION(BlueprintCallable, Category = "Smooth Number")
    void AddValue(float Amount);

    /**
     * Call from Blueprint to set the value instantly without animation.
     * @param NewValue The new value to display.
     */
    UFUNCTION(BlueprintCallable, Category = "Smooth Number")
    void SetValueInstantly(float NewValue);

    /** Gets the current visually displayed value. This may be an intermediate value if an animation is in progress. */
    UFUNCTION(BlueprintPure, Category = "Smooth Number")
    float GetCurrentValue() const;

    /** The total duration of the animation in seconds. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smooth Number|Animation")
    float AnimationDuration = 1.0f;

    /** The time between each update tick of the animation. Smaller values are smoother but cost more. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smooth Number|Animation")
    float UpdateInterval = 0.02f;

protected:
    /** The TextBlock widget in UMG that displays the number. Its name in the UMG editor must be "ValueText". */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ValueText;

    virtual void NativeConstruct() override;

private:
    float CurrentValue;
    float StartValue;
    float TargetValue;
    float AnimationElapsedTime;

    FTimerHandle UpdateTimerHandle;

    void UpdateValueAnimation();
    void UpdateText();
}; 