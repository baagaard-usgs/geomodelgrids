(cxx-api-serial-query)=
# Query

**Full name**: geomodelgrids::serial::Query

## Enumerated types

### SquashingEnum

- **SQUASH_NONE** No squashing.
- **SQUASH_TOP_SURFACE** Squash relative to the top surface of the model.
- **SQUASH_TOPOGRPAHY_BATHYMETRY** Squash relative to the topography/bathymetry surface.

## Methods

### Query()

Constructor.

### getErrorHandler()

Get the error handler.

### initialize(const std::vector\<std::string\>& modelFilenames, const std::vector\<std::string\>& valueNames, const std::string& inputCRSString)

Setup for querying.

- **modelFilenames**[in] Array of model filenames (in query order).
- **valueNames**[in] Array of names of values to return in query.
- **inputCRSString**[in] Coordinate reference system (CRS) as string (PROJ, EPSG, WKT) for input points.

### setSquashMinElev(const double value)

Set minimum elevation (m) above which vertical coordinate is given as -depth.

This option is used to adjust (squash) topography to sea level above `value`. Below `value` the original geometry of the model is maintained. For example, this maintains the original geometry of deeper structure.

- **value**[in] Minimum elevation (m) for squashing topography.

### setSquashing(const SquashingEnum value)

Set type of squashing.

- **value**[in] True if squashing is on, false otherwise.

### double queryTopElevation(const double x, const double y)

Query model for elevation of the top surface of the model at a point using bilinear interpolation (interpolation along each model axis).

- **x**[in] X coordinate of of point in (in input CRS).
- **y**[in] Y coordinate of of point in (in input CRS).
- **return value** Elevation (meters) of top surface at point.

### double queryTopoBathyElevation(const double x, const double y)

Query model for elevation of the topography/bathymetry surface of the model at a point using bilinear interpolation (interpolation along each model axis). If the model does not contain a topography/bathymetry surface, then the top surface of the model is used.

- **x**[in] X coordinate of of point in (in input CRS).
- **y**[in] Y coordinate of of point in (in input CRS).
- **return value** Elevation (meters) of topography/bathymetry surface at point.

### query(const double* values, const double x, const double y, const double z)

Query model for values at a point using trilinear interpolation (interpolation along each model axis).

- **values**[out] Array of values (must be preallocated).
- **x**[in] X coordinate of of point in (in input CRS).
- **y**[in] Y coordinate of of point in (in input CRS).
- **z**[in] Z coordinate of of point in (in input CRS).

### finalize()

Cleanup after querying.
