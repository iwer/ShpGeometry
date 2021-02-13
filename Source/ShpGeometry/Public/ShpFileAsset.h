// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "ShpFileAsset.generated.h"
/**
 *  https://gdal.org/tutorials/vector_api_tut.html
 *  https://www.esri.com/library/whitepapers/pdfs/shapefile.pdf
 */
UCLASS(BlueprintType, hidecategories=(Object))
class SHPGEOMETRY_API UShpFileAsset
    : public UDataAsset
{
    GENERATED_BODY()
public:

};
