# The geogrid_query command

The `geogrid_query` command line program is used to query for values
of the model at a set of points.

## Synopsis

Optional command line arguments are in square brackets.

```
geogrid_query [-h] [-v VALUE_0,...,VALUE_N] [-s SQUASH_MIN_ELEV] -f FILE_0,...,FILE_M -i FILE_POINTS -o FILE_VALUES
```

### Required arguments

* **-f FILE_0,...,FILE_M** Names of `M` model files to query. The models
  are queried in the order given until a model is found that contains
  value(s) for the given point.
* **-i FILE_INPUT** Name of file with a list of input points. The
  format is whitespace separated columns of longitude, latitude,
  elevation.
* **-o FILE_OUTPUT** Name of file for output values. The
  format is whitespace separated columns of longitude, latitude,
  elevation, `VALUE_0`, ..., `VALUE_N`.

### Optional arguments

* **-h** Print help information to stdout and exit.
* **-v VALUE_0,...,VALUE_N** Names of `N` values to be returned in
  query. Values will be returned in the order specified. The default
  is to return all values in the order and units in which they are
  stored in the model.
* **-s SQUASH_MIN_ELEV** Vertical coordinates is interpreted as -depth
  instead of elevation if the elevation is above
  `SQUASH_MIN_ELEV`. This option is used to adjust (squash) topography
  to sea level above `SQUASH_MIN_ELEV`. Below `SQUASH_MIN_ELEV` the
  original geometry of the model is maintained. For example, this
  maintains the original geometry of deeper structure.
