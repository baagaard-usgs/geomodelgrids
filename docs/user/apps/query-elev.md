# geomodelgrids_queryelev

The `geomodelgrids_queryelev` command line program is used to query for the elevation of the top surface of the model or the topography/bathymetry surface at a set of points in a model.

The elevation will be interpolated from the raster surface grid using bilinear interpolation (interpolation along each model axis).

## Synopsis

Optional command line arguments are in square brackets.

```
geomodelgrids_queryelev [--help] [--log=FILE_LOG]
  --models=FILE_0,...,FILE_M
  --points=FILE_POINTS
  --output=FILE_OUTPUT
  [--surface=SURFACE]
  [--points-coordsys=PROJ|EPSG|WKT]
```

### Required arguments

* **--models=FILE_0,...,FILE_M** Names of `M` model files to query. For each point the models are queried in the order given until a model is found that contains value(s) the point.
* **--points=FILE_POINTS** Name of file with a list of input points. The format is whitespace separated columns of x, y in the user specified coordinate reference system.
* **--output=FILE_OUTPUT** Name of file for output values. The format is whitespace separated columns of the input coordinates and elevation.

### Optional arguments

* **--help** Print help information to stdout and exit.
* **--log=FILE_LOG** Name of file for logging.
* **--surface=SURFACE** Name of surface to query; `top_surface` (default) or `topography_bathymetry`.
* **--points-coordsys=PROJ\|EPSG\|WKT** Coordinate reference system of input points as Proj parameters, EPSG code, or Well-Known Text. Default is EPSG:4326 (latitude, WGS84 degrees; longitude, WGS84 degrees; elevation, m above ellipsoid.


### Output file

The output file contains a one line header with the command used to generate the file. The header is followed by lines with columns of the input coordinates and the elevation of the ground surface in the model coordinate system.


## Example

The input files for these examples are located in `tests/data`.

Query the model `one-block-topo.h5` for the elevation of the ground surface at three points given in file `one-block-flat_elev.in` with the output written to file `one-block-flat_elev.out`. We use the default coordinate system for the input points, which is latitude and longitude in the WGS84 horizontal datum. The elevation of the ground surface is given in meters above the WGS84 ellipsoid.

```bash
geomodelgrids_queryelev \
--models=tests/data/one-block-topo.h5 \
--points=tests/data/one-block-topo_elev.in \
--output=tests/data/one-block-flat_elev.out

# Input: one-block-topo_elev.in
37.455  -121.941
37.479  -121.734
37.381  -121.581

# Output: one-block-topo_latlon.out, latitude (deg), longitude (deg), elevation (m)
# geomodelgrids_queryelev --models=tests/data/one-block-topo.h5 --points=tests/data/one-block-topo_elev.in --output=tests/data/one-block-flat_elev.out
#           x0            x1     elevation
  3.745500e+01 -1.219410e+02  1.500461e+02
  3.747900e+01 -1.217340e+02  1.497750e+02
  3.738100e+01 -1.215810e+02  1.500231e+02
```
