# The geomodelgrids_borehole command

The `geomodelgrids_borehole` command line program is used to query for
values of the model at a virtual borehole. This program provides a
higher-level interface for this common, specific query compared to
using `geomodelgrids_query`.

## Synopsis

Optional command line arguments are in square brackets.

```
geomodelgrids_borehole [--help] [--log=FILE_LOG] --location=X,Y --models=FILE_0,...,FILE_M --output=FILE_OUTPUT [--values=VALUE_0,...,VALUE_N] [--max-depth=DEPTH] [--dz=RESOLUTION] [--points-coordsys=PROJ|EPSG|WKT]
```

### Required arguments

* **--models=FILE_0,...,FILE_M** Names of `M` model files to
  query. For each point the models are queried in the order given
  until a model is found that contains value(s) the point.
* **--location=X,Y** Location of virtual borehole in points coordinate system.
* **--output=FILE_OUTPUT** Name of file for output values. The format
  is whitespace separated columns of the input coordinates and
  `VALUE_0`, ..., `VALUE_N`.

### Optional arguments

* **--help** Print help information to stdout and exit.
* **--log=FILE_LOG** Name of file for logging.
* **--values=VALUE_0,...,VALUE_N** Names of `N` values to be returned in
  query. Values will be returned in the order specified. The default
  is to return all values in the order and units in which they are
  stored in the model.
* **--max-depth=DEPTH** Depth extext of virtual borehole in point
  coordinate system vertical units (default=5000m).
* **--dz=RESOLUTION** Vertical resolution of query points in virtual
  borehole in point coordinate system vertical units (default=10m).
* **--points-coordsys=PROJ|EPSG|WKT** Coordinate reference system of
  input points as Proj parameters, EPSG code, or Well-Known
  Text. Default is EPSG:4326 (latitude, WGS84 degrees; longitude,
  WGS84 degrees; elevation, m above ellipsoid.
