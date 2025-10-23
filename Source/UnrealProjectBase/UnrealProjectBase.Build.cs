// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealProjectBase : ModuleRules
{
	public UnrealProjectBase(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"AIModule", 
			"GameplayTasks",
			"GameplayTags",
			"NavigationSystem",
			"FMODStudio",
			"Niagara",
			"UMG",
			"Slate",
			"SlateCore",
			"MoviePlayer",
			"DeveloperSettings",
			"NiagaraFluids",
			"Chaos",
			"ChaosSolverEngine",
			"GeometryCollectionEngine"
		});
	}
}
