using UnrealBuildTool;

public class Mortar : ModuleRules
{
	public Mortar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject",
			"Engine",
			"InputCore" 
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"UMG", "Slate", "SlateCore"
		});
	}
}
