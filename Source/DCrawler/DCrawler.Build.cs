// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DCrawler : ModuleRules
{
	public DCrawler(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
