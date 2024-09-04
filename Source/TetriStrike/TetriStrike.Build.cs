// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TetriStrike : ModuleRules
{
	public TetriStrike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ProceduralMeshComponent", "UMG", "AdvancedWidgets", "AdvancedWidgets" });
	}
}
