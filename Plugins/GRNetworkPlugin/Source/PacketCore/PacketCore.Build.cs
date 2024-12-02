// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class PacketCore : ModuleRules
{
	public PacketCore(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Lib", "Win64", "libprotobuf.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Lib", "Win64", "PacketLibrary.lib"));

        PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1");
	}
}