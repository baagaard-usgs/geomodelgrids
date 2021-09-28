# geomodelgrids_create_model

Note: This application requires Python 3.7 or later and is not included in the binary package.

The `geomodelgrids_create_model` command line program is used to generate a model from a data source.
The current data sources include CSV files, IRIS EMC files (vertex-based grids), and EarthVision.

## Synopsis

Optional command line arguments are in square brackets.

```
geomodelgrids_create_model
  --config=CONFIG
  [--help]
  [--show-parameters]
  [--import-domain]
  [--import-surfaces]
  [--import-blocks]
  [--update-metadata]
  [--all]
  [--quiet]
  [--log=LOG_FILENAME]
  [--debug]
```

### Required arguments

+ **`--config=FILE_0,...,FILE_N`** Names of `N` model configuration files.

### Optional arguments

+ **`--help`** Print help information to stdout and exit.
+ **`--show-parameters`** Print parameters to stdout.
+ **`--import-domain`** Create domain.
+ **`--import-surfaces`** Create surfaces.
+ **`--import-blocks`** Create blocks.
+ **`--all`** Equivalent to `--import-domain --import-surfaces --import-block`.
+ **`--update-metadata`** Update all metadata in file using current model configuration.
+ **`--quiet`** Turn off printing progress information to stdout.
+ **`--log=LOG_FILENAME`** Name of file for logging output.
+ **`--debug`** Log debugging information.

:::{hint}
The `--update-metadata` option makes it possible to update the model, surface, and block metadata without altering the values.
It is very useful for updating metadata associated with publication or archiving of the model, such as repository information, references, acknowledgement, history, and comment.
:::

## Model configuration files

The model configuration files use the [Python Config](https://docs.python.org/3/library/configparser.html) syntax. In general, we denote lists using comma separated entries surrounded by square brackets. The files contain the following sections:

+ **`geomodelgrids`** Model metadata.
+ **`coordsys`** Model coordinate system.
+ **`data`** Values stored in the model.
+ **`domain`** Domain information.

The files also contain sections for the data source and sections corresponding to the surface and block names.

### `geomodelgrids` parameters

+ **title** *(string)* Title of model.
+ **id** *(string)* Model identifier.
+ **description** *(string)* Description of model.
+ **keywords** *(array of string)* Comma separated list of keywords.
+ **history** *(string)* History of model.
+ **comment** *(string)* General comments for model.
+ **creator_name** *(string)* Name of person who generated the GeoModelGrids file.
+ **creator_email** *(string)* Email address of person who create the GeoModelGrids file.
+ **creator_institution** *(string)* Institution of person who create the GeoModelGrids file.
+ **acknowledgement** *(string)* Acknowledgement for the model.
+ **authors** *(array of string)* \| separated list of model authors.
+ **references** *(array of string)* \| separated list of publications associated with creation of the model that should be cited when using the model.
+ **repository_name** *(string)* Name of repository holding model.
+ **repository_url** *(string)* URL of repository holding model.
+ **repository_doi** *(string)* Digital Object Identifier (DOI) of the model.
+ **version** *(string)* Version number of model.
+ **license** *(string)* Name of license for the model.
+ **filename** *(string)* Filename for the generated model.
+ **data_source** *(string)* Path to the Python DataSrc object used to generate the model. The path must be in the `PYTHONPATH`.

## `coordsys` parameters

+ **crs** *(string)* Proj compatible coordinate reference system (CRS). Valid values include Proj parameters, EPSG codes, and Well-Known Text (WKT).
+ **origin_x** *(float)* X coordinate of the southwest corner in the CRS.
+ **origin_y** *(float)* Y coordinate of the southwest corner in the CRS.
+ **y_azimuth** *(float)* Azimuth of the y coordinate axes (clockwise from North).

## `data` parameters

+ **values** *(array of string)* Comma separated list of values stored in the model.
+ **units** *(array of string)* Comma separated list of units of the values stored in the model. Units for dimensionless values are `none`.
+ **layout** *(string)* `vertex` for vertex-based values (values are specified at coordinates of vertices) or `cell` for cell-based values (values are specified at center of grid cells). Currently only vertex-based values are supported.

## `domain` parameters

+ **dim_x** *(float)* Dimension of domain in x direction in units of CRS.
+ **dim_y** *(float)* Dimension of domain in y direction in units of CRS.
+ **dim_z** *(float)* Dimension of domain in z direction in units of CRS.
+ **blocks** *(float)* Comma separated list of block names.
+ **batch_size** *(integer)* Target number of points to use in a single batch when generating a model in pieces (avoids loading an entire model into memory).

## `surface` parameters

Metadata for the `top_surface` and `topography_bathymetry` surfaces.

+ **use_surface** *(boolean)* `True` to use surface, `False` to ignore.
+ **chunk_size** *(array)* Tuple of 3 integer values for HDF5 chunk size. The chunk size cannot exceed the dataset size and should be in the range of 10 kilobytes to 1 megabyte.

Any axis can have either a uniform resolution grid or a variable resolution grid.
The grid resolution need not be the same type along the axes.

### Uniform resolution parameters

Parameters if a grid has uniform along corresponding coordinate axis.

+ **resolution_x** *(float)* Horizontal resolution, in CRS units, along x axis.
+ **resolution_y** *(float)* Horizontal resolution, in CRS units, along y axis.

### Variable resolution parameters

Parameters if a grid has variable resolution along corresponding coordinate axis.

+ **coordinates_x** *(float)* Coordinates, in CRS units, along x axis.
+ **coordinates_y** *(float)* Coordinates, in CRS units, along y axis.

```{important}
Some data sources provide some of the parameters.
In those cases, the parameter file need not include those parameters provided by the data source.
```
## `block` parameters

Metadata for blocks in the model.

+ **z_top** *(float)* Z coordinate, in CRS units, of the top of the block.
+ **z_bot** *(float)* Z coordinate, in CRS units, of the bottom of the block.
+ **z_offset** *(float)* Offset in z coordinate, in CRS units, for the top of the block applied to queries of the data source.
+ **chunk_size** *(array)* Tuple of 4 integer values for HDF5 chunk size. The chunk size cannot exceed the dataset size and should be in the range of 10 kilobytes to 1 megabyte.

### Uniform resolution parameters

Parameters if a grid has uniform along corresponding coordinate axis.

+ **resolution_x** *(float)* Horizontal resolution, in CRS units, along x axis.
+ **resolution_y** *(float)* Horizontal resolution, in CRS units, along y axis.
+ **resolution_z** *(float)* Horizontal resolution, in CRS units, along z axis.

### Variable resolution parameters

Parameters if a grid has variable resolution along corresponding coordinate axis.

+ **coordinates_x** *(float)* Coordinates, in CRS units, along x axis.
+ **coordinates_y** *(float)* Coordinates, in CRS units, along y axis.
+ **coordinates_z** *(float)* Coordinates, in CRS units, along z axis.

```{important}
Some data sources provide some of the parameters.
In those cases, the parameter file need not include those parameters provided by the data source.
```

## Data sources

```{toctree}
data_srcs/csv.md
data_srcs/iris-emc.md
data_srcs/earthvision.md
```
