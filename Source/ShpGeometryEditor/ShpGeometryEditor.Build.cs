// Copyright (c) Iwer Petersen. All rights reserved.

using UnrealBuildTool;

public class ShpGeometryEditor : ModuleRules
{
    public ShpGeometryEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                // ... add public include paths required here ...
            });


        PrivateIncludePaths.AddRange(
            new string[] {
                // ... add other private include paths required here ...
                "ShpGeometryEditor/Private",
                "ShpGeometryEditor/Private/Factories",
                "ShpGeometryEditor/Private/AssetTools",
                "ShpGeometryEditor/Private/Styles"
            });


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                // ... add private dependencies that you statically link with here ...
                "ContentBrowser",
                "Core",
                "CoreUObject",
                "DesktopWidgets",
                "EditorStyle",
                "Engine",
                "InputCore",
                "Projects",
                "RenderCore",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "GDAL",
                "UnrealGDAL",
                "GeoReference",
                "ShpGeometry"
            });


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
                "AssetTools",
                "MainFrame"
            });

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "AssetTools",
                "UnrealEd"
            });
    }
}
