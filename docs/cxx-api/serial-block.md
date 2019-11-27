# Serial Block Object 

**Full name**: geomodelgrids::serial::Block

## Methods

* [Block(const char* name)](#blockconst-char-name)
* [loadMetadata()](#loadmetadata)
* [getName()](#const-stdstring-getname)
* [getResolutionHoriz()](#double-getresolutionhoriz)
* [getResolutionVert()](#double-getresolutionvert)
* [getZTop()](#double-getztop)
* [getDims()](#const-size_t-getdims)
* [getNumValues())](#size_t-getnumvalues)
* [query(const double x, const double y, const double z)](#const-double-queryconst-double-x-const-double-y-const-double-z)


## Block(const char* name)

Constructor with name.

* **name**[in] Name of block.

## loadMetadata()

Load metadata from the model file.


## const std::string& getName()

Get the name of the block.

* **returns** Name of the block


## double getResolutionHoriz()

Get horizontal resolution of grid in block.

* **returns** Horizontal resolution (m).


## double getResolutionVert()

Get vertical resolution of grid in block.

* **returns** Vertical resolution (m).


## double getZTop()

Get elevation of top of block in topological space.

* **returns** Elvation (m) of top of block.


## const size_t getDims()

Get numebr of values along each grid dimension.

* **returns** Number of points along grid in each dimension [x, y, z].


## size_t getNumValues()

Get number of values stored at each grid point.

* **returns** Number of values stored at each grid point.


## const double* query(const double x, const double y, const double z)

Query for values at a point using bilinear interpolation. 

This low-level function returns all values stored at a point.

* **x[in]** X coordinate of point in model coordinate system.
* **y[in]** Y coordinate of point in model coordinate system.
* **z[in]** Z coordinate of point in model coordinate system.

* **returns** Array of values for model at specified point.
