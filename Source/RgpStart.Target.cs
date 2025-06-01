// ANNN PRO

using UnrealBuildTool;
using System.Collections.Generic;

public class RgpStartTarget : TargetRules
{
	public RgpStartTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "RgpStart" } );
	}
}
