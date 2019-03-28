using UnrealBuildTool;
using System.Collections.Generic;

public class EmergencyDeepTarget : TargetRules
{
	public EmergencyDeepTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "EmergencyDeep" } );
	}
}
