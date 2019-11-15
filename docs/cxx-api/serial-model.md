# Serial Model Object 

**Full name**: geomodelgrids::serial::Model

## Methods

* [Model()](#model)
* [open(const char* filename, ModelMode mode)](#openconst-char-filename-modelmode-mode)
* [close()](#close)
* [loadMetadata()](#loadmetadata)
* [getValueNames()](#const-stdvectorstdstring-getvaluenames)
* [getValueUnits()](#const-stdvectorstdstring-getvalueunits)
* [getDims()](#const-double-getdims)
* [getOrigin()](#const-double-getorigin)
* [getYAzimuth()](#double-getyazimuth)
* [getProjectionString()](#const-stdstring-getprojectionstring)
* [getInfo()](#const-geomodelgridsserialmodelinfo-getinfo)
* [getTopography()](#const-geomodelgridsserialtopography-gettopography)
* [getBlocks()](#const-stdvectorgeomodelgridsserialblock-getblocks)
* [contains(const double longitude, const double latitude, const double elevation)](#bool-containsconst-double-longitude-const-double-latitude-const-double-elevation)
* [queryElevation(const double longitude, const double latitude)](#double-queryelevationconst-double-longitude-const-double-latitude)
* [query(const double longitude, const double latitude, const double elevation)](#const-double-queryconst-double-longitude-const-double-latitude-const-double-elevation)


## Model()

Constructor.

## open(const char* filename, ModelMode mode)

Open the model for querying.


## close()

Close the model after querying.


## loadMetadata()

Load model metadata.


## const std::vector\<std::string\>& getValueNames()

Get names of values in the model.

* **returns** Array of names of values in the model.


## const std::vector\<std::string\>& getValueUnits()

Get units of values in the model.

* **returns** Array of units of values in the model.


## const double* getDims()

Get model dimension.

* **returns** Model dimensions (m) [x, y, z].


## const double* getOrigin()

Get coordinates of model origin in geographic projection.

* **returns** Coordinates of model origin [x, y].


## double getYAzimuth()

Get azimuth of y coordinate axis.

* **returns** Azimuth (degrees) of y coordinate axis.


## const std::string& getProjectionString()

Get geographic projection for model.

* **returns** Geographic projection in WKT.


## const geomodelgrids::serial::ModelInfo& getInfo()

Get model description information.

* **returns** Model description.


## const geomodelgrids::serial::Topography& getTopography()

Get topography for model.

* **returns** Model topography.


## const std::vector\<geomodelgrids::serial::Block*\>& getBlocks()

Get blocks in model.

* **returns** Array of blocks in model.


## bool contains(const double longitude, const double latitude, const double elevation)

Does model contain given point?

* **longitude**[in] Longitude (degrees) of point in WGS84 horizontal datum.
* **latitude**[in] Latitude (degrees) of point in WGS84 horizontal datum.
* **elevation**[in] Elevation (meters) of point in XXX vertical datum.
* **returns** True if model contains given point, false otherwise.


## double queryElevation(const double longitude, const double latitude)

Query model for elevation of the ground surface (topograpy) at a point
using bilinear interpolation.

* **longitude**[in] Longitude (degrees) of point in WGS84 horizontal datum.
* **latitude**[in] Latitude (degrees) of point in WGS84 horizontal datum.
* **returns** Elevation (meters).


## const double* query(const double longitude, const double latitude, const double elevation)

Query model for values at a point using bilinear interpolation

* **longitude**[in] Longitude (degrees) of point in WGS84 horizontal datum.
* **latitude**[in] Latitude (degrees) of point in WGS84 horizontal datum.
* **elevation**[in] Elevation (meters) of point in XXX vertical datum.
* **returns** Array of model values at point.
