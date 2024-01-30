(python-api-model)=
# Model

**Full name**: geomodelgrids.Model

## Enums

### ModelMode

- **READ** Read only mode.
- **READ_WRITE** Read/write mode.
- **READ_WRITE_TRUNCATE** Read/write mode, truncate upon opening.

### DataLayout

- **VERTEX** Vertex-based data (values are specified at coordinates of vertices).
- **CELL** Cell-based data (values are specified at centers of grid cells).

## Read-only Attributes

- **value_names** List of names of values in the model `list(str)`
- **value_units** List of units of values in the model `list(str)`
- **data_layout** Data layout for values (`VERTEX` or `CELL`)
- **dims** Model dimensions in meters [x, y, z] `list(float)`
- **origin** Coordinates of model origin [x, y] `list(float)` in model coordinate system
- **y_azimuth** Azimuth (degrees) of y coordinate axis `float`
- **crs** Coordinate reference system for model as Proj, EPSG, or WKT `str`

## Methods

### Model()

Constructor.

### set_input_crs(value: str)

Set the coordinate reference system (CRS) of query input points.
The default value is `EPSG:4326`, which corresponds to latitude and longitude in degrees in the WGS84 datum.

- **value** CRS of input points as string (PROJ, EPSG, WKT).

### open(filename: str, mode: ModelMode)

Open the model for querying.

- **filename** Name of model file.
- **mode** File mode.

### close()

Close the model after querying.

### load_metadata()

Load model metadata.

### initialize()

Initialize the model.

### get_info()

Get model description information.

- **returns** Model description.

### contains(points: numpy.ndarray)

Does model contain given point?

- **points** NumPy array [numPoints, 3] of point coordinates in input CRS.
- **returns** numpy.ndarray with True if model contains given point, False otherwise.

### query_top_elevation(points: numpy.ndarray)

Query model for elevation of the top surface at a point using bilinear interpolation.

- **points** NumPy array [numPoints, 2[ of point coordinates in input CRS.
- **returns** NumPy array of elevation (meters) of surface at each point.

### query_topobathy_elevation(points: numpy.ndarray)

Query model for elevation of the topography/bathymetry surface at a point using bilinear interpolation.

- **points** NumPy array [numPoints, 2] of point coordinates in input CRS.
- **returns** NumPy array of elevation (meters) of surface at each point.

### query(points: numpy.ndarray)

Query model for values at a point using bilinear interpolation

- **points** NumPy array [numPoints, 3] of point coordinates in input CRS.
- **returns** NumPy array of model values at each point.
