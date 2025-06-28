// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
#include "LsGameplayTag.generated.h"


struct FLsGameplayTags
{

public:
	static const FLsGameplayTags& Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();


	FGameplayTag UI_WidgetStack_MainMenu;
	FGameplayTag UI_WidgetStack_InGameMenu;


	FGameplayTag Player_State_InOptionMenu;
	FGameplayTag Player_State_InMainMenu;


	FGameplayTag Player_State_Running;
	FGameplayTag Player_State_LockingOnTarget;
	FGameplayTag Player_State_Stunned;
	FGameplayTag Player_State_Rolling;
	
	FGameplayTag Player_State_Passive_StaminaRegenPaused;
	
	
	FGameplayTag PlayerInputState_BlockInput;


	FGameplayTag Ability_Melee;
	FGameplayTag Ability_AISelectMeleeAttack;

	FGameplayTag Ability_Passive_StaminaRegen;

	FGameplayTag Event_Montage_Shared_WeaponHit;

	FGameplayTag Player_Action_State;

	FGameplayTag Player_Action_State_Entry;

	FGameplayTag Player_Action_State_OnLadder;
	FGameplayTag Player_Action_State_Climbing;

	FGameplayTag Player_Action_State_Sword_Combo_1;
	FGameplayTag Player_Action_State_Sword_Combo_2;
	FGameplayTag Player_Action_State_Sword_Combo_3;

private:
	static FLsGameplayTags GameplayTags;
};


/**
 * 
 */
UCLASS()
class LIGHTSOULS_API ULsGameplayTag : public UObject
{
	GENERATED_BODY()
	
};
