using UnrealBuildTool;

public class EmergencyDeep : ModuleRules
{
	public EmergencyDeep(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"Slate",
			"SlateCore",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"RenderCore",
			"HeadMountedDisplay",
			"NavigationSystem",
			"Foliage",
			"Steamworks",
			"Sockets",
			"AIModule",
			"CableComponent"
		});

        PrivateDependencyModuleNames.AddRange(new string[]
        {
	        "UMG",
	        "Slate",
	        "SlateCore",
	        "OnlineSubsystem",
			"CableComponent"
		});
	}
}
