# Serial Hyperslab Object 

**Full name**: geomodelgrids::serial::Hyperslab

## Methods

* [Hyperslab(geomodelgrids::serial::HDF* const h5, const char* path, const hsize_t dims\[\], const size_t ndims)](#hyperslabgeomodelgridsserialhdf-const-h5-const-char-path-const-hsize_t-dims-const-size_t-ndims)
* [interpolate(double* const values, const double indexFloat\[\])](#interpolatedouble-const-values-const-double-indexfloat)


### Hyperslab(geomodelgrids::serial::HDF* const h5, const char* path, const hsize_t dims\[\], const size_t ndims)

Constructor.

* **h5**[in] HDF5 object with model.
* **path**[in] Full path to dataset.
* **dims**[in] Array of hyperslab dimensions.
* **ndims**[in] Number of dimensions of hyperslab (should match number of dimensions of dataset).


### interpolate(double* const values, const double indexFloat\[\])

Compute values at point using bilinear interpolation.

* **values**[out] Preallocated array for interpolated values.
* **indexFloat**[in] Index of target point as floating point values.
