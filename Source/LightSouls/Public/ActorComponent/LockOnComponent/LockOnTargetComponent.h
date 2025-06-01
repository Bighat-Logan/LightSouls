// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnTargetComponent.generated.h"

USTRUCT(BlueprintType)
struct FLockOnPointInfo
{
	GENERATED_BODY()

	// 如果绑定到骨骼插槽，填写插槽名称；如果为 None，则使用相对位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOnPoint")
	FName SocketName;

	// 如果 SocketName 为 None，或者作为 Socket 的附加偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOnPoint")
	FVector RelativeOffset;

	// 该锁定点的碰撞盒大小 (半高宽)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOnPoint", Meta = (EditCondition = "SocketName == NAME_None"))
	FVector BoxExtent;

	FLockOnPointInfo()
		: SocketName(NAME_None)
		, RelativeOffset(FVector::ZeroVector)
		, BoxExtent(10.0f, 10.0f, 10.0f)
	{
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTSOULS_API ULockOnTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULockOnTargetComponent();

protected:
	
	virtual void OnComponentCreated() override
	{
		Super::OnComponentCreated();

		// 在属性初始化后创建碰撞盒
		CreateLockOnBoxes();
		CreateLockOnWidget();
	}

	// 创建锁定点碰撞盒
	void CreateLockOnBoxes();
	
	void CreateLockOnWidget();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// 可配置的锁定点列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn Settings")
	TArray<FLockOnPointInfo> LockOnPointsConfig;

	// 统一设置所有碰撞盒的大小 (半高宽)，如果 LockOnPointsConfig 中的 BoxExtent 未被特别指定 (或者用于骨骼附加点)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn Settings", meta = (ToolTip = "Default extent for collision boxes if not specified in LockOnPointsConfig, or for socket-attached points."))
	FVector DefaultBoxExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn Settings")
	FVector DefaultOffsetOfWidget = FVector(0,0,0);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn Settings")
	FVector2D DefaultWidgetSize = FVector2d(40.0f,40.0f);
	

	// 碰撞盒使用的碰撞通道 (应设置为你创建的 LockOn 通道)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn Settings")
	TEnumAsByte<ECollisionChannel> CollisionChannelToUse;

	// 调试：是否显示碰撞盒
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn Settings|Debug")
	bool bDrawDebugBoxes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn Settings")
	TSubclassOf<UUserWidget> LockOnWidgetClass;

private:
	// 存储动态创建的碰撞盒组件
	UPROPERTY()
	TArray<TObjectPtr<class UBoxComponent>> CreatedCollisionBoxes;
	
	UPROPERTY()
	TArray<TObjectPtr<class UWidgetComponent>> CreatedWidgets;

public:
	// 获取所有有效的锁定点世界坐标 (用于外部访问，例如 LockOnComponent)
	UFUNCTION(BlueprintCallable, Category = "LockOn Target")
	TArray<FVector> GetLockOnBoxWorldLocations() const;

	// 获取所有有效的锁定点世界坐标 (用于外部访问，例如 LockOnComponent)
	UFUNCTION(BlueprintCallable, Category = "LockOn Target")
	TArray<UWidgetComponent*> GetLockOnWidget() const;

	// 获取最接近给定位置的锁定点
	UFUNCTION(BlueprintCallable, Category = "LockOn Target")
	FVector GetClosestLockOnPointLocation(const FVector& FromLocation) const;

	// 获取最接近给定位置的锁定点
	UFUNCTION(BlueprintCallable, Category = "LockOn Target")
	void ShowLockOnWidget() ;

	UFUNCTION(BlueprintCallable, Category = "LockOn Target")
	void HideLockOnWidget() ;
};
