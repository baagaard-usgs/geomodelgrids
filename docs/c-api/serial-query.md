# Serial Query functions

These functions are prefixed by `geomodelgrids_squery`.

## Functions

* [geomodelgrids_squery_create()](#void-geomodelgrids_squery_create)
* [geomodelgrids_squery_destroy(void** handle)](#geomodelgrids_squery_destroyvoid-handle)
* [geomodelgrids_squery_getErrorHandler(void* handle)](#void-geomodelgrids_squery_geterrorhandler)
* [geomodelgrids_squery_initialize(void* handle, const char* const modelFilenames\[\], const int modelFilenamesSize, const char* const valueNames\[\], const int valueNamesSize, const char* const inputCRSString)](#int-geomodelgrids_squery_initializevoid-handle-const-char-const-modelfilenames-const-int-modelfilenamessize-const-char-const-valuenames-const-int-valuenamessize-const-char-const-inputcrsstring)
* [geomodelgrids_squery_setSquashMinElev(const void* handle, const double value)](#int-geomodelgrids_squery_setsquashminelevconst-double-value)
* [geomodelgrids_squery_setSquashing(const void* handle, const int value)](#int-geomodelgrids_squery_setsquashingconst-int-value)
* [geomodelgrids_squery_queryElevation(const void* handle, const double x, const double y)](#double-geomodelgrids_squery_queryelevationconst-double-x-const-double-y)
* [geomodelgrids_squery_query(const void* handle, const double* values, const double x, const double y, const double z)](#int-geomodelgrids_squery_queryconst-double-values-const-double-x-const-double-y-const-double-z)
* [geomodelgrids_squery_finalize(void* handle)](#geomodelgrids_squery_finalize)


### void* geomodelgrids_squery_create()

Create C++ query object.

* **returns** Pointer to C++ query object (NULL on failure).


### geomodelgrids_squery_destroy(void** handle)

Destroy C++ query object.


### void* geomodelgrids_squery_getErrorHandler()

Get the error handler.

* **handle**[in] Pointer to C++ query object.
* **returns** Pointer to C++ error handler.


### int geomodelgrids_squery_initialize(void* handle, const char* const modelFilenames\[\], const int modelFilenamesSize, const char* const valueNames\[\], const int valueNamesSize, const char* const inputCRSString)

Setup for querying.

* **handle**[in] Pointer to C++ query object.
* **modelFilenames**[in] Array of model filenames (in query order).
* **modelFilenamesSize**[in] Size of model filenames array.
* **valueNames**[in] Array of names of values to return in query.
* **valueNamesSize**[in] Size of names of values array.
* **inputCRSString**[in] Coordinate reference system (CRS) as string
  (PROJ, EPSG, WKT) for input points.
* **returns** GeomodelgridsStatusEnum for error status.


### int geomodelgrids_squery_setSquashMinElev(const double value)

Set minimum elevation (m) above which vertical coordinate is given as -depth.

This option is used to adjust (squash) topography to sea level above
`value`. Below `value` the original geometry of the model is
maintained. For example, this maintains the original geometry of
deeper structure.

* **handle**[in] Pointer to C++ query object.
* **value**[in] Minimum elevation (m) for squashing topography.
* **returns** GeomodelgridsStatusEnum for error status.


### int geomodelgrids_squery_setSquashing(const int value)

Turn squashing on/off

* **handle**[in] Pointer to C++ query object.
* **value**[in] 1 if squashing is on, 0 otherwise.
* **returns** GeomodelgridsStatusEnum for error status.


### double geomodelgrids_squery_queryElevation(const double x, const double y)

Query model for elevation of the ground surface (topograpy) at a point.

* **handle**[in] Pointer to C++ query object.
* **x**[in] X coordinate of of point in (in input CRS).
* **y**[in] Y coordinate of of point in (in input CRS).
* **return value** Elevation (meters) of ground surface at point.


### int geomodelgrids_squery_query(const double* values, const double x, const double y, const double z)

Query model for values at a point.

* **handle**[in] Pointer to C++ query object.
* **values**[out] Array of values (must be preallocated).
* **x**[in] X coordinate of of point in (in input CRS).
* **y**[in] Y coordinate of of point in (in input CRS).
* **z**[in] Z coordinate of of point in (in input CRS).
* **returns** GeomodelgridsStatusEnum for error status.


### geomodelgrids_squery_finalize()

Cleanup after querying.

* **handle**[in] Pointer to C++ query object.
