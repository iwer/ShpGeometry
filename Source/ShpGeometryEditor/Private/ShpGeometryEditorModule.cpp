// Copyright (c) Iwer Petersen. All rights reserved.


#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "UnrealGDAL.h"

#include "AssetTools/ShpFileAssetActions.h"
#include "Styles/ShpFileAssetEditorStyle.h"

#define LOCTEXT_NAMESPACE "FShpGeometryEditorModule"


class FShpGeometryEditorModule : public IModuleInterface
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override
    {
        // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
        FUnrealGDALModule* UnrealGDAL = FModuleManager::Get().LoadModulePtr<FUnrealGDALModule>("UnrealGDAL");
        UnrealGDAL->InitGDAL();

        Style = MakeShareable(new FShpFileAssetEditorStyle());

        RegisterAssetTools();
    }

    virtual void ShutdownModule() override
    {
        UnregisterAssetTools();
    }

    virtual bool SupportsDynamicReloading() override
    {
        return true;
    }
protected:
    /** Registers asset tool actions. */
    void RegisterAssetTools()
    {
        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        TSharedPtr<IAssetTypeActions> ShpFileAction = MakeShareable(new FShpFileAssetActions(Style.ToSharedRef()));
        RegisterAssetTypeAction(AssetTools, ShpFileAction.ToSharedRef());

    }

    /**
     * Registers a single asset type action.
     *
     * @param AssetTools The asset tools object to register with.
     * @param Action The asset type action to register.
     */
    void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
    {
        AssetTools.RegisterAssetTypeActions(Action);
        RegisteredAssetTypeActions.Add(Action);
    }

    /** Unregisters asset tool actions. */
    void UnregisterAssetTools()
    {
        FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");

        if (AssetToolsModule)
        {
            IAssetTools& AssetTools = AssetToolsModule->Get();

            for (auto Action : RegisteredAssetTypeActions)
            {
                AssetTools.UnregisterAssetTypeActions(Action);
            }
        }
    }

private:
    /** The collection of registered asset type actions. */
    TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
    /** Holds the plug-ins style set. */
    TSharedPtr<ISlateStyle> Style;
};

IMPLEMENT_MODULE(FShpGeometryEditorModule, ShpGeometryEditor);
#undef LOCTEXT_NAMESPACE
