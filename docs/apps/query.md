# geomodelgrids_query

The `geomodelgrids_query` command line program is used to query for values of the model at a set of points.

The query values will be interpolated from the model using trilinear interpolation (interpolation along each model axis). If any value in the interpolation is equal to NODATA_VALUE, then the value returned will be NODATA_VALUE.

## Synopsis

Optional command line arguments are in square brackets.

```
geomodelgrids_query [--help] [--log=FILE_LOG]
  --values=VALUE_0,...,VALUE_N
  --models=FILE_0,...,FILE_M
  --points=FILE_POINTS
  --output=FILE_OUTPUT
  [--squash-min-elev=ELEV]
  [--squash-surface=SURFACE]
  [--points-coordsys=PROJ|EPSG|WKT]
```

### Required arguments

* **--values=VALUE_0,...,VALUE_N** Names of `N` values to be returned in query. Values will be returned in the order specified.
* **--models=FILE_0,...,FILE_M** Names of `M` model files to query. For each point the models are queried in the order given until a model is found that contains value(s) the point.
* **--points=FILE_POINTS** Name of file with a list of input points. The format is whitespace separated columns of x, y, z in the user specified coordinate reference system.
* **--output=FILE_OUTPUT** Name of file for output values. The format is whitespace separated columns of the input coordinates and `VALUE_0`, ..., `VALUE_N`.

### Optional arguments

* **--help** Print help information to stdout and exit.
* **--log=FILE_LOG** Name of file for logging.
* **--squash-min-elev=ELEV** Interpret vertical coordinate as depth instead of elevation if the elevation is above `ELEV`. This option is used to adjust (squash) topography to sea level above
  `ELEV`. Below `ELEV` the original geometry of the model is maintained; this maintains the original geometry of deeper structure.
* **--squash-surface=SURFACE** Surface to use as a vertical reference for computing depth. Valid values for `SURFACE` include `top_surface` (default), `topography_bathymetry`, and `none` (disables squashing).
* **--points-coordsys=PROJ\|EPSG\|WKT** Coordinate reference system of input points as Proj parameters, EPSG code, or Well-Known Text. Default is EPSG:4326 (latitude, WGS84 degrees; longitude, WGS84 degrees; elevation, m above ellipsoid.


### Output file

The output file contains a one line header with the command used to generate the file. The header is followed by lines with columns of the input coordinates and the values (in the order they were specified on the command line).


## Examples

The input files for these examples are located in `tests/data`.

### Query with input points in WGS84

Query the model `one-block-flat.h5` for a single value `two` at three points given in file `one-block-flat_latlon.in` with the output written to file `one-block-flat_latlon.out`. We use the default coordinate system for the input points, which is latitude, longitude, elevation with the WGS84 horizontal datum and meters above the ellipsoid for the vertical datum.

```bash
geomodelgrids_query \
--models=tests/data/one-block-flat.h5 \
--points=tests/data/one-block-flat_latlon.in \
--output=tests/data/one-block-flat_latlon.out \
--values=two

# Input: one-block-flat_latlon.in
37.455  -121.941   0.0
37.479  -121.734  -5.0e+3
37.381  -121.581  -3.0e+3

# Output: one-block-flat_latlon.out, latitude (deg), longitude (deg), elevation (m), two (m/s)
# geomodelgrids_query --models=one-block-flat.h5 --points=one-block-flat_latlon.in --output=one-block-flat_latlon.out --values=two
#           x0            x1            x2           two
  3.745500e+01 -1.219410e+02  0.000000e+00 -1.520064e+03
  3.747900e+01 -1.217340e+02 -5.000000e+03  1.853648e+04
  3.738100e+01 -1.215810e+02 -3.000000e+03  7.266073e+03
```


### Query with input points in UTM

Query the model `one-block-flat.h5` for values `one` and `two` at the same three points as the previous example given in file `one-block-flat_utm.in` with the output written to file `one-block-flat_utm.out`. In this example, we provide the points in UTM zone 10 coordinates, which corresponds to EPSG:26910.


```bash
geomodelgrids_query \
--models=tests/data/one-block-flat.h5 \
--points=tests/data/one-block-flat_utm.in \
--output=tests/data/one-block-flat_utm.out \
--values=one,two \
--points-coordsys=EPSG:26910

# Input: one-block-flat_utm.in
593662.64	4145875.37     0.00
611935.55	4148764.09 -5000.00
625627.70	4138083.87 -3000.00

# Output: one-block-flat_utm.out, easting (m), northing (m), elevation (m), one (m), two (m/s)
# geomodelgrids_query --models=one-block-flat.h5 --points=one-block-flat_utm.in --output=one-block-flat_utm.out --values=one,two --points-coordsys=EPSG:26910
#           x0            x1            x2           one           two
  5.936626e+05  4.145875e+06  0.000000e+00  4.702445e+03 -1.520065e+03
  6.119356e+05  4.148764e+06 -5.000000e+03  3.114499e+04  1.853648e+04
  6.256277e+05  4.138084e+06 -3.000000e+03  3.182592e+04  7.266073e+03
```
