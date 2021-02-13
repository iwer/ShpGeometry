// Copyright (c) Iwer Petersen. All rights reserved.
#include "ShpFileAssetActions.h"


#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ShpFileAsset.h"
#include "Styling/SlateStyle.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FShpFileAssetActions::FShpFileAssetActions(const TSharedRef<ISlateStyle>& InStyle)
    : Style(InStyle)
{}

bool FShpFileAssetActions::CanFilter()
{
    return true;
}

void FShpFileAssetActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
    FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

    auto TextAssets = GetTypedWeakObjectPtrs<UShpFileAsset>(InObjects);

    MenuBuilder.AddMenuEntry(
        LOCTEXT("ShpFileAsset_DummyText", "Dummy Text"),
        LOCTEXT("ShpFileAsset_DummyTextToolTip", "Dummy menu entry."),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateLambda([=]{

            }),
            FCanExecuteAction::CreateLambda([=] {
                return false;
            })
        )
    );
}

uint32 FShpFileAssetActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}


FText FShpFileAssetActions::GetName() const
{
    return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ShpFileAsset", "ESRI Shape File Asset");
}


UClass* FShpFileAssetActions::GetSupportedClass() const
{
    return UShpFileAsset::StaticClass();
}


FColor FShpFileAssetActions::GetTypeColor() const
{
    return FColor::Blue;
}


bool FShpFileAssetActions::HasActions(const TArray<UObject*>& InObjects) const
{
    return true;
}



#undef LOCTEXT_NAMESPACE
