# geomodelgrids_borehole

The `geomodelgrids_borehole` command line program is used to query for values of the model at a virtual borehole.
This program provides a higher-level interface for this common, specific query compared to using `geomodelgrids_query`.

The points in the borehole will start at the surface and go down to the specified maximum depth.
The query values will be interpolated from the model using trilinear interpolation (interpolation along each model axis).


## Synopsis

Optional command line arguments are in square brackets.

```
geomodelgrids_borehole [--help] [--log=FILE_LOG]
  --location=X,Y
  --values=VALUE_0,...,VALUE_N
  --models=FILE_0,...,FILE_M
  --output=FILE_OUTPUT
  [--max-depth=DEPTH]
  [--dz=RESOLUTION]
  [--points-coordsys=PROJ|EPSG|WKT]
```

### Required arguments

* **--location=X,Y** Location of virtual borehole in points coordinate system.
* **--values=VALUE_0,...,VALUE_N** Names of `N` values to be returned in query. Values will be returned in the order specified.
* **--models=FILE_0,...,FILE_M** Names of `M` model files to query. For each point the models are queried in the order given until a model is found that contains value(s) the point.
* **--output=FILE_OUTPUT** Name of file for output values. The format is whitespace separated columns of the input coordinates and `VALUE_0`, ..., `VALUE_N`.

### Optional arguments

* **--help** Print help information to stdout and exit.
* **--log=FILE_LOG** Name of file for logging.
* **--max-depth=DEPTH** Depth extent of virtual borehole in point coordinate system vertical units (default=5000m).
* **--dz=RESOLUTION** Vertical resolution of query points in virtual borehole in point coordinate system vertical units (default=10m).
* **--points-coordsys=PROJ\|EPSG\|WKT** Coordinate reference system of input points as Proj parameters, EPSG code, or Well-Known Text. Default is EPSG:4326 (latitude, WGS84 degrees; longitude, WGS84 degrees; elevation, m above ellipsoid.


### Output file

The output file contains a two line header with the command used to generate the file and column headings.
The header is followed by lines with columns of the elevation and depth in the points coordinate system and the values (in the order they were specified on the command line).


## Example

Query the model `three-blocks-topo.h5` for value `two` in a virtual borehole at a point given in UTM coordinates with the output written to file `three-blocks-topo_borehole.out`.
In this example, we provide the location of the virtual borehole in UTM zone 11 coordinates, which corresponds to EPSG:26911. We probe to a maximum depth of 20 km with a sampling interval of 2 km.

The model file for this example is located in `tests/data`.

```bash
geomodelgrids_borehole \
--models=tests/data/three-blocks-topo.h5 \
--location=436201.11,3884356.88 \
--max-depth=20.0e+3 \
--dz=2000.0 \
--output=tests/data/three-blocks-topo_borehole.out \
--values=two \
--points-coordsys=EPSG:26911

# Contents of tests/data/three-blocks-topo_borehole.out:
# geomodelgrids_borehole --models=tests/data/three-blocks-topo.h5 --location=436201.11,3884356.88 --max-depth=20.0e+3 --dz=2000.0 --output=tests/data/three-blocks-topo_borehole.out --values=two --points-coordsys=EPSG:26911
#    Elevation         Depth           two
  1.516904e+02  0.000000e+00 -9.093030e+03
 -1.848310e+03  2.000000e+03  4.747184e+02
 -3.848310e+03  4.000000e+03  1.004247e+04
 -5.848310e+03  6.000000e+03  1.961021e+04
 -7.848310e+03  8.000000e+03  2.917796e+04
 -9.848310e+03  1.000000e+04  3.874571e+04
 -1.184831e+04  1.200000e+04  4.831346e+04
 -1.384831e+04  1.400000e+04  5.788121e+04
 -1.584831e+04  1.600000e+04  6.744896e+04
 -1.784831e+04  1.800000e+04  7.701670e+04
 -1.984831e+04  2.000000e+04  8.658445e+04
 ```
