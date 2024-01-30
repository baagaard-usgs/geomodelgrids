(python-api-query)=
# Query

**Full name**: geomodelgrids.Query

## Enums

### SquashingEnum

- **SQUASH_NONE** No squashing.
- **SQUASH_TOP_SURFACE** Squash model so top surface is flat.
- **SQUASH_TOPGRAPHY_BATHYMETRY** Squash model so topography/bathymetry surface is flat.

## Constants

- **NODATA_VALUE** Value returned when query could not return a valid value.

## Methods

### Query()

Constructor.

### initialize(models: list(Model), values: list(str), input_crs: str)

Perform initialization required to query the models.

- **models** List of model filenames (in query order).
- **values** List of names of values to return in queries.
- **input_crs** CRS as string (PROJ, EPSG, WKT) for input points.

### set_squash_min_elev(min_elev: float)

Turn on squashing using the top surface and set the minimum elevation for squashing.

- **min_elev** Elevation in meters.

### set_squashing(squash_type: SquashingEnum)

Set type of squashing.

- **squash_type** Squashing setting (SQUASH_NONE, SQUASH_TOP_SURFACE, SQUASH_TOPOGRAPHY_BATHYMETRY)

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
- **returns** Tuple(values, status) where values is a NumPy array of model values at each point and status is a NumPy array with ErrorHandler.OK for a point if returning a valid value and  ErrorHandler.WARNING for a point if unable to return a valid value.
