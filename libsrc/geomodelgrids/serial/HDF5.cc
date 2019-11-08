#include <portinfo>

#include "HDF5.hh" // implementation of class methods

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::HDF5::HDF5(void) :
    _file(-1)
{} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::HDF5::~HDF5(void) {
    close();
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Open HDF5 file.
void
geomodelgrids::serial::HDF5::open(const char* filename,
                                  hid_t mode) {
    assert(filename);

    if (_file >= 0) {
        throw std::runtime_error("HDF5 file already open.");
    } // if

    if (hid_t(H5F_ACC_TRUNC) == mode) {
        _file = H5Fcreate(filename, mode, H5P_DEFAULT, H5P_DEFAULT);
        if (_file < 0) {
            std::ostringstream msg;
            msg << "Could not create HDF5 file '" << filename << "'.";
            throw std::runtime_error(msg.str());
        } // if

    } else {
        _file = H5Fopen(filename, mode, H5P_DEFAULT);
        if (_file < 0) {
            std::ostringstream msg;
            msg << "Could not open existing HDF5 file '" << filename << "'.";
            throw std::runtime_error(msg.str());
        } // if
    } // if/else
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Close HDF5 file.
void
geomodelgrids::serial::HDF5::close(void) {
    if (_file >= 0) {
        herr_t err = H5Fclose(_file);
        if (err < 0) {
            throw std::runtime_error("Could not close HDF5 file.");
        }
    } // if
    _file = -1;
} // close


// ---------------------------------------------------------------------------------------------------------------------
// Check if HDF5 file is open.
bool
geomodelgrids::serial::HDF5::isOpen(void) const {
    return (_file == -1) ? false : true;
} // isOpen


// ---------------------------------------------------------------------------------------------------------------------
// Check if HDF5 file has group.
bool
geomodelgrids::serial::HDF5::hasGroup(const char* name) {
    assert(isOpen());
    assert(name);

    bool exists = false;
    if (H5Lexists(_file, name, H5P_DEFAULT)) {
        hid_t obj = H5Oopen(_file, name, H5P_DEFAULT);
        assert(obj >= 0);
        H5O_info_t info;
        herr_t err = H5Oget_info(obj, &info);
        assert(err >= 0);
        if (H5O_TYPE_GROUP == info.type) {
            exists = true;
        }
        err = H5Oclose(obj);
        assert(err >= 0);
    } // if

    return exists;
} // hasGroup


// ---------------------------------------------------------------------------------------------------------------------
// Check if HDF5 file has dataset.
bool
geomodelgrids::serial::HDF5::hasDataset(const char* name) {
    assert(isOpen());
    assert(name);

    bool exists = false;
    if (H5Lexists(_file, name, H5P_DEFAULT)) {
        hid_t obj = H5Oopen(_file, name, H5P_DEFAULT);
        assert(obj >= 0);
        H5O_info_t info;
        herr_t err = H5Oget_info(obj, &info);
        assert(err >= 0);
        if (H5O_TYPE_DATASET == info.type) {
            exists = true;
        }
        err = H5Oclose(obj);
        assert(err >= 0);
    } // if

    return exists;
} // hasDataset


// ---------------------------------------------------------------------------------------------------------------------
// Get topology metadata.
void
geomodelgrids::serial::HDF5::getDatasetDims(hsize_t** dims,
                                            int* ndims,
                                            const char* path) {
    assert(dims);
    assert(ndims);
    assert(path);
    assert(isOpen());

    try {
        // Open the dataset
        hid_t dataset = H5Dopen2(_file, path, H5P_DEFAULT);
        if (dataset < 0) { throw std::runtime_error("Could not open dataset."); }

        hid_t dataspace = H5Dget_space(dataset);
        if (dataspace < 0) { throw std::runtime_error("Could not get dataspace."); }

        *ndims = H5Sget_simple_extent_ndims(dataspace);
        delete[] *dims;*dims = (*ndims > 0) ? new hsize_t[*ndims] : 0;
        H5Sget_simple_extent_dims(dataspace, *dims, 0);

        herr_t err = H5Sclose(dataspace);
        if (err < 0) { throw std::runtime_error("Could not close dataspace."); }

        err = H5Dclose(dataset);
        if (err < 0) { throw std::runtime_error("Could not close dataset."); }

    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while reading dataset '"
            << path << "':\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } catch (...) {
        std::ostringstream msg;
        msg << "Unknown error occurred while reading dataset '"
            << path << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
} // getDatasetDims


// ---------------------------------------------------------------------------------------------------------------------
// Get names of datasets in group.
void
geomodelgrids::serial::HDF5::getGroupDatasets(std::vector<std::string>* names,
                                              const char* path) {
    assert(names);
    assert(isOpen());

    try {
        // Open group
        hid_t group = H5Gopen2(_file, path, H5P_DEFAULT);
        if (group < 0) {
            throw std::runtime_error("Could not open group.");
        }

        H5G_info_t ginfo;
        herr_t err = H5Gget_info(group, &ginfo);
        if (err < 0) {
            throw std::runtime_error("Could not get group info.");
        }
        const int gsize = ginfo.nlinks;

        names->resize(gsize);
        for (int i = 0, index = 0; i < gsize; ++i) {
            char buffer[256];
            ssize_t namelen = H5Lget_name_by_idx(group, ".", H5_INDEX_NAME, H5_ITER_NATIVE, i, buffer, 256, H5P_DEFAULT);assert(namelen > 0);
            (*names)[index++] = buffer;
        } // for

        err = H5Gclose(group);
        if (err < 0) {
            throw std::runtime_error("Could not close group.");
        }

    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while getting names of datasets for group '"
            << path << "':\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } catch (...) {
        std::ostringstream msg;
        msg << "Unknown error occurred while getting names of datasets for group '"
            << path << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
} // getGroupDatasets


// ---------------------------------------------------------------------------------------------------------------------
// Read scalar attribute.
void
geomodelgrids::serial::HDF5::readAttribute(const char* path,
                                           const char* name,
                                           hid_t datatype,
                                           void* value) {
    assert(path);
    assert(name);
    assert(value);

    try {
        hid_t attribute = H5Aopen_by_name(_file, path, name, H5P_DEFAULT, H5P_DEFAULT);
        if (attribute < 0) { throw std::runtime_error("Could not open"); }

        hid_t dtype = H5Aget_type(attribute);
        if (dtype < 0) { throw std::runtime_error("Could not get datatype of"); }

        hid_t err = H5Aread(attribute, dtype, value);
        if (err < 0) { throw std::runtime_error("Could not read"); }

        err = H5Tclose(dtype);
        if (err < 0) { throw std::runtime_error("Could not close datatype for"); }

        err = H5Aclose(attribute);
        if (err < 0) { throw std::runtime_error("Could not close"); }

    } catch (std::exception& err) {
        std::ostringstream msg;
        msg << err.what() << " attribute '" << name << "' of '" << path << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
} // readAttribute


// ---------------------------------------------------------------------------------------------------------------------
// Read string attribute.
std::string
geomodelgrids::serial::HDF5::readAttribute(const char* path,
                                           const char* name) {
    assert(path);
    assert(name);

    std::string value;

    try {
        hid_t attribute = H5Aopen_by_name(_file, path, name, H5P_DEFAULT, H5P_DEFAULT);
        if (attribute < 0) { throw std::runtime_error("Could not open"); }

        hid_t datatype = H5Aget_type(attribute);
        if (datatype < 0) { throw std::runtime_error("Could not get datatype of"); }
        assert(H5Tis_variable_str(datatype) > 0);

        char* buffer = NULL;
        herr_t err = H5Aread(attribute, datatype, (void*)&buffer);
        if (err < 0) { throw std::runtime_error("Could not read"); }
        value = buffer;

        err = H5Tclose(datatype);
        if (err < 0) { throw std::runtime_error("Could not close datatype for"); }

        err = H5Aclose(attribute);
        if (err < 0) { throw std::runtime_error("Could not close"); }

    } catch (std::exception& err) {
        std::ostringstream msg;
        msg << err.what() << " attribute '" << name << "' of '" << path << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
    
    return value;
} // readAttribute


// ---------------------------------------------------------------------------------------------------------------------
// Read strings attribute.
void
geomodelgrids::serial::HDF5::readAttribute(const char* path,
                                           const char* name,
                                           std::vector<std::string>* values) {
    assert(path);
    assert(name);
    assert(values);

    try {
        hid_t attribute = H5Aopen_by_name(_file, path, name, H5P_DEFAULT, H5P_DEFAULT);
        if (attribute < 0) { throw std::runtime_error("Could not open"); }

        hid_t datatype = H5Aget_type(attribute);
        if (datatype < 0) { throw std::runtime_error("Could not get datatype of"); }

        hid_t dataspace = H5Aget_space(attribute);
        const hsize_t ndims = H5Sget_simple_extent_ndims(dataspace);assert(1 == ndims);
        hsize_t dims[1];
        H5Sget_simple_extent_dims(dataspace, dims, NULL);
	const hsize_t numStrings = dims[0];assert(numStrings > 0);

        char** buffer = new char*[numStrings];
        herr_t err = H5Aread(attribute, datatype, (void*)buffer);
        if (err < 0) { throw std::runtime_error("Could not read"); }

        values->resize(numStrings);
        for (size_t i = 0; i < values->size(); ++i) {
            (*values)[i] = buffer[i];
        } // for
        err = H5Dvlen_reclaim(datatype, dataspace, H5P_DEFAULT, buffer);
        delete[] buffer;buffer = NULL;

        err = H5Sclose(dataspace);
        if (err < 0) { throw std::runtime_error("Could not close dataspace for"); }

        err = H5Tclose(datatype);
        if (err < 0) { throw std::runtime_error("Could not close datatype for"); }

        err = H5Aclose(attribute);
        if (err < 0) { throw std::runtime_error("Could not close"); }

    } catch (std::exception& err) {
        std::ostringstream msg;
        msg << err.what() << " attribute '" << name << "' of '" << path << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
} // readAttribute


// ---------------------------------------------------------------------------------------------------------------------
// Read dataset slice.
void
geomodelgrids::serial::HDF5::readDatasetChunk(const char* parent,
                                              const char* name,
                                              char** const data,
                                              hsize_t** const dimsChunk,
                                              int* const ndims,
                                              const int chunk,
                                              hid_t datatype) {
    assert(parent);
    assert(name);
    assert(data);
    assert(dimsChunk);
    assert(_file > 0);

    try {
        // Open group
        hid_t group = H5Gopen2(_file, parent, H5P_DEFAULT);
        if (group < 0) {
            throw std::runtime_error("Could not open group.");
        }

        // Open the dataset
        hid_t dataset = H5Dopen2(group, name, H5P_DEFAULT);
        if (dataset < 0) {
            throw std::runtime_error("Could not open dataset.");
        }

        hid_t dataspace = H5Dget_space(dataset);
        if (dataspace < 0) {
            throw std::runtime_error("Could not get dataspace.");
        }

        *ndims = H5Sget_simple_extent_ndims(dataspace);
        assert(*ndims > 0);
        hsize_t* dims = (*ndims > 0) ? new hsize_t[*ndims] : 0;
        H5Sget_simple_extent_dims(dataspace, dims, 0);

        // Select hyperslab in file
        delete[] *dimsChunk;*dimsChunk = (*ndims > 0) ? new hsize_t[*ndims] : 0;
        hsize_t* count = (*ndims > 0) ? new hsize_t[*ndims] : 0;
        hsize_t* stride = (*ndims > 0) ? new hsize_t[*ndims] : 0;
        hsize_t* offset = (*ndims > 0) ? new hsize_t[*ndims] : 0;

        for (int i = 0; i < *ndims; ++i) {
            (*dimsChunk)[i] = dims[i];
            count[i] = 1;
            stride[i] = 1;
            offset[i] = 0;
        } // for
        (*dimsChunk)[0] = 1;
        offset[0] = chunk;

        hid_t chunkspace = H5Screate_simple(*ndims, *dimsChunk, 0);
        if (chunkspace < 0) {
            throw std::runtime_error("Could not create chunk dataspace.");
        }

        herr_t err = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET,
                                         offset, stride, count, *dimsChunk);
        delete[] count;count = 0;
        delete[] stride;stride = 0;
        delete[] offset;offset = 0;
        if (err < 0) {
            throw std::runtime_error("Could not select hyperslab.");
        }

        int sizeBytes = H5Tget_size(datatype);
        for (int i = 0; i < *ndims; ++i) {
            sizeBytes *= (*dimsChunk)[i];
        }
        delete[] *data;*data = (sizeBytes > 0) ? new char[sizeBytes] : 0;
        delete[] dims;dims = 0;

        err = H5Dread(dataset, datatype, chunkspace, dataspace,
                      H5P_DEFAULT, (void*)*data);
        if (err < 0) {
            throw std::runtime_error("Could not read data.");
        }

        err = H5Sclose(chunkspace);
        if (err < 0) {
            throw std::runtime_error("Could not close chunk dataspace.");
        }

        err = H5Sclose(dataspace);
        if (err < 0) {
            throw std::runtime_error("Could not close dataspace.");
        }

        err = H5Dclose(dataset);
        if (err < 0) {
            throw std::runtime_error("Could not close dataset.");
        }

        err = H5Gclose(group);
        if (err < 0) {
            throw std::runtime_error("Could not close group.");
        }

    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while reading dataset '"
            << parent << "/" << name << "':\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } catch (...) {
        std::ostringstream msg;
        msg << "Unknown error occurred while reading dataset '"
            << parent << "/" << name << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
} // readDatasetChunk


// End of file
