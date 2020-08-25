# Serial Block Object 

**Full name**: geomodelgrids::serial::Block

## Methods

* [Block(const char* name)](#blockconst-char-name)
* [loadMetadata(geomodelgrids::serial::HDF5* const h5)](#loadmetadatageomodelgridsserialhdf5-const-h5)
* [getName()](#const-stdstring-getname)
* [getResolutionHoriz()](#double-getresolutionhoriz)
* [getResolutionVert()](#double-getresolutionvert)
* [getZTop()](#double-getztop)
* [getZBottom()](#double-getzbottom)
* [getDims()](#const-size_t-getdims)
* [getNumValues())](#size_t-getnumvalues)
* [setHyperslabDims(const size_t dims\[\], const size_t ndims)](#sethyperslabdimsconst-size_t-dims-const-size_t-ndims)
* [openQuery(geomodelgrids::serial::HDF5* const h5)](#openquerygeomodelgridsserialhdf5-const-h5)
* [query(const double x, const double y, const double z)](#const-double-queryconst-double-x-const-double-y-const-double-z)
* [closeQuery()](#closequery)


### Block(const char* name)

Constructor with name.

* **name**[in] Name of block.


### loadMetadata(geomodelgrids::serial::HDF5* const h5)

Load metadata from the model file.

* **h5**[in] HDF5 object with model.


### const std::string& getName()

Get the name of the block.

* **returns** Name of the block


### double getResolutionHoriz()

Get horizontal resolution of grid in block.

* **returns** Horizontal resolution (m).


### double getResolutionVert()

Get vertical resolution of grid in block.

* **returns** Vertical resolution (m).


### double getZTop()

Get elevation of top of block in logical space.

* **returns** Elvation (m) of top of block.


### double getZBottom()

Get elevation of bottom of block in logical space.

* **returns** Elvation (m) of bottom of block.


### const size_t getDims()

Get numebr of values along each grid dimension.

* **returns** Number of points along grid in each dimension [x, y, z].


### size_t getNumValues()

Get number of values stored at each grid point.

* **returns** Number of values stored at each grid point.


### setHyperslabDims(const size_t dims\[\], const size_t ndims)

Set hyperslab size.

* **dims**[in] Dimensions of hyperslab.
* **ndims**[in] Number of dimensions.


### openQuery(geomodelgrids::serial::HDF5* const h5)

Prepare for querying.

* **h5** HDF5 object with model.


### const double* query(const double x, const double y, const double z)

Query for values at a point using bilinear interpolation. 

This low-level function returns all values stored at a point.

* **x[in]** X coordinate of point in model coordinate system.
* **y[in]** Y coordinate of point in model coordinate system.
* **z[in]** Z coordinate of point in model coordinate system.

* **returns** Array of values for model at specified point.


### closeQuery()

Cleanup after querying.
