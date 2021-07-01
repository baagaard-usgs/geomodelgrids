# geomodelgrids_isosurface

:::{note}
This application requires the GDAL library and is not included in the binary package.
:::

The `geomodelgrids_isosurface` command line program is used to generate horizontal isosurfaces for model values.
The isosurface values are the depth from the reference surface; the default surface is `topography_bathymetry`.
Currently, only output as GeoTiff raster image files is supported.
The isosurface values are computed at the center of each pixel in the raster image.

The isosurfaces are found using a multigrid line search with the number of points between the surface and the specified maximum depth (`--max-depth=DEPTH` in the units of the model coordinate system) given by `--num-search-points`.
The default direction of the line search is shallow to deep; this can be reversed using the `--prefer-deep` command line argument.
The same number of points are used at each level of refinement with the resolution of the maximum level of refinement given by `--vresolution=RESOLUTION` (default=10.0) in the model vertical coordinate system.
After the line search at the finest resolution, the depth of the isosurface is found using linear interpolation.


## Synopsis

Optional command line arguments are in square brackets.

```
geomodelgrids_isosurface [--help] [--log=FILE_LOG]
  --bbox=XMIN,XMAX,YMIN,YMAX
  --hresolution=RESOLUTION
  --isosurface=NAME,VALUE
  --max-depth=DEPTH
  --models=FILE_0,...,FILE_M
  --output=FILE_OUTPUT
  [--depth-reference=SURFACE]
  [--num-search-points=NUM]
  [--vresolution=RESOLUTION]
  [--prefer-deep] 
  [--bbox-coordsys=PROJ|EPSG|WKT]
```

### Required arguments

* **--bbox=XMIN,XMAX,YMIN,YMAX** Bounding box for isosurface.
* **--hresolution=RESOLUTION** Horizontal resolution of isosurface.
* **--isosurface=NAME,VALUE** Name and values for isosurfaces (default=Vs,1.0 and Vs,2.5; repeat for multiple values).
* **--max-depth=DEPTH** Maximum depth allowed for isosurface.
* **--models=FILE_0,...,FILE_M** Models to query (in order).
* **--output=FILE_OUTPUT** Name of file for isosurface values.

### Optional arguments

* **--help** Print help information to stdout and exit.
* **--log=FILE_LOG** Name of file for logging.
* **--depth-reference=SURFACE** Surface to use for calculating depth (default=`topography_bathymetry`)
* **--num-search-points=NUM** Number of search points in each iteration (default=10).
* **--vresolution=RESOLUTION** Vertical resolution for depth of isosurface (default=10.0).
* **--prefer-deep** Prefer deepest elevation for isosurface rather than shallowest (default=shallowest).
* **--bbox-coordsys=PROJ\|EPSG\|WKT** Coordinate system for isosurface points as Proj parameters, EPSG code, or Well-Known Text. Default is EPSG:4326 (latitude, WGS84 degrees; longitude, WGS84 degrees; elevation, m above ellipsoid.

### Output file

The output is a raster grid with two bands stored as a GeoTiff file.
The GeoTiff files includes the geographic coordinate system information as Well-Known-Text (WKT) along with the labels for the bands in the form `NAME=VALUE`, where `NAME` is the name of the model value and `VALUE` is the isosurface value.
The GeoTiff file can be read using a variety of open-source and commercial GIS software.
Note that GeoTiff images can be loaded into many image viewers, but because the bands contain floating point numbers, they will not be rendered as conventional RGB images.

## Example

Extract isosurfaces for values `one=12.0e+4` and `two=40.0e+3` from model `three-blocks-topo.h5` using a bounding box given in latitude and longitude in the WGS84 datum.
We specify a horizontal resolution of 0.1 degrees and a vertical resolution of 500 meters for the finest scale line search.
We specify the maximum search depth to be 45 km, which corresponds to the base of the model.
The model for this example is located in `tests/data`.

```{code-block} bash
geomodelgrids_isosurface \
--models=tests/data/three-blocks-topo.h5 \
--bbox=34.6,34.8,-117.7,-117.3 \
--hresolution=0.1 \
--vresolution=500.0 \
--isosurface=one,12.0e+4 \
--isosurface=two,40.0e+3 \
--max-depth=45.0e+3 \
--depth-reference=topography_bathymetry \
--output=tests/data/three-blocks-topo-isosurface.tiff \
--bbox-coordsys=EPSG:4326
```

We can see the metadata by running gdalinfo on the resulting GeoTiff file.
```{code-block} bash
gdalinfo tests/data/three-blocks-topo-isosurface.tiff

# Output
Driver: GTiff/GeoTIFF
Files: tests/data/three-blocks-topo-isosurface.tiff
Size is 4, 2
Coordinate System is:
GEOGCRS["WGS 84",
    DATUM["World Geodetic System 1984",
        ELLIPSOID["WGS 84",6378137,298.257223563,
            LENGTHUNIT["metre",1]]],
    PRIMEM["Greenwich",0,
        ANGLEUNIT["degree",0.0174532925199433]],
    CS[ellipsoidal,2],
        AXIS["geodetic latitude (Lat)",north,
            ORDER[1],
            ANGLEUNIT["degree",0.0174532925199433]],
        AXIS["geodetic longitude (Lon)",east,
            ORDER[2],
            ANGLEUNIT["degree",0.0174532925199433]],
    USAGE[
        SCOPE["unknown"],
        AREA["World"],
        BBOX[-90,-180,90,180]],
    ID["EPSG",4326]]
Data axis to CRS axis mapping: 2,1
Origin = (-117.700000000000003,34.799999999999997)
Pixel Size = (0.100000000000001,-0.099999999999998)
Metadata:
  AREA_OR_POINT=Area
Image Structure Metadata:
  COMPRESSION=DEFLATE
  INTERLEAVE=PIXEL
Corner Coordinates:
Upper Left  (-117.7000000,  34.8000000) (117d42' 0.00"W, 34d48' 0.00"N)
Lower Left  (-117.7000000,  34.6000000) (117d42' 0.00"W, 34d36' 0.00"N)
Upper Right (-117.3000000,  34.8000000) (117d18' 0.00"W, 34d48' 0.00"N)
Lower Right (-117.3000000,  34.6000000) (117d18' 0.00"W, 34d36' 0.00"N)
Center      (-117.5000000,  34.7000000) (117d30' 0.00"W, 34d42' 0.00"N)
Band 1 Block=4x2 Type=Float32, ColorInterp=Gray
  Description = one=120000
  NoData Value=-1.00000002004087734e+20
Band 2 Block=4x2 Type=Float32, ColorInterp=Undefined
  Description = two=40000
  NoData Value=-1.00000002004087734e+20
```
