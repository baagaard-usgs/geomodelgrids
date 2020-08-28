# Serial Query Object 

**Full name**: geomodelgrids::serial::Query

## Methods

* [Query()](#query)
* [getErrorHandler()](#geterrorhandler)
* [initialize(const std::vector\<std::string\>& modelFilenames, const std::vector\<std::string\>& valueNames, const std::string inputCRSString)](#initializeconst-stdvector-modelfilename-const-stdvector-valuenames-const-stdstring-inputcrsstring)
* [setSquashMinElev(const double value)](#setsquashminelevconst-double-value)
* [setSquashing(const bool value)](#setsquashingconst-bool-value)
* [queryElevation(const double x, const double y)](#double-queryelevationconst-double-x-const-double-y)
* [query(const double* values, const double x, const double y, const double z)](#queryconst-double-values-const-double-x-const-double-y-const-double-z)
* [finalize()](#finalize)


### Query()

Constructor.


### getErrorHandler()

Get the error handler.


### initialize(const std::vector\<std::string\>& modelFilenames, const std::vector\<std::string\>& valueNames, const std::string& inputCRSString)

Setup for querying.

* **modelFilenames**[in] Array of model filenames (in query order).
* **valueNames**[in] Array of names of values to return in query.
* **inputCRSString**[in] Coordinate reference system (CRS) as string
  (PROJ, EPSG, WKT) for input points.


### setSquashMinElev(const double value)

Set minimum elevation (m) above which vertical coordinate is given as -depth.

This option is used to adjust (squash) topography to sea level above
`value`. Below `value` the original geometry of the model is
maintained. For example, this maintains the original geometry of
deeper structure.

* **value**[in] Minimum elevation (m) for squashing topography.


### setSquashing(const bool value)

Turn squashing on/off

* **value**[in] True if squashing is on, false otherwise.


### double queryElevation(const double x, const double y)

Query model for elevation of the ground surface (topograpy) at a point.

* **x**[in] X coordinate of of point in (in input CRS).
* **y**[in] Y coordinate of of point in (in input CRS).
* **return value** Elevation (meters) of ground surface at point.


### query(const double* values, const double x, const double y, const double z)

Query model for values at a point.

* **values**[out] Array of values (must be preallocated).
* **x**[in] X coordinate of of point in (in input CRS).
* **y**[in] Y coordinate of of point in (in input CRS).
* **z**[in] Z coordinate of of point in (in input CRS).


### finalize()

Cleanup after querying.
