# The geomodelgrids_queryelev command

The `geomodelgrids_queryelev` command line program is used to query for
the elevation of the ground surface at a set of points in a model.

## Synopsis

Optional command line arguments are in square brackets.

```
geomodelgrids_queryelev [--help] --models=FILE_0,...,FILE_M --points=FILE_POINTS [--points-coordsys=PROJ|EPSG|WKT] [--log=FILE_LOG] --output=FILE_OUTPUT
```

### Required arguments

* **--models=FILE_0,...,FILE_M** Names of `M` model files to
  query. For each point the models are queried in the order given
  until a model is found that contains value(s) the point.
* **--points=FILE_POINTS** Name of file with a list of input points. The
  format is whitespace separated columns of x, y in the user
  specified coordinate reference system.
* **--output=FILE_OUTPUT** Name of file for output values. The format
  is whitespace separated columns of the input coordinates and
  elevation.

### Optional arguments

* **--help** Print help information to stdout and exit.
* **--points-coordsys=PROJ|EPSG|WKT** Coordinate reference system of
  input points as Proj parameters, EPSG code, or Well-Known
  Text. Default is EPSG:4326 (latitude, WGS84 degrees; longitude,
  WGS84 degrees; elevation, m above ellipsoid.
* **--log=FILE_LOG** Name of file for logging.
