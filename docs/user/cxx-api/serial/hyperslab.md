# Serial Hyperslab Object 

**Full name**: geomodelgrids::serial::Hyperslab

## Methods

+ [Hyperslab(geomodelgrids::serial::HDF* const h5, const char* path, const hsize_t dims\[\], const size_t ndims)](cxx-api-serial-hyperslab-Hyperslab)
+ [interpolate(double* const values, const double indexFloat\[\])](cxx-api-serial-hyperslab-interpolate)

(cxx-api-serial-hyperslab-Hyperslab)=
### Hyperslab(geomodelgrids::serial::HDF* const h5, const char* path, const hsize_t dims\[\], const size_t ndims)

Constructor.

* **h5**[in] HDF5 object with model.
* **path**[in] Full path to dataset.
* **dims**[in] Array of hyperslab dimensions.
* **ndims**[in] Number of dimensions of hyperslab (should match number of dimensions of dataset).

(cxx-api-serial-hyperslab-interpolate)=
### interpolate(double* const values, const double indexFloat\[\])

Compute values at point using bilinear interpolation.

* **values**[out] Preallocated array for interpolated values.
* **indexFloat**[in] Index of target point as floating point values.
