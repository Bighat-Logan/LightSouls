// Fill out your copyright notice in the Description page of Project Settings.


#include "GasSystem/Tag/LsGameplayTag.h"

#include "GameplayTagsManager.h"


FLsGameplayTags FLsGameplayTags::GameplayTags;

void FLsGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Player_Action_State_OnLadder = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerActionState.OnLadder"),
		FString("OnLadder State")
		);

	GameplayTags.Player_Action_State_Climbing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerActionState.Climbing"),
		FString("Climbing State")
		);
	

	GameplayTags.Player_State_Running = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerState.Running"),
		FString("Running State")
		);

	GameplayTags.Player_State_LockingOnTarget = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerState.LockingOnTarget"),
		FString("LockingOnTarget State")
		);

	
	GameplayTags.Player_State_InOptionMenu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerState.InOptionMenu"),
		FString("InOptionMenu State")
		);

	GameplayTags.Player_State_InMainMenu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PlayerState.InMainMenu"),
		FString("InMainMenu State")
		);

	GameplayTags.PlayerInputState_BlockInput = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputState.BlockInput"),
		FString("Block input")
		);





	
	
	GameplayTags.Ability_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Ability.Melee"),
	FString("Block input")
	);

	GameplayTags.Ability_AISelectMeleeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Ability.AISelectMeleeAttack"),
	FString("AISelectMeleeAttack")
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


	GameplayTags.UI_WidgetStack_MainMenu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("UI.WidgetStack.MainMenu"),
		FString("UI MainMenu Widget Stack")
		);

	GameplayTags.UI_WidgetStack_InGameMenu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("UI.WidgetStack.InGameMenu"),
		FString("UI InGameMenu Widget Stack")
		);


	

}
