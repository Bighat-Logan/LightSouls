// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LightSouls : ModuleRules
{
	public LightSouls(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"EnhancedInput", 
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks", 
				"CommonUI"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Slate",
				"SlateCore",
				"InputCore",
				"AIModule", 
				"EnhancedInput",
				"GameplayTags",
				"GameplayAbilities",
				"CommonUI"
			}
		);
	}
}
