(cxx-api-serial-hdf5)=
# HDF5

**Full name**: geomodelgrids::serial::HDF5

## Methods

### HDF5()

Constructor.

### setCache(const size_t cacheSize, const size_t nslots, const double preemption)

Must be called BEFORE open().

The cache should be large enough to fit at least as many chunks as there are in a hyperslab. HDF5 uses a default cache size of 1 MB. We use a default of 16 MB.

The number of slots should be a prime number at least 10 times the number of chunks that can fit into the cache; usually 100 times that number of chunks provides maximum performance. HDF5 uses a default of 521. We use a default of 63997.

Chunk preemption policy for this dataset; value between 0 and 1 (default is 0.75).

See [H5Pset_cache](https://portal.hdfgroup.org/display/HDF5/H5P_SET_CACHE) for more information.

- **cacheSize**[in] Dataset cache size in bytes.
- **nslots**[in] Number of chunk slots.
- **preemption**[in] Preemption policy value.

### open(const char* filename, hid_t mode)

Open HDF5 file.

- **filename**[in] Name of HDF5 file.
- **mode**[in] Mode for HDF5 file.

### close()

Close HDF5 file.

### bool isOpen()

Check if HDF5 file is open.

### bool hasGroup(const char* name)

Check if HDF5 file has group.

- **name**[in] Full path of group.
- **returns** True if group exists, false otherwise.

### bool hasDataset(const char* name)

Check if HDF5 file has dataset.

- **name**[in] Full path of dataset.
- **returns** True if dataset exists, false otherwise.

### getDatasetDims(hsize_t** dims, int* ndims, const char* path)

Get dimensions of dataset.

- **dims**[out] Array of dimensions.
- **ndims**[out] Number of dimensions.
- **path**[in] Full path of dataset.

### getGroupDatasets(std::vector\<std::string\>* names, const char* parent)

Get names of datasets in group.

- **names**[out] Array of dataset names.
- **group**[in] Full path of group.

### readAttribute(const char* path, const char* name, hid_t datatype, void* value)

Read scalar attribute.

- **path**[in] Full path to object with attribute.
- **name**[in] Name of attribute.
- **datastype**[in] Datatype of scalar.
- **value**[out] Attribute value.

### readAttribute(const char* path, const char* name, hid_t datatype, void** value, size_t* valuesSize)

Read array attribute.

- **path[in]** Full path to object with attribute.
- **name[in]** Name of attribute.
- **datatype[in]** Datatype of array.
- **values[out]** Attribute value.

### std::string readAttribute(const char* path, const char* name)

Read fixed or variable length string attribute.

- **path**[in] Full path to object with attribute.
- **path**[in] name Name of attribute.

### readAttribute(const char* path, const char* name, std::vector\<std::string\>* values)

Read array of fixed or variable length strings attribute.

- **path**[in] Full path to object with attribute.
- **name**[in] Name of attribute.
- **values**[out] Array of strings.

### readDatasetHyperslab(void* values, const char* path, const hsize_t* const origin, const hsize_t* const dims, int ndims, hid_t datatype)

Read hyperslab (subset of values) from dataset.

- **values**[out] Values of hyperslab.
- **path**[in] Full path to dataset.
- **orogin**[in] Origin of hyperslab in dataset.
- **dims**[in] Dimensions of hyperslab.
- **ndims**[in] Number of dimensions of hyperslab.
- **datatype**[in] Type of data in dataset.
