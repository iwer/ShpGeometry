// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "UnrealGDAL.h"

class FShpGeometryEditorModule : public IModuleInterface
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override
    {
        // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
        FUnrealGDALModule* UnrealGDAL = FModuleManager::Get().LoadModulePtr<FUnrealGDALModule>("UnrealGDAL");
        UnrealGDAL->InitGDAL();
    }

    virtual void ShutdownModule() override
    {

    }
};
