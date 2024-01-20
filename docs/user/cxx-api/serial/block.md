(cxx-api-serial-block)=
# Block

**Full name**: geomodelgrids::serial::Block

## Methods

### Block(const char* name)

Constructor with name.

- **name**[in] Name of block.

### loadMetadata(geomodelgrids::serial::HDF5* const h5)

Load metadata from the model file.

- **h5**[in] HDF5 object with model.

### const std::string& getName()

Get the name of the block.

- **returns** Name of the block

### double getResolutionX()

Get resolution along x axis. Only valid (nonzero) for uniform resolution.

- **returns** Resolution along x axis.

### double getResolutionY()

Get resolution along y axis. Only valid (nonzero) for uniform resolution.

- **returns** Resolution along y axis.

### double getResolutionZ()

Get resolution along z axis. Only valid (nonzero) for uniform resolution.

- **returns** Resolution along z axis.

### double getZTop()

Get elevation of top of block in logical space.

- **returns** Elevation of top of block.

### double getZBottom()

Get elevation of bottom of block in logical space.

- **returns** Elevation of bottom of block.

### double* getCoordinatesX(void)

Get coordinates along x axis.

- **returns** Array of coordinates along x axis. Only valid (non-null) if variable resolution.

### double* getCoordinatesY(void)

Get coordinates along y axis.

- **returns** Array of coordinates along y axis. Only valid (non-null) if variable resolution.

### double* getCoordinatesZ(void)

Get coordinates along z axis.

- **returns** Array of coordinates along z axis. Only valid (non-null) if variable resolution.

### const size_t getDims()

Get numebr of values along each grid dimension.

- **returns** Number of points along grid in each dimension [x, y, z].

### size_t getNumValues()

Get number of values stored at each grid point.

- **returns** Number of values stored at each grid point.

### setHyperslabDims(const size_t dims\[\], const size_t ndims)

Set hyperslab size.

- **dims**[in] Dimensions of hyperslab.
- **ndims**[in] Number of dimensions.

### openQuery(geomodelgrids::serial::HDF5* const h5)

Prepare for querying.

- **h5** HDF5 object with model.

### const double* query(const double x, const double y, const double z)

Query for values at a point using bilinear interpolation. 

This low-level function returns all values stored at a point.

- **x[in]** X coordinate of point in model coordinate system.
- **y[in]** Y coordinate of point in model coordinate system.
- **z[in]** Z coordinate of point in model coordinate system.

- **returns** Array of values for model at specified point.

### closeQuery()

Cleanup after querying.

### bool compare(const Block* a, const Block* b)

Comparison for ordering blocks by vertical location.

- **a[in]** Block to compare.
- **b[in]** Block to compare.
- **returns** True if a.z_top > b.z_top, else false.
