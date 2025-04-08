// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/LsAssetManager.h"

#include "GasSystem/Tag/LsGameplayTag.h"


ULsAssetManager& ULsAssetManager::Get()
{
	check(GEngine);
	
	ULsAssetManager* AuraAssetManager = Cast<ULsAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void ULsAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FLsGameplayTags::InitializeNativeGameplayTags();
	
}