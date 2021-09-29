# Serial Model Object 

**Full name**: geomodelgrids::serial::Model

## Enums

### ModelMode

- **READ** Read only mode.
- **READ_WRITE** Read/write mode.
- **READ_WRITE_TRUNCATE** Read/write mode, truncate upon opening.

### DataLayout

- **VERTEX** Vertex-based data (values are specified at coordinates of vertices).
- **CELL** Cell-based data (values are specified at centers of grid cells).

## Methods

+ [Model()](cxx-api-serial-model-Model)
+ [setInputCRS(const std::string& value)](cxx-api-serial-model-setInputCRS)
+ [open(const char* filename, ModelMode mode)](cxx-api-serial-model-open)
+ [close()](cxx-api-serial-model-close)
+ [loadMetadata()](cxx-api-serial-model-loadMetadata)
+ [initialize()](cxx-api-serial-model-initialize)
+ [getValueNames()](cxx-api-serial-model-getValueNames)
+ [getValueUnits()](cxx-api-serial-model-getValueUnits)
+ [getDataLayout()](cxx-api-serial-model-getDataLayout)
+ [getDims()](cxx-api-serial-model-getDims)
+ [getOrigin()](cxx-api-serial-model-getOrigin)
+ [getYAzimuth()](cxx-api-serial-model-getYAzimuth)
+ [getCRSString()](cxx-api-serial-model-getCRSString)
+ [getInfo()](cxx-api-serial-model-getInfo)
+ [getTopSurface()](cxx-api-serial-model-getTopSurface)
+ [getTopoBathy()](cxx-api-serial-model-getTopoBathy)
+ [getBlocks()](cxx-api-serial-model-getBlocks)
+ [contains(const double x, const double y, const double z)](cxx-api-serial-model-contains)
+ [queryTopElevation(const double x, const double y)](cxx-api-serial-model-queryTopElevation)
+ [queryTopoBathyElevation(const double x, const double y)](cxx-api-serial-model-queryTopoBathy)
+ [query(const double x, const double y, const double z)](cxx-api-serial-model-query)


(cxx-api-serial-model-Model)=
### Model()

Constructor.

(cxx-api-serial-model-setInputCRS)=
### setInputCRS(const std::string& value)

Set the coordinate reference system (CRS) of query input points.

* **value**[in] CRS of input points as string (PROJ, EPSG, WKT).


(cxx-api-serial-model-open)=
### open(const char* filename, ModelMode mode)

Open the model for querying.


(cxx-api-serial-model-close)=
### close()

Close the model after querying.

(cxx-api-serial-model-loadMetadata)=
### loadMetadata()

Load model metadata.

(cxx-api-serial-model-initialize)=
### initialize()

Initialize the model.

(cxx-api-serial-model-getValueNames)=
### const std::vector\<std::string\>& getValueNames()

Get names of values in the model.

* **returns** Array of names of values in the model.

(cxx-api-serial-model-getValueUnits)=
### const std::vector\<std::string\>& getValueUnits()

Get units of values in the model.

* **returns** Array of units of values in the model.

(cxx-api-serial-model-getDataLayout)=
### DataLayout getDataLayout()

Get data layout for the model.

* **returns** Data layout for values.

(cxx-api-serial-model-getDims)=
### const double* getDims()

Get model dimension.

* **returns** Model dimensions (m) [x, y, z].

(cxx-api-serial-model-getOrigin)=
### const double* getOrigin()

Get coordinates of model origin in geographic projection.

* **returns** Coordinates of model origin [x, y].

(cxx-api-serial-model-getYAzimuth)=
### double getYAzimuth()

Get azimuth of y coordinate axis.

* **returns** Azimuth (degrees) of y coordinate axis.

(cxx-api-serial-model-getCRSString)=
### const std::string& getCRSString()

Get coordinate reference system for model.

* **returns** Coordinate reference system for model as Proj, EPSG, or WKT.

(cxx-api-serial-model-getInfo)=
### const geomodelgrids::serial::ModelInfo& getInfo()

Get model description information.

* **returns** Model description.

(cxx-api-serial-model-getTopSurface)=
### const geomodelgrids::serial::Surface& getTopSurface()

Get top surface of model.

* **returns** Model surface.

(cxx-api-serial-model-getTopoBathy)=
### const geomodelgrids::serial::Surface& getTopoBathy()

Get topography/bathymetry surface of model.

* **returns** Model surface.

(cxx-api-serial-model-getBlocks)=
### const std::vector\<geomodelgrids::serial::Block*\>& getBlocks()

Get blocks in model.

* **returns** Array of blocks in model.

(cxx-api-serial-model-contains)=
### bool contains(const double x, const double y, const double z)

Does model contain given point?

* **x**[in] X coordinate of point (in input CRS).
* **y**[in] Y coordinate of point (in input CRS).
* **z**[in] Z coordinate of point (in input CRS).
* **returns** True if model contains given point, false otherwise.

(cxx-api-serial-model-queryTopElevation)=
### double queryTopElevation(const double x, const double y)

Query model for elevation of the top surface at a point using bilinear interpolation.

* **x**[in] X coordinate of point (in input CRS).
* **y**[in] Y coordinate of point (in input CRS).
* **returns** Elevation (meters) of surface at point.

(cxx-api-serial-model-queryTopoBathy)=
### double queryTopoBathyElevation(const double x, const double y)

Query model for elevation of the topography/bathymetry surface at a point using bilinear interpolation.

* **x**[in] X coordinate of point (in input CRS).
* **y**[in] Y coordinate of point (in input CRS).
* **returns** Elevation (meters) of surface at point.

(cxx-api-serial-model-query)=
### const double* query(const double x, const double y, const double z)

Query model for values at a point using bilinear interpolation

* **x**[in] X coordinate of point (in input CRS).
* **y**[in] Y coordinate of point (in input CRS).
* **z**[in] Z coordinate of point (in input CRS).
* **returns** Array of model values at point.
