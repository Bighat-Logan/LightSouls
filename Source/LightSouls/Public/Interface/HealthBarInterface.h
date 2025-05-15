// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthBarInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHealthBarInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIGHTSOULS_API IHealthBarInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	* 一个可以在蓝图中实现并且可以从C++或蓝图调用的函数。
	* 如果C++类实现了这个接口，它必须提供一个名为 Interact_Implementation 的函数。
	* 如果你希望有一个默认的C++实现，你可以在实现类中提供。
	* 如果你希望这个函数可以被蓝图覆盖（即使有C++实现），使用 BlueprintNativeEvent。
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void ChangeHealth(float Percent); // 示例函数：交互

};
