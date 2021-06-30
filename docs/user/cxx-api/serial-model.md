# Serial Model Object 

**Full name**: geomodelgrids::serial::Model

## Enums

### ModelMode

- **READ** Read only mode.
- **READ_WRITE** Read/write mode.
- **READ_WRITE_TRUNCATE** Read/write mode, truncate upon opening.

## Methods

+ [Model()](#model)
+ [setInputCRS(const std::string& value)](#setinputcrsconst-stdstring-value)
+ [open(const char* filename, ModelMode mode)](#openconst-char-filename-modelmode-mode)
+ [close()](#close)
+ [loadMetadata()](#loadmetadata)
+ [initialize()](#initialize)
+ [getValueNames()](#const-stdvectorstdstring-getvaluenames)
+ [getValueUnits()](#const-stdvectorstdstring-getvalueunits)
+ [getDims()](#const-double-getdims)
+ [getOrigin()](#const-double-getorigin)
+ [getYAzimuth()](#double-getyazimuth)
+ [getCRSString()](#const-stdstring-getcrsstring)
+ [getInfo()](#const-geomodelgridsserialmodelinfo-getinfo)
+ [getTopSurface()](#const-geomodelgridsserialsurface-gettopsurface)
+ [getTopoBathy()](#const-geomodelgridsserialsurface-gettopobathy)
+ [getBlocks()](#const-stdvectorgeomodelgridsserialblock-getblocks)
+ [contains(const double x, const double y, const double z)](#bool-containsconst-double-x-const-double-y-const-double-z)
+ [queryTopElevation(const double x, const double y)](#double-querytopelevationconst-double-x-const-double-y)
+ [queryTopoBathyElevation(const double x, const double y)](#double-querytopobathyelevationconst-double-x-const-double-y)
+ [query(const double x, const double y, const double z)](#const-double-queryconst-double-x-const-double-y-const-double-z)


### Model()

Constructor.

### setInputCRS(const std::string& value)

Set the coordinate reference system (CRS) of query input points.

* **value**[in] CRS of input points as string (PROJ, EPSG, WKT).


### open(const char* filename, ModelMode mode)

Open the model for querying.


### close()

Close the model after querying.


### loadMetadata()

Load model metadata.


### initialize()

Initialize the model.


### const std::vector\<std::string\>& getValueNames()

Get names of values in the model.

* **returns** Array of names of values in the model.


### const std::vector\<std::string\>& getValueUnits()

Get units of values in the model.

* **returns** Array of units of values in the model.


### const double* getDims()

Get model dimension.

* **returns** Model dimensions (m) [x, y, z].


### const double* getOrigin()

Get coordinates of model origin in geographic projection.

* **returns** Coordinates of model origin [x, y].


### double getYAzimuth()

Get azimuth of y coordinate axis.

* **returns** Azimuth (degrees) of y coordinate axis.


### const std::string& getCRSString()

Get coordinate reference system for model.

* **returns** Coordinate reference system for model as Proj, EPSG, or WKT.


### const geomodelgrids::serial::ModelInfo& getInfo()

Get model description information.

* **returns** Model description.


### const geomodelgrids::serial::Surface& getTopSurface()

Get top surface of model.

* **returns** Model surface.


### const geomodelgrids::serial::Surface& getTopoBathy()

Get topography/bathymetry surface of model.

* **returns** Model surface.


### const std::vector\<geomodelgrids::serial::Block*\>& getBlocks()

Get blocks in model.

* **returns** Array of blocks in model.


### bool contains(const double x, const double y, const double z)

Does model contain given point?

* **x**[in] X coordinate of point (in input CRS).
* **y**[in] Y coordinate of point (in input CRS).
* **z**[in] Z coordinate of point (in input CRS).
* **returns** True if model contains given point, false otherwise.


### double queryTopElevation(const double x, const double y)

Query model for elevation of the top surface at a point using bilinear interpolation.

* **x**[in] X coordinate of point (in input CRS).
* **y**[in] Y coordinate of point (in input CRS).
* **returns** Elevation (meters) of surface at point.


### double queryTopoBathyElevation(const double x, const double y)

Query model for elevation of the topogrpahy/bathymetry surface at a point using bilinear interpolation.

* **x**[in] X coordinate of point (in input CRS).
* **y**[in] Y coordinate of point (in input CRS).
* **returns** Elevation (meters) of surface at point.


### const double* query(const double x, const double y, const double z)

Query model for values at a point using bilinear interpolation

* **x**[in] X coordinate of point (in input CRS).
* **y**[in] Y coordinate of point (in input CRS).
* **z**[in] Z coordinate of point (in input CRS).
* **returns** Array of model values at point.
