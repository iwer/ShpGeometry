// Copyright (c) Iwer Petersen. All rights reserved.

#include "ShpFileAssetFactory.h"
#include "ShpFileAsset.h"
#include "GDALHelpers.h"
#include "GeoCoordinate.h"

UShpFileAssetFactory::UShpFileAssetFactory(const FObjectInitializer& ObjectInitializer)
{
    SupportedClass = UShpFileAsset::StaticClass();
    bCreateNew = false;
    bEditorImport = true;
    Formats.Add(TEXT("shp;ESRI Shapefile"));
}

UObject* UShpFileAssetFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
    UShpFileAsset * Asset = NewObject<UShpFileAsset>(InParent, InClass, InName, Flags);

    // open file
    const GDALDatasetRef Gdaldata = GDALHelpers::OpenVector(Filename, true);
    if(!Gdaldata){
        bOutOperationCanceled = true;
        return nullptr;
    }



    // get layers
    const int NumLayers = Gdaldata->GetLayerCount();
    for(int i = 0; i < NumLayers; ++i) {
        OGRLayer * Layer = Gdaldata->GetLayer(i);
        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: layer name %s"), ANSI_TO_TCHAR(Layer->GetName()));

        char * SrcWktStr = nullptr;
        Layer->GetSpatialRef()->exportToWkt(&SrcWktStr);
        FString SourceWkt = FString(SrcWktStr);
        FString TargetWkt = GDALHelpers::WktFromEPSG(UGeoCoordinate::EPSG_WGS84, false);
        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: src: %s tgt: %s"), *SourceWkt, *TargetWkt);
        OGRCoordinateTransformationRef CoordTransform = GDALHelpers::CreateCoordinateTransform(SourceWkt, TargetWkt);
        if(!CoordTransform.IsValid()) {
            UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: CoordTransform invalid."))
        }

        Layer->ResetReading();
        for(auto &Feature : Layer) {
            for( const auto& OField: *Feature )
            {
                switch( OField.GetType() )
                {
                    case OFTInteger:
                        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Field %s: %d,"), ANSI_TO_TCHAR(OField.GetName()) ,OField.GetInteger() );
                        break;
                    case OFTReal:
                        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Field %s: %.3f,"), ANSI_TO_TCHAR(OField.GetName()) ,OField.GetDouble() );
                        break;
                    case OFTString:
                        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Field %s: %s,"), ANSI_TO_TCHAR(OField.GetName()) ,OField.GetString() );
                        break;
                    default:
                        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Field %s: %s,"), ANSI_TO_TCHAR(OField.GetName()) ,OField.GetAsString() );
                        break;
                }
            }


            const int NumGeom = Feature->GetGeomFieldCount();
            for(int j = 0; j < NumGeom; ++j) {
                OGRGeometry* Geometry = Feature->GetGeomFieldRef(j);
                if(Geometry != nullptr) {
                    if(wkbFlatten(Geometry->getGeometryType()) == wkbPoint){
                        UE_LOG(LogTemp,Warning,TEXT("UShpFileAssetFactory: Point geometry"))
                    }
                    else if (wkbFlatten(Geometry->getGeometryType()) == wkbLineString) {
                        UE_LOG(LogTemp,Warning,TEXT("UShpFileAssetFactory: LineString geometry"))
                    }
                    else if (wkbFlatten(Geometry->getGeometryType()) == wkbPolygon) {
                        UE_LOG(LogTemp,Warning,TEXT("UShpFileAssetFactory: Polygon geometry"))
                        const auto Polygon = Geometry->toPolygon();

                        OGRLinearRing * Exterior = Polygon->getExteriorRing();

                        // fix order if necessary
                        if(!Exterior->isClockwise()) {
                            Exterior->reverseWindingOrder();
                        }

                        FShpPolygon ShpPolygon;

                        for(auto &Point : Exterior) {
                            UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Point X:%.3f Y:%.3f"), Point.getX(), Point.getY());
                            FVector In(Point.getX(), Point.getY(),0);
                            FVector Out;
                            if(GDALHelpers::TransformCoordinate(CoordTransform, In, Out)) {
                                UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Transformed Point X:%.3f Y:%.3f"), Out.X, Out.Y);
                                ShpPolygon.Vertices.Add(Out);
                            }
                        }

                        if(ShpPolygon.Vertices.Num() > 0)
                            Asset->Polygons.Add(ShpPolygon);
                    }
                    else if (wkbFlatten(Geometry->getGeometryType()) == wkbMultiPoint) {
                        UE_LOG(LogTemp,Warning,TEXT("UShpFileAssetFactory: MultiPoint geometry"))
                    }
                    else if (wkbFlatten(Geometry->getGeometryType()) == wkbMultiPolygon) {
                        UE_LOG(LogTemp,Warning,TEXT("UShpFileAssetFactory: MultiPolygon geometry"))
                    }
                }
            }

        }
    }


    return Asset;
}

bool UShpFileAssetFactory::FactoryCanImport(const FString & Filename)
{
    return true;
}
