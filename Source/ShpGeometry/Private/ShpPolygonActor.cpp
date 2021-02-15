// Fill out your copyright notice in the Description page of Project Settings.


#include "ShpPolygonActor.h"
#include "PolygonHelper.h"

// Sets default values
AShpPolygonActor::AShpPolygonActor()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
    ProcMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AShpPolygonActor::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AShpPolygonActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AShpPolygonActor::OnConstruction(const FTransform &Transform)
{
    if(GeoRef) {
        Longitude = GeoRef->Longitude;
        Latitude = GeoRef->Latitude;
    }
    Super::OnConstruction(Transform);

    if(ShpFile) {
        ProcMesh->ClearAllMeshSections();

        int polyIdx = 0;
        for (auto &polygon : ShpFile->Polygons) {

            float minZ = std::numeric_limits<float>::max();
            // transform vertices to game coordinates
            TArray < FVector > gameVerts;
            for (auto &v : polygon.Vertices) {
                FVector gv = GeoRef->ToGameCoordinate(FVector(v.X, v.Y, GetActorLocation().Z));


                // make relative to origin
                gv -= GetActorLocation();
                //gv.Z = GetActorLocation().Z;
                gameVerts.Add(gv);

                FVector groundVert = SnapToGround(gv,100);
                minZ = std::min(groundVert.Z-GetActorLocation().Z, minZ);
            }

            // set to min height
//            for(auto &v : gameVerts) {
//                v.Z = minZ;
//            }

            // earcut
            TArray < int32 > indices = PolygonHelper::TesselatePolygon(gameVerts);

            // construct buffers
            TArray <int32> triIndices;
            TArray <FVector> normals;
            TArray<FProcMeshTangent> tangents;
            TArray<FVector2D> texCoords;
            TArray<FLinearColor> colors;

            TArray<FVector> normTriplet;
            normTriplet.Init(FVector(0,0,1),3);
            TArray<FProcMeshTangent> tangTriplet;
            tangTriplet.Init(FProcMeshTangent(1,0,0),3);
            TArray<FLinearColor> colTriplet;
            colTriplet.Init(FLinearColor(0,0,0,1),3);

            int i = 0;
            while((i+3) <= indices.Num()) {
                triIndices.Add(indices[i+2]);
                triIndices.Add(indices[i+1]);
                triIndices.Add(indices[i]);
                normals.Append(normTriplet);
                tangents.Append(tangTriplet);
                colors.Append(colTriplet);
                i+=3;
            }

            texCoords = PolygonHelper::FlatUVMap(gameVerts);

            // add meshsection
            ProcMesh->CreateMeshSection_LinearColor(polyIdx, gameVerts, triIndices, normals, texCoords, colors, tangents, false);

            if(Material)
                ProcMesh->SetMaterial(polyIdx, Material);
            polyIdx++;
        }
    }
}
