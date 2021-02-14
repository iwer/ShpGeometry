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
    GDALDatasetRef gdaldata = GDALHelpers::OpenVector(Filename, true);
    if(!gdaldata){
        bOutOperationCanceled = true;
        return nullptr;
    }



    // get layers
    int numLayers = gdaldata->GetLayerCount();
    for(int i = 0; i < numLayers; ++i) {
        OGRLayer * layer = gdaldata->GetLayer(i);
        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: layer name %s"), ANSI_TO_TCHAR(layer->GetName()));

        char * srcWktStr = nullptr;
        layer->GetSpatialRef()->exportToWkt(&srcWktStr);
        FString sourceWkt = FString(srcWktStr);
        FString targetWkt = GDALHelpers::WktFromEPSG(UGeoCoordinate::EPSG_WGS84, false);
        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: src: %s tgt: %s"), *sourceWkt, *targetWkt);
        OGRCoordinateTransformationRef CoordTransform = GDALHelpers::CreateCoordinateTransform(sourceWkt, targetWkt);
        if(!CoordTransform.IsValid()) {
            UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: CoordTransform invalid."))
        }

        layer->ResetReading();
        for(auto &feature : layer) {
            for( const auto& oField: *feature )
            {
                switch( oField.GetType() )
                {
                    case OFTInteger:
                        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Field %s: %d,"), ANSI_TO_TCHAR(oField.GetName()) ,oField.GetInteger() );
                        break;
                    case OFTReal:
                        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Field %s: %.3f,"), ANSI_TO_TCHAR(oField.GetName()) ,oField.GetDouble() );
                        break;
                    case OFTString:
                        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Field %s: %s,"), ANSI_TO_TCHAR(oField.GetName()) ,oField.GetString() );
                        break;
                    default:
                        UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Field %s: %s,"), ANSI_TO_TCHAR(oField.GetName()) ,oField.GetAsString() );
                        break;
                }
            }


            OGRGeometry *geometry;
            int numGeom = feature->GetGeomFieldCount();
            for(int j = 0; j < numGeom; ++j) {
                geometry = feature->GetGeomFieldRef(j);
                if(geometry != NULL) {
                    if(wkbFlatten(geometry->getGeometryType()) == wkbPoint){
                        UE_LOG(LogTemp,Warning,TEXT("UShpFileAssetFactory: Point geometry"))
                    }
                    else if (wkbFlatten(geometry->getGeometryType()) == wkbLineString) {
                        UE_LOG(LogTemp,Warning,TEXT("UShpFileAssetFactory: LineString geometry"))
                    }
                    else if (wkbFlatten(geometry->getGeometryType()) == wkbPolygon) {
                        UE_LOG(LogTemp,Warning,TEXT("UShpFileAssetFactory: Polygon geometry"))
                        auto polygon = geometry->toPolygon();

                        OGRLinearRing * exterior = polygon->getExteriorRing();

                        // fix order if necessary
                        if(!exterior->isClockwise()) {
                            exterior->reverseWindingOrder();
                        }

                        FShpPolygon shpPolygon;

                        for(auto &point : exterior) {
                            UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Point X:%.3f Y:%.3f"), point.getX(), point.getY());
                            FVector in(point.getX(), point.getY(),0);
                            FVector out;
                            if(GDALHelpers::TransformCoordinate(CoordTransform, in, out)) {
                                UE_LOG(LogTemp, Warning, TEXT("UShpFileAssetFactory: Transformed Point X:%.3f Y:%.3f"), out.X, out.Y);
                                shpPolygon.Vertices.Add(out);
                            }
                        }

                        if(shpPolygon.Vertices.Num() > 0)
                            Asset->Polygons.Add(shpPolygon);
                    }
                    else if (wkbFlatten(geometry->getGeometryType()) == wkbMultiPoint) {
                        UE_LOG(LogTemp,Warning,TEXT("UShpFileAssetFactory: MultiPoint geometry"))
                    }
                    else if (wkbFlatten(geometry->getGeometryType()) == wkbMultiPolygon) {
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
