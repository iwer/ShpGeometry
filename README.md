# ShpGeometry - Unreal Engine Plugin

This plugins is intended to import geometry from ESRI shape files. This is a very early version that imports only polygon shapes into a ShpFileAsset, that can be used to generate ProceduralMeshComponents.

## Compatibility

* Unreal Engine 4 is supported at least with 4.27 (branch ue4, tag ue4.27)
* Unreal Engine 5 is supported up to 5.1 (on master branch)


## Building

This plugin uses [GeoReference](https://github.com/iwer/GeoReference), [SpatialGeometryTools](https://github.com/iwer/SpatialGeometryTools), [UnrealGDAL](https://github.com/TensorWorks/UnrealGDAL), [ue4cli](https://github.com/adamrehn/ue4cli) and [conan-ue4cli](https://github.com/adamrehn/conan-ue4cli) to build and manage the required GDAL libraries.

* Download a Release of UnrealGDAL and place it in the Plugins directory or clone and follow instructions to build UnrealGDAL from source.

## Usage

Add ShpGeometry to `.uproject` or `.uplugin`

```json
"Plugins": [
  {
    "Name": "ShpGeometry",
    "Enabled": true
  }
]
```

Add build dependencies to your modules

```csharp
PublicDependencyModuleNames.AddRange(
  new string[]
  {
    "ShpGeometry",
    "ShpGeometryEditor"
  }
);
```

## Funding
Development of this code was partially funded by the German Federal Ministry of Education and Research (BMBF) under funding reference number 13FH1I01IA.
