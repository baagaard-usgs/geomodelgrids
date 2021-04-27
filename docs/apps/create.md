# geomodelgrids_create_model

The `geomodelgrids_create_model` command line program is used to generate a model from a data source.
Currently, only the EarthVision data source is implemented.

The model is generated in pieces, looping over the blocks and batches of points within a block.
This allows arbitrarily large models to be generated on computers with limited memory.

## Synopsis

Optional command line arguments are in square brackets.

```
geomodelgrids_create_model
  [--help]
  --config=CONFIG
  [--show-parameters]
  [--import-domain]
  [--import-surfaces]
  [--import-blocks]
  [--all]
  [--quiet]
  [--log=LOG_FILENAME]
  [--debug]
```

### Required arguments

- **--config=FILE_0,...,FILE_N** Names of `N` model configuration files.

### Optional arguments

- **--help** Print help information to stdout and exit.
- **--import-domain** Create domain.
- **--import-surfaces** Create surfaces.
- **--import-blocks** Create blocks.
- **--log=LOG_FILENAME** Name of file for logging.
- **--debug** Log debugging information.

## Model configuration files

The model configuration files use the [Python Config](https://docs.python.org/3/library/configparser.html) syntax. In general, we denote lists using square brackets and comma separated entries. The files contain the following sections:

- **geomodelgrids** Model metadata.
- **coordsys** Model coordinate system.
- **data** Values stored in the model.
- **domain** Domain information.
- **earthvision** EarthVision model information (specific to the EarthVision data source).
- **earthvision.environment** Bash environment for running EarthVision (specific to the EarthVision data source).

The files also contain sections corresponding to the surface and block names.


### geomodelgrids

- **title** Title of model (str).
- **id** Model identifier (str).
- **description** Description of model (str).
- **version** Version number of model (str).
- **keywords** Comma separated list of keywords (list of str).
- **creator_name** Name of person who generated the GeoModelGrids file (str).
- **creator_institution** Institution of person who create the GeoModelGrids file (str).
- **creator_email** Email address of person who create the GeoModelGrids file (str).
- **acknowledgements** Acknowledgements for the model (str).
- **authors** \| separated list of model authors (list of str).
- **references** \| separated list of publications associated with creation of the model that should be cited when using the model (list of str).
- **doi** Digital Object Identifier (DOI) of the model. (str)
- **filename** Filename for the generated model. (str)
- **data_source** Path to the Python DataSrc object used to generate the model (str). The path must be in the `PYTHONPATH`.

## coordsys

- **crs** Proj compatible coordinate reference system (CRS) (str). Valid values include Proj parameters, EPSG codes, and Well-Known Text (WKT).
- **origin_x** X coordinate of the southwest corner in the CRS (float).
- **origin_y** Y coordinate of the southwest corner in the CRS (float).
- **y_azimuth** Azimuth of the y coordinate axes (clockwise from North) (float).

## data

- **values** Comma separated list of values stored in the model (list of str).
- **units** Comma separated list of units of the values stored in the model (list of str). Units for dimensionless values are `none`.

## domain

- **dim_x** Dimension of domain in x direction in units of CRS (float).
- **dim_y** Dimension of domain in y direction in units of CRS (float).
- **dim_z** Dimension of domain in z direction in units of CRS (float).
- **blocks** Comma separated list of block names (list of str).
- **batch_size** Target number of points to use in a single batch (int).  

## earthvision

This section is specific to the EarthVision data source.

- **model_dir** Full absolute path to the directory containing the EarthVision model (str).
- **top_surface_2grd** Filename of `2grd` file for the top surface of the model (str, optional).
- **topography_bathymetry_2grd** Filename of `2grd` file for the topography/bathymetry surface of the model (str, optional).
- **geologic_model** Filename of the sequence file corresponding to the EarthVision geologic model.
- **elev_units** Units of elevation in the EarthVision geologic model (str).
- **xy_units** Units of x and y coordinates in the EarthVision geologic model (str).
- **rules_module** Path to the Python object used to assign material properties to the geologic units (str). The path must be in the `PYTHONPATH`.

## earthvision.environment

Bash environment variables that include locations of the EarthVision files.

- **PATH** `PATH` variable for executables (str).
- **LD_LIBRARY_PATH** `LD_LIBRARY_PATH` for dynamic libraries (str).
- **LM_LICENSE_FILE** Absolute path of the EarthVision license file (str).

## surfaces

Metadata for the `top_surface` and `topography_bathymetry` surfaces.

- **use_surface** `True` to use surface, `False` to ignore (bool).
- **resolution_horiz** Horizontal resolution, in meters, for surface (float).
- **chunk_size** Tuple of 3 integer values for HDF5 chunk size. The chunk size cannot exceed the dataset size and should be in the range of 10 kilobytes to 1 megabyte.

## blocks

Metadata for blocks in the model.

- **resolution_horiz** Horizontal resolution, in CRS units, for block (float).
- **resolution_vert** Vertical resolution, in CRS units, for block (float).
- **z_top** Z coordinate, in CRS units, of the top of the block (float).
- **z_bot** Z coordinate, in CRS units, of the bottom of the block (float).
- **z_offset** Offset in z coordinate, in CRS units, for the top of the block applied to queries of the data source.
- **chunk_size** Tuple of 4 integer values for HDF5 chunk size. The chunk size cannot exceed the dataset size and should be in the range of 10 kilobytes to 1 megabyte.

### Examples

The `models/usgs_sfbay` directory contains model configuration files for the San Francisco Bay 3D seismic velocity model (detailed and regional domains) that is generated from an EarthVision 3D geologic model.
The models include a top surface (topography and ocean surface) and topography/bathymetry (topography and ocean bottom).

