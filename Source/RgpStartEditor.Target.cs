// ANNN PRO

using UnrealBuildTool;
using System.Collections.Generic;

public class RgpStartEditorTarget : TargetRules
{
	public RgpStartEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "RgpStart" } );
	}
}
