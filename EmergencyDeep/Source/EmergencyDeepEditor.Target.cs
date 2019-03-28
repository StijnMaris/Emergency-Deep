using UnrealBuildTool;
using System.Collections.Generic;

public class EmergencyDeepEditorTarget : TargetRules
{
	public EmergencyDeepEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "EmergencyDeep" } );
	}
}
