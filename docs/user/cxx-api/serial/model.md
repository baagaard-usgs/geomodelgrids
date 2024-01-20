(cxx-api-serial-model)=
# Model

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

### Model()

Constructor.

### setInputCRS(const std::string& value)

Set the coordinate reference system (CRS) of query input points.

- **value**[in] CRS of input points as string (PROJ, EPSG, WKT).

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

- **returns** Array of names of values in the model.

### const std::vector\<std::string\>& getValueUnits()

Get units of values in the model.

- **returns** Array of units of values in the model.

### DataLayout getDataLayout()

Get data layout for the model.

- **returns** Data layout for values.

### const double* getDims()

Get model dimension.

- **returns** Model dimensions (m) [x, y, z].

### const double* getOrigin()

Get coordinates of model origin in geographic projection.

- **returns** Coordinates of model origin [x, y].

### double getYAzimuth()

Get azimuth of y coordinate axis.

- **returns** Azimuth (degrees) of y coordinate axis.

### const std::string& getCRSString()

Get coordinate reference system for model.

- **returns** Coordinate reference system for model as Proj, EPSG, or WKT.

### const geomodelgrids::serial::ModelInfo& getInfo()

Get model description information.

- **returns** Model description.

### const geomodelgrids::serial::Surface& getTopSurface()

Get top surface of model.

- **returns** Model surface.

### const geomodelgrids::serial::Surface& getTopoBathy()

Get topography/bathymetry surface of model.

- **returns** Model surface.

### const std::vector\<geomodelgrids::serial::Block*\>& getBlocks()

Get blocks in model.

- **returns** Array of blocks in model.

### bool contains(const double x, const double y, const double z)

Does model contain given point?

- **x**[in] X coordinate of point (in input CRS).
- **y**[in] Y coordinate of point (in input CRS).
- **z**[in] Z coordinate of point (in input CRS).
- **returns** True if model contains given point, false otherwise.

### double queryTopElevation(const double x, const double y)

Query model for elevation of the top surface at a point using bilinear interpolation.

- **x**[in] X coordinate of point (in input CRS).
- **y**[in] Y coordinate of point (in input CRS).
- **returns** Elevation (meters) of surface at point.

### double queryTopoBathyElevation(const double x, const double y)

Query model for elevation of the topography/bathymetry surface at a point using bilinear interpolation.

- **x**[in] X coordinate of point (in input CRS).
- **y**[in] Y coordinate of point (in input CRS).
- **returns** Elevation (meters) of surface at point.

### const double* query(const double x, const double y, const double z)

Query model for values at a point using bilinear interpolation

- **x**[in] X coordinate of point (in input CRS).
- **y**[in] Y coordinate of point (in input CRS).
- **z**[in] Z coordinate of point (in input CRS).
- **returns** Array of model values at point.
