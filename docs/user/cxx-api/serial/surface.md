(cxx-api-serial-surface)=
# Surface

**Full name**: geomodelgrids::serial::Surface

## Methods

### Surface(const char* const name)

Constructor.

- **name**[in] Name of surface.

### loadMetadata(geomodelgrids::serial::HDF5* const h5)

Load metadata from the model file.

### double getResolutionX()

Get horizontal resolution along x axis. Only valid (nonzero) for uniform resolution.

- **returns** Resolution along x axis.

### double getResolutionY()

Get horizontal resolution along y axis. Only valid (nonzero) for uniform resolution.

- **returns** Resolution along y axis.

### double* getCoordinatesX(void)

Get coordinates along x axis.

- **returns** Array of coordinates along x axis. Only valid (non-null) if variable resolution.

### double* getCoordinatesY(void)

Get coordinates along y axis.

- **returns** Array of coordinates along y axis. Only valid (non-null) if variable resolution.

### const size_t* getDims()

Get number of values along each grid dimension.

- **returns** Array of values along each grid dimension.

### setHyperslabDims(const size_t dims[], const size_t ndims)

Set hyperslab size.

- **dims**[in] Dimensions of hyperslab.
- **ndims**[in] Number of dimensions.

### openQuery(geomodelgrids::serial::HDF5* const h5)

Prepare for querying.

- **h5**[in] HDF5 object with model.

### closeQuery()

Cleanup after querying.

### double query(const double x, const double y)

Query for elevation of ground surface at a point using bilinear interpolation. 

- **x[in]** X coordinate of point in model coordinate system.
- **y[in]** Y coordinate of point in model coordinate system.

- **returns** Elevation of ground surface at point.
