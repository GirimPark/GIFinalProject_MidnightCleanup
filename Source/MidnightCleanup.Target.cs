// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MidnightCleanupTarget : TargetRules
{
	public MidnightCleanupTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("MidnightCleanup");
        //BuildEnvironment = TargetBuildEnvironment.Unique;
        //bUseLoggingInShipping = true;
	}
}
