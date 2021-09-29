# Serial Surface Object 

**Full name**: geomodelgrids::serial::Surface

## Methods

* [Surface()](cxx-api-serial-surface-Surface)
* [loadMetadata()](cxx-api-serial-surface-loadMetadata)
* [getResolutionX()](cxx-api-serial-surface-getResolutionX)
* [getResolutionY()](cxx-api-serial-surface-getResolutionY)
* [getCoordinatesX()](cxx-api-serial-surface-getCoordinatesX)
* [getCoordinatesY()](cxx-api-serial-surface-getCoordinatesY)
* [getDims()](cxx-api-serial-surface-getDims)
* [setHyperslabDims(const size_t dims\[\], const size_t ndims)](cxx-api-serial-surface-setHyperslabDims)
* [openQuery(geomodelgrids::serial::HDF5* const h5)](cxx-api-serial-surface-openQuery)
* [closeQuery()](cxx-api-serial-surface-closeQuery)
* [query(const double x, const double y)](cxx-api-serial-surface-query)

(cxx-api-serial-surface-Surface)=
### Surface(const char* const name)

Constructor.

* **name**[in] Name of surface.

(cxx-api-serial-surface-loadMetadata)=
### loadMetadata(geomodelgrids::serial::HDF5* const h5)

Load metadata from the model file.

(cxx-api-serial-surface-getResolutionX)=
### double getResolutionX()

Get horizontal resolution along x axis. Only valid (nonzero) for uniform resolution.

* **returns** Resolution along x axis.

(cxx-api-serial-surface-getResolutionY)=
### double getResolutionY()

Get horizontal resolution along y axis. Only valid (nonzero) for uniform resolution.

* **returns** Resolution along y axis.

(cxx-api-serial-surface-getCoordinatesX)=
### double* getCoordinatesX(void)

Get coordinates along x axis.

* **returns** Array of coordinates along x axis. Only valid (non-null) if variable resolution.

(cxx-api-serial-surface-getCoordinatesY)=
### double* getCoordinatesY(void)

Get coordinates along y axis.

* **returns** Array of coordinates along y axis. Only valid (non-null) if variable resolution.

(cxx-api-serial-surface-getDims)=
### const size_t* getDims()

Get number of values along each grid dimension.

* **returns** Array of values along each grid dimension.

(cxx-api-serial-surface-setHyperslabDims)=
### setHyperslabDims(const size_t dims[], const size_t ndims)

Set hyperslab size.

* **dims**[in] Dimensions of hyperslab.
* **ndims**[in] Number of dimensions.

(cxx-api-serial-surface-openQuery)=
### openQuery(geomodelgrids::serial::HDF5* const h5)

Prepare for querying.

* **h5**[in] HDF5 object with model.

(cxx-api-serial-surface-closeQuery)=
### closeQuery()

Cleanup after querying.

(cxx-api-serial-surface-query)=
### double query(const double x, const double y)

Query for elevation of ground surface at a point using bilinear interpolation. 

* **x[in]** X coordinate of point in model coordinate system.
* **y[in]** Y coordinate of point in model coordinate system.

* **returns** Elevation of ground surface at point.
