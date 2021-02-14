// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once
#include "CoreMinimal.h"

#include "ShpPolygon.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FShpPolygon {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FVector> Vertices;

    FShpPolygon() {}
};
