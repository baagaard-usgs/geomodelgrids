# Serial HDF5 Object 

**Full name**: geomodelgrids::serial::HDF5

## Methods

* [HDF5()](#hdf5)
* [open(const char* filename, hid_t mode)](#openconst-char-filename-hid_t-mode)
* [close()](#close)
* [isOpen()](#bool-isopen)
* [hasGroup(const char* name)](#bool-hasgroupconst-char-name)
* [hasDataset(const char* name)](#bool-hasdatasetconst-char-name)
* [getDatasetDims(hsize_t** dims, int* ndims, const char* path)](#getdatasetdimshsize_t-dims-int-ndims-const-char-path)
* [getGroupDatasets(std::vector\<std::string\>* names, const char* parent)](#getgroupdatasetsstdvectorstdstring-names-const-char-parent)
* [readAttribute(const char* path, const char* name, hid_t datatype, void* value)](#readattributeconst-char-path-const-char-name-hid_t-datatype-void-value)
* [readAttribute(const char* path, const char* name)](#stdstring-readattributeconst-char-path-const-char-name)
* [readAttribute(const char* path, const char* name, std::vector\<std::string\>* values)](#readattributeconst-char-path-const-char-name-stdvectorstdstring-values)
* [readDatasetHyperslab(void* values, const char* path, const hsize_t* const origin, const hsize_t* const dims, int ndims, hid_t datatype)](#readdatasethyperslabvoid-values-const-char-path-const-hsize_t-const-origin-const-hsize_t-const-dims-int-ndims-hid_t-datatype)


### HDF5()

Constructor.


### open(const char* filename, hid_t mode)

Open HDF5 file.

* **filename**[in] Name of HDF5 file.
* **mode**[in] Mode for HDF5 file.


### close()

Close HDF5 file.


### bool isOpen()

Check if HDF5 file is open.


### bool hasGroup(const char* name)

Check if HDF5 file has group.

* **name**[in] Full path of group.
* **returns** True if group exists, false otherwise.


### bool hasDataset(const char* name)

Check if HDF5 file has dataset.

* **name**[in] Full path of dataset.
* **returns** True if dataset exists, false otherwise.


### getDatasetDims(hsize_t** dims, int* ndims, const char* path)

Get dimensions of dataset.

* **dims**[out] Array of dimensions.
* **ndims**[out] Number of dimensions.
* **path**[in] Full path of dataset.


### getGroupDatasets(std::vector\<std::string\>* names, const char* parent)

Get names of datasets in group.

* **names**[out] Array of dataset names.
* **group**[in] Full path of group.


### readAttribute(const char* path, const char* name, hid_t datatype, void* value)

Read scalar attribute.

* **path**[in] Full path to object with attribute.
* **name**[in] Name of attribute.
* **datastype**[in] Datatype of scalar.
* **value**[out] Attribute value.


### std::string readAttribute(const char* path, const char* name)

Read fixed or variable length string attribute.

* **path**[in] Full path to object with attribute.
* **path**[in] name Name of attribute.


### readAttribute(const char* path, const char* name, std::vector\<std::string\>* values)

Read array of fixed or variable length strings attribute.

* **path**[in] Full path to object with attribute.
* **name**[in] Name of attribute.
* **values**[out] Array of strings.

### readDatasetHyperslab(void* values, const char* path, const hsize_t* const origin, const hsize_t* const dims, int ndims, hid_t datatype)

Read hyperslab (subset of values) from dataset.

* **values**[out] Values of hyperslab.
* **path**[in] Full path to dataset.
* **orogin**[in] Origin of hyperslab in dataset.
* **dims**[in] Dimensions of hyperslab.
* **ndims**[in] Number of dimensions of hyperslab.
* **datatype**[in] Type of data in dataset.
