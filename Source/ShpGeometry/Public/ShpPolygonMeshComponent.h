// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ShpTypes/ShpPolygon.h"

#include "ShpPolygonMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHPGEOMETRY_API UShpPolygonMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FShpPolygon Polygon;
	
	virtual void OnComponentCreated() override;
};
