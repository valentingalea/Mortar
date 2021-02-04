using UnrealBuildTool;
using System.Collections.Generic;

public class MortarEditorTarget : TargetRules
{
	public MortarEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Mortar" } );
	}
}
