# Serial Topography Object 

**Full name**: geomodelgrids::serial::Topography

## Methods

* [Topography()](#topography)
* [loadMetadata()](#loadmetadatageomodelgridsserialhdf5-const-h5)
* [getResolutionHoriz()](#double-getresolutionhoriz)
* [getDims()](#const-size_t-getdims)
* [setHyperslabDims(const size_t dims\[\], const size_t ndims)](#sethyperslabdimsconst-size_t-dims-const-size_t-ndims)
* [openQuery(geomodelgrids::serial::HDF5* const h5)](#openquerygeomodelgridsserialhdf5-const-h5)
* [closeQuery()](#closequery)
* [query(const double x, const double y)](#double-queryconst-double-x-const-double-y)


### Topography()

Constructor.

### loadMetadata(geomodelgrids::serial::HDF5* const h5)

Load metadata from the model file.


### double getResolutionHoriz()

Get horizontal resolution of topography grid.

* **returns** Horizontal resolution (m).


### const size_t* getDims()

Get number of values along each grid dimension.

* **returns** Array of values along each grid dimension.


### setHyperslabDims(const size_t dims[], const size_t ndims)

Set hyperslab size.

* **dims**[in] Dimensions of hyperslab.
* **ndims**[in] Number of dimensions.

### openQuery(geomodelgrids::serial::HDF5* const h5)

Prepare for querying.

* **h5**[in] HDF5 object with model.


### closeQuery()

Cleanup after querying.


### double query(const double x, const double y)

Query for elevation (m) of ground surface at a point using bilinear interpolation. 

* **x[in]** X coordinate of point in model coordinate system.
* **y[in]** Y coordinate of point in model coordinate system.

* **returns** Eelvation (m) of ground surface at point.
