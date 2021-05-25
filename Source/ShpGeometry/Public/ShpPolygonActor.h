// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeoLocatedActor.h"
#include "ShpFileAsset.h"
#include "ProceduralMeshComponent.h"

#include "ShpPolygonActor.generated.h"

UCLASS()
class SHPGEOMETRY_API AShpPolygonActor : public AGeoLocatedActor
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UShpFileAsset * ShpFile;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UMaterialInterface * Material;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UProceduralMeshComponent *ProcMesh;

	// Sets default values for this actor's properties
	AShpPolygonActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void OnConstruction(const FTransform &Transform) override;
	
};
