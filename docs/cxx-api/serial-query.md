# Serial Query Object 

**Full name**: geomodelgrids::serial::Query

## Methods

* [Query(const char* filename)](#queryconst-char_filename)
* [open()](#open)
* [close()](#close)
* [setValues(const char** names, const size_t numNames)](#setvaluesconst-char-names-const-size_t-numnames)
* [setSquashMinElev(const double value)](#setsquashminelevconst-double-value)
* [setCacheSize(const double value)](#setcachesizeconst-double-value)
* [queryTopography(const double longitude, const double latitude)](#double-querytopographyconst-double-longitude-const-double-latitude)
* [query(const double* values, const int numValues, const double longitude, const double latitude, const double elevation)](#queryconst-double-values-const-int-numvalues-const-double-longitude-const-double-latitude-const-double-elevation)
* [query(const double** values, const int numValues, const double* points), const size_t numPoints](#queryconst-double-values-const-int-numvalues-const-double-points-const-size_t-numpoints)


## Query(const char* filename)

Constructor with filename.

* **filename**[in] Name of model file.

## open()

Open the model for querying.


## setValues(const char** names, const size_t numNames)

Set the values to be returned in a query.

* **names**[in] Array of value names.
* **numNames**[in] Size of array.


## setSquashMinElev(const double value)

Set minimum elevation (m) above which vertical coordinate is given as -depth.

This option is used to adjust (squash) topography to sea level above
`value`. Below `value` the original geometry of the model is
maintained. For example, this maintains the original geometry of
deeper structure.

* **value**[in] Minimum elevation (m) for squashing topography.


## setCacheSize(const double value)

Set maximum cache size (MB).

* **value**[in] Maximum size of cache in MB.


## double queryTopography(const double longitude, const double latitude)

Query model for elevation of the ground surface (topograpy) at a point.

* **longitude**[in] Longitude (degrees) of point in WGS84 horizontal datum.
* **latitude**[in] Latitude (degrees) of point in WGS84 horizontal datum.
* **return value** Elevation (meters).


## query(const double* values, const int numValues, const double longitude, const double latitude, const double elevation)

Query model for values at a point.

* **values**[out] Array of values (must be preallocated).
* **numValues**[in] Number of values (size of array).
* **longitude**[in] Longitude (degrees) of point in WGS84 horizontal datum.
* **latitude**[in] Latitude (degrees) of point in WGS84 horizontal datum.
* **elevation**[in] Elevation (meters) of point in XXX vertical datum.


## query(const double** values, const int numValues, const double* points, const size_t numPoints)

Query model for values at a set of points. The coordinates of the
points are longitude (degrees; WGS84 horizontal datum), latitude
(degrees; WGS84 horizontal datum), and elevation (meters; XXX vertical
datum).

* **values**[out] Array of values at the set of points (must be preallocated).
* **numValues**[in] Number of values (size of array).
* **points**[in] Array of coordinates of the points [numPoints, 3]
* **numPoints**[in] Number of points.
