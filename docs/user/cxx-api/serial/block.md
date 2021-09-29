# Serial Block Object 

**Full name**: geomodelgrids::serial::Block

## Methods

+ [Block(const char* name)](cxx-api-serial-block-Block)
+ [loadMetadata(geomodelgrids::serial::HDF5* const h5)](cxx-api-serial-block-loadMetadata)
+ [getName()](cxx-api-serial-block-getName)
+ [getResolutionX()](cxx-api-serial-block-getResolutionX)
+ [getResolutionY()](cxx-api-serial-block-getResolutionY)
+ [getResolutionZ()](cxx-api-serial-block-getResolutionZ)
+ [getZTop()](cxx-api-serial-block-getZTop)
+ [getZBottom()](cxx-api-serial-block-getZBottom)
+ [getCoordinatesX()](cxx-api-serial-block-getCoordinatesX)
+ [getCoordinatesY()](cxx-api-serial-block-getCoordinatesY)
+ [getCoordinatesZ()](cxx-api-serial-block-getCoordinatesZ)
+ [getDims()](cxx-api-serial-block-getDims)
+ [getNumValues())](cxx-api-serial-block-getNumValues)
+ [setHyperslabDims(const size_t dims\[\], const size_t ndims)](cxx-api-serial-block-setHyperslabDims)
+ [openQuery(geomodelgrids::serial::HDF5* const h5)](cxx-api-serial-block-openQuery)
+ [query(const double x, const double y, const double z)](cxx-api-serial-block-query)
+ [closeQuery()](cxx-api-serial-block-closeQuery)
+ [compare(const Block*, const Block*)](cxx-api-serial-block-compare)

(cxx-api-serial-block-Block)=
### Block(const char* name)

Constructor with name.

* **name**[in] Name of block.

(cxx-api-serial-block-loadMetadata)=
### loadMetadata(geomodelgrids::serial::HDF5* const h5)

Load metadata from the model file.

* **h5**[in] HDF5 object with model.

(cxx-api-serial-block-getName)=
### const std::string& getName()

Get the name of the block.

* **returns** Name of the block

(cxx-api-serial-block-getResolutionX)=
### double getResolutionX()

Get resolution along x axis. Only valid (nonzero) for uniform resolution.

* **returns** Resolution along x axis.

(cxx-api-serial-block-getResolutionY)=
### double getResolutionY()

Get resolution along y axis. Only valid (nonzero) for uniform resolution.

* **returns** Resolution along y axis.

(cxx-api-serial-block-getResolutionZ)=
### double getResolutionZ()

Get resolution along z axis. Only valid (nonzero) for uniform resolution.

* **returns** Resolution along z axis.


(cxx-api-serial-block-getZTop)=
### double getZTop()

Get elevation of top of block in logical space.

* **returns** Elevation of top of block.


(cxx-api-serial-block-getZBottom)=
### double getZBottom()

Get elevation of bottom of block in logical space.

* **returns** Elevation of bottom of block.


(cxx-api-serial-block-getCoordinatesX)=
### double* getCoordinatesX(void)

Get coordinates along x axis.

* **returns** Array of coordinates along x axis. Only valid (non-null) if variable resolution.

(cxx-api-serial-block-getCoordinatesY)=
### double* getCoordinatesY(void)

Get coordinates along y axis.

* **returns** Array of coordinates along y axis. Only valid (non-null) if variable resolution.

(cxx-api-serial-block-getCoordinatesZ)=
### double* getCoordinatesZ(void)

Get coordinates along z axis.

* **returns** Array of coordinates along z axis. Only valid (non-null) if variable resolution.


(cxx-api-serial-block-getDims)=
### const size_t getDims()

Get numebr of values along each grid dimension.

* **returns** Number of points along grid in each dimension [x, y, z].


(cxx-api-serial-block-getNumValues)=
### size_t getNumValues()

Get number of values stored at each grid point.

* **returns** Number of values stored at each grid point.


(cxx-api-serial-block-setHyperslabDims)=
### setHyperslabDims(const size_t dims\[\], const size_t ndims)

Set hyperslab size.

* **dims**[in] Dimensions of hyperslab.
* **ndims**[in] Number of dimensions.


(cxx-api-serial-block-openQuery)=
### openQuery(geomodelgrids::serial::HDF5* const h5)

Prepare for querying.

* **h5** HDF5 object with model.


(cxx-api-serial-block-query)=
### const double* query(const double x, const double y, const double z)

Query for values at a point using bilinear interpolation. 

This low-level function returns all values stored at a point.

* **x[in]** X coordinate of point in model coordinate system.
* **y[in]** Y coordinate of point in model coordinate system.
* **z[in]** Z coordinate of point in model coordinate system.

* **returns** Array of values for model at specified point.

(cxx-api-serial-block-closeQuery)=
### closeQuery()

(cxx-api-serial-block-compare)=
### bool compare(const Block* a, const Block* b);

Cleanup after querying.

* **a[in]** Block to compare.
* **b[in]** Block to compare.
* **returns** True if a.z_top > b.z_top, else false.
