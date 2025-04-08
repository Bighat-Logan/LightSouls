// Fill out your copyright notice in the Description page of Project Settings.


#include "GasSystem/Tag/LsGameplayTag.h"

#include "GameplayTagsManager.h"


FLsGameplayTags FLsGameplayTags::GameplayTags;

void FLsGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.PlayerState_OnLadder = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerState.OnLadder"),
		FString("OnLadder State")
		);

	GameplayTags.PlayerState_Climbing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerState.Climbing"),
		FString("Climbing State")
		);

	GameplayTags.PlayerInputState_BlockInput = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputState.BlockInput"),
		FString("Block input")
		);

	
	GameplayTags.Ability_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Ability.Melee"),
	FString("Block input")
	);

	GameplayTags.Event_Montage_Shared_WeaponHit = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Event.Montage.Shared.WeaponHit"),
	FString("Block input")
	);


	GameplayTags.Player_Action_State = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerActionState"),
		FString("Player Action State")
		);


	GameplayTags.Player_Action_State_Entry = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerActionState.Entry"),
		FString("Player Action State Entry")
		);
	

	GameplayTags.Player_Action_State_Sword_Combo_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerActionState.Sword_Combo_1"),
		FString("Player Action State Sword Combo_1")
		);
	
	GameplayTags.Player_Action_State_Sword_Combo_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerActionState.Sword_Combo_2"),
		FString("Player Action State Sword Combo_2")
		);

	GameplayTags.Player_Action_State_Sword_Combo_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerActionState.Sword_Combo_3"),
		FString("Player Action State Sword Combo_3")
		);


	

}
