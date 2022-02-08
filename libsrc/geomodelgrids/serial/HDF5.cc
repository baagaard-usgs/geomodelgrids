#include <portinfo>

#include "HDF5.hh" // implementation of class methods

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

#if H5_VERSION_GE(1,12,0)
#define GEOMODELGRIDS_HDF5_USE_API_112
#else
#if H5_VERSION_GE(1,8,0)
#define GEOMODELGRIDS_HDF5_USE_API_18
#endif
#endif

const hid_t geomodelgrids::serial::HDF5::H5_NULL = -1;

// ------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace serial {
        struct _HDF5Access;
    } // serial
} // geomodelgrids
struct geomodelgrids::serial::_HDF5Access {
    hid_t object;
    hid_t group;
    hid_t dataset;
    hid_t dataspace;
    hid_t attribute;
    hid_t datatype;

    _HDF5Access(void) :
        object(HDF5::H5_NULL),
        group(HDF5::H5_NULL),
        dataset(HDF5::H5_NULL),
        dataspace(HDF5::H5_NULL),
        attribute(HDF5::H5_NULL),
        datatype(HDF5::H5_NULL) {}


    ~_HDF5Access(void) {
        if (object >= 0) { H5Oclose(object); }
        if (group >= 0) { H5Gclose(group); }
        if (dataset >= 0) { H5Dclose(dataset); }
        if (dataspace >= 0) { H5Sclose(dataspace); }
        if (attribute >= 0) { H5Aclose(attribute); }
        if (datatype >= 0) { H5Tclose(datatype); }
    } // destructor

};

// ------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::HDF5::HDF5(void) :
    _file(H5_NULL),
    _cacheSize(128*1048576),
    _cacheNumSlots(63997),
    _cachePreemption(0.75) {}


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::HDF5::~HDF5(void) {
    close();
} // destructor


// ------------------------------------------------------------------------------------------------
// Set chunk caching parameters.
void
geomodelgrids::serial::HDF5::setCache(const size_t cacheSize,
                                      const size_t nslots,
                                      const double preemption) {
    _cacheSize = cacheSize;
    _cacheNumSlots = nslots;
    _cachePreemption = std::max(0.0, std::min(1.0, preemption));
} // setDatasetCache


// ------------------------------------------------------------------------------------------------
// Open HDF5 file.
void
geomodelgrids::serial::HDF5::open(const char* filename,
                                  hid_t mode) {
    assert(filename);

    if (_file >= 0) {
        throw std::runtime_error("HDF5 file already open.");
    } // if

    hid_t fileAccess = H5Pcreate(H5P_FILE_ACCESS);
    if (fileAccess < 0) { throw std::runtime_error("Could not create property for HDF5 cache parameters."); }
    herr_t err = H5Pset_cache(fileAccess, 0, _cacheNumSlots, _cacheSize, _cachePreemption);
    if (err < 0) { throw std::runtime_error("Could not set HDF5 file cache properties."); }

    if (hid_t(H5F_ACC_TRUNC) == mode) {
        _file = H5Fcreate(filename, mode, H5P_DEFAULT, fileAccess);
        if (_file < 0) {
            std::ostringstream msg;
            msg << "Could not create HDF5 file '" << filename << "'.";
            throw std::runtime_error(msg.str());
        } // if

    } else {
        _file = H5Fopen(filename, mode, fileAccess);
        if (_file < 0) {
            std::ostringstream msg;
            msg << "Could not open existing HDF5 file '" << filename << "'.";
            throw std::runtime_error(msg.str());
        } // if
    } // if/else

    H5Pclose(fileAccess);
} // constructor


// ------------------------------------------------------------------------------------------------
// Close HDF5 file.
void
geomodelgrids::serial::HDF5::close(void) {
    if (_file >= 0) {
        herr_t err = H5Fclose(_file);
        if (err < 0) {
            throw std::runtime_error("Could not close HDF5 file.");
        } // if
    } // if
    _file = H5_NULL;
} // close


// ------------------------------------------------------------------------------------------------
// Check if HDF5 file is open.
bool
geomodelgrids::serial::HDF5::isOpen(void) const {
    return (_file != H5_NULL);
} // isOpen


// ------------------------------------------------------------------------------------------------
// Check if HDF5 file has group.
bool
geomodelgrids::serial::HDF5::hasGroup(const char* name) {
    assert(isOpen());
    assert(name);

    bool exists = false;
    if (H5Lexists(_file, name, H5P_DEFAULT)) {
        _HDF5Access h5access;
        h5access.object = H5Oopen(_file, name, H5P_DEFAULT);assert(h5access.object >= 0);
        H5O_info_t info;
#if defined(GEOMODELGRIDS_HDF5_USE_API_112)
        herr_t err = H5Oget_info(h5access.object, &info, H5O_INFO_ALL);
#else
        herr_t err = H5Oget_info(h5access.object, &info);
#endif
        assert(err >= 0);
        if (H5O_TYPE_GROUP == info.type) {
            exists = true;
        } // if
    } // if

    return exists;
} // hasGroup


// ------------------------------------------------------------------------------------------------
// Check if HDF5 file has dataset.
bool
geomodelgrids::serial::HDF5::hasDataset(const char* name) {
    assert(isOpen());
    assert(name);

    bool exists = false;
    if (H5Lexists(_file, name, H5P_DEFAULT)) {
        _HDF5Access h5access;
        h5access.object = H5Oopen(_file, name, H5P_DEFAULT);assert(h5access.object >= 0);
        H5O_info_t info;
#if defined(GEOMODELGRIDS_HDF5_USE_API_112)
        herr_t err = H5Oget_info(h5access.object, &info, H5O_INFO_ALL);
#else
        herr_t err = H5Oget_info(h5access.object, &info);
#endif
        assert(err >= 0);
        if (H5O_TYPE_DATASET == info.type) {
            exists = true;
        } // if
    } // if

    return exists;
} // hasDataset


// ------------------------------------------------------------------------------------------------
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
        _HDF5Access h5access;

        // Open the dataset
        h5access.dataset = H5Dopen2(_file, path, H5P_DEFAULT);
        if (h5access.dataset < 0) { throw std::runtime_error("Could not open dataset."); }

        h5access.dataspace = H5Dget_space(h5access.dataset);
        if (h5access.dataspace < 0) { throw std::runtime_error("Could not get dataspace."); }

        *ndims = H5Sget_simple_extent_ndims(h5access.dataspace);
        delete[] *dims;*dims = (*ndims > 0) ? new hsize_t[*ndims] : 0;
        H5Sget_simple_extent_dims(h5access.dataspace, *dims, 0);

    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while reading dataset '"
            << path << "':\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } // try/catch
} // getDatasetDims


// ------------------------------------------------------------------------------------------------
// Get names of datasets in group.
void
geomodelgrids::serial::HDF5::getGroupDatasets(std::vector<std::string>* names,
                                              const char* path) {
    assert(names);
    assert(isOpen());

    try {
        _HDF5Access h5access;

        // Open group
        h5access.group = H5Gopen2(_file, path, H5P_DEFAULT);
        if (h5access.group < 0) {
            throw std::runtime_error("Could not open group.");
        } // if

        H5G_info_t ginfo;
        herr_t err = H5Gget_info(h5access.group, &ginfo);
        if (err < 0) {
            throw std::runtime_error("Could not get group info.");
        } // if
        const int gsize = ginfo.nlinks;

        names->resize(gsize);
        for (int i = 0, index = 0; i < gsize; ++i) {
            char buffer[256];
            ssize_t namelen = H5Lget_name_by_idx(h5access.group, ".", H5_INDEX_NAME, H5_ITER_NATIVE, i,
                                                 buffer, 256, H5P_DEFAULT);assert(namelen > 0);
            (*names)[index++] = buffer;
        } // for

    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while getting names of datasets for group '"
            << path << "':\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } // try/catch
} // getGroupDatasets


// ------------------------------------------------------------------------------------------------
// Check if HDF5 file has attribute.
bool
geomodelgrids::serial::HDF5::hasAttribute(const char* path,
                                          const char* name) {
    assert(path);
    assert(name);

    htri_t exists = H5Aexists_by_name(_file, path, name, H5P_DEFAULT);
    return exists > 0;
} // hasAttribute


// ------------------------------------------------------------------------------------------------
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
        _HDF5Access h5access;

        h5access.attribute = H5Aopen_by_name(_file, path, name, H5P_DEFAULT, H5P_DEFAULT);
        if (h5access.attribute < 0) { throw std::runtime_error("Could not open"); }

        hid_t err = H5Aread(h5access.attribute, datatype, value);
        if (err < 0) { throw std::runtime_error("Could not read"); }

    } catch (std::exception& err) {
        std::ostringstream msg;
        msg << err.what() << " attribute '" << name << "' of '" << path << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
} // readAttribute


// ------------------------------------------------------------------------------------------------
// Read vector attribute.
void
geomodelgrids::serial::HDF5::readAttribute(const char* path,
                                           const char* name,
                                           hid_t datatype,
                                           void** values,
                                           size_t* valuesSize) {
    assert(path);
    assert(name);
    assert(values);
    assert(valuesSize);

    try {
        _HDF5Access h5access;

        h5access.attribute = H5Aopen_by_name(_file, path, name, H5P_DEFAULT, H5P_DEFAULT);
        if (h5access.attribute < 0) { throw std::runtime_error("Could not open"); }

        h5access.dataspace = H5Aget_space(h5access.attribute);
        if (h5access.dataspace < 0) { throw std::runtime_error("Could not get dataspace of"); }
        const hsize_t ndims = H5Sget_simple_extent_ndims(h5access.dataspace);assert(1 == ndims);
        hsize_t dims[1];
        H5Sget_simple_extent_dims(h5access.dataspace, dims, NULL);
        const hsize_t numValues = dims[0];assert(numValues > 0);

        hsize_t typeNumBytes = H5Tget_size(datatype);
        void* buffer = (void*) new char[numValues * typeNumBytes];

        herr_t err = H5Aread(h5access.attribute, datatype, buffer);
        if (err < 0) { throw std::runtime_error("Could not read"); }

        *valuesSize = numValues;
        *values = buffer;
    } catch (std::exception& err) {
        std::ostringstream msg;
        msg << err.what() << " attribute '" << name << "' of '" << path << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch

} // readAttribute


// ------------------------------------------------------------------------------------------------
// Read string attribute.
std::string
geomodelgrids::serial::HDF5::readAttribute(const char* path,
                                           const char* name) {
    assert(path);
    assert(name);

    std::string value;

    try {
        _HDF5Access h5access;

        h5access.attribute = H5Aopen_by_name(_file, path, name, H5P_DEFAULT, H5P_DEFAULT);
        if (h5access.attribute < 0) { throw std::runtime_error("Could not open"); }

        h5access.datatype = H5Aget_type(h5access.attribute);
        if (h5access.datatype < 0) { throw std::runtime_error("Could not get datatype of"); }

        h5access.dataspace = H5Aget_space(h5access.attribute);
        if (h5access.dataspace < 0) { throw std::runtime_error("Could not get dataspace of"); }

        char* buffer = NULL;
        if (0 == H5Tis_variable_str(h5access.datatype)) { // Fixed length strings
            const hsize_t stringLength = H5Tget_size(h5access.datatype);
            buffer = new char[stringLength+1];assert(buffer);
            buffer[stringLength] = '\0';

            herr_t err = H5Aread(h5access.attribute, h5access.datatype, buffer);
            if (err < 0) { throw std::runtime_error("Could not read"); }
            value = buffer;

            delete[] buffer;buffer = NULL;
        } else {
            herr_t err = H5Aread(h5access.attribute, h5access.datatype, &buffer);
            if (err < 0) { throw std::runtime_error("Could not read"); }
            value = buffer;

            err = H5Dvlen_reclaim(h5access.datatype, h5access.dataspace, H5P_DEFAULT, &buffer);
            if (err < 0) { throw std::runtime_error("Could not reclaim variable length string for"); }
        } // if/else

    } catch (std::exception& err) {
        std::ostringstream msg;
        msg << err.what() << " attribute '" << name << "' of '" << path << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch

    return value;
} // readAttribute


// ------------------------------------------------------------------------------------------------
// Read strings attribute.
void
geomodelgrids::serial::HDF5::readAttribute(const char* path,
                                           const char* name,
                                           std::vector<std::string>* values) {
    assert(path);
    assert(name);
    assert(values);

    try {
        _HDF5Access h5access;

        h5access.attribute = H5Aopen_by_name(_file, path, name, H5P_DEFAULT, H5P_DEFAULT);
        if (h5access.attribute < 0) { throw std::runtime_error("Could not open"); }

        h5access.datatype = H5Aget_type(h5access.attribute);
        if (h5access.datatype < 0) { throw std::runtime_error("Could not get datatype of"); }

        h5access.dataspace = H5Aget_space(h5access.attribute);
        if (h5access.dataspace < 0) { throw std::runtime_error("Could not get dataspace of"); }
        const hsize_t ndims = H5Sget_simple_extent_ndims(h5access.dataspace);assert(1 == ndims);
        hsize_t dims[1];
        H5Sget_simple_extent_dims(h5access.dataspace, dims, NULL);
        const hsize_t numStrings = dims[0];assert(numStrings > 0);

        char** buffer = new char*[numStrings];
        if (0 == H5Tis_variable_str(h5access.datatype)) { // Fixed length strings
            hsize_t stringLength = H5Tget_size(h5access.datatype);
            buffer[0] = new char[numStrings * stringLength];assert(buffer[0]);
            for (size_t i = 1; i < numStrings; ++i) {
                buffer[i] = buffer[0] + i*stringLength;
            } // for

            herr_t err = H5Aread(h5access.attribute, h5access.datatype, buffer[0]);
            if (err < 0) { throw std::runtime_error("Could not read"); }

            values->resize(numStrings);
            for (size_t i = 0; i < values->size(); ++i) {
                char* tmp = new char[stringLength+1];
                for (hsize_t p = 0; p < stringLength; ++p) {
                    tmp[p] = buffer[i][p];
                } // for
                tmp[stringLength] = '\0';
                (*values)[i] = tmp;
                delete[] tmp;tmp = NULL;
            } // for

            delete[] buffer[0];buffer[0] = NULL;
        } else {
            herr_t err = H5Aread(h5access.attribute, h5access.datatype, buffer);
            if (err < 0) { throw std::runtime_error("Could not read"); }

            values->resize(numStrings);
            for (size_t i = 0; i < values->size(); ++i) {
                (*values)[i] = buffer[i];
            } // for

            err = H5Dvlen_reclaim(h5access.datatype, h5access.dataspace, H5P_DEFAULT, buffer);
            if (err < 0) { throw std::runtime_error("Could not reclaim variable length string for"); }
        } // if/else

        delete[] buffer;buffer = NULL;

    } catch (std::exception& err) {
        std::ostringstream msg;
        msg << err.what() << " attribute '" << name << "' of '" << path << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
} // readAttribute


// ------------------------------------------------------------------------------------------------
// Read dataset slice.
void
geomodelgrids::serial::HDF5::readDatasetHyperslab(void* values,
                                                  const char* path,
                                                  const hsize_t* const origin,
                                                  const hsize_t* const dims,
                                                  const int ndims,
                                                  hid_t datatype) {
    assert(values);
    assert(path);
    assert(origin);
    assert(dims);
    assert(_file > 0);

    try {
        _HDF5Access h5access;

        // Open the dataset
        h5access.dataset = H5Dopen2(_file, path, H5P_DEFAULT);
        if (h5access.dataset < 0) { throw std::runtime_error("Could not open dataset."); }

        h5access.dataspace = H5Dget_space(h5access.dataset);
        if (h5access.dataspace < 0) { throw std::runtime_error("Could not get dataspace."); }

        const int ndimsAll = H5Sget_simple_extent_ndims(h5access.dataspace);
        hsize_t* dimsAll = (ndimsAll > 0) ? new hsize_t[ndimsAll] : NULL;
        H5Sget_simple_extent_dims(h5access.dataspace, dimsAll, NULL);

        // Validate arguments.
        if (ndims != ndimsAll) {
            std::ostringstream msg;
            msg << "Rank of hyperslab origin and dimension (" << ndims
                << ") does not match rank of dataset (" << ndimsAll << ").";
            delete[] dimsAll;dimsAll = NULL;
            throw std::length_error(msg.str());
        } // if
        for (int i = 0; i < ndimsAll; ++i) {
            if (origin[i] + dims[i] > dimsAll[i]) {
                std::ostringstream msg;
                msg << "Hyperslab extent in dimension " << i
                    << " (origin:" << origin[i] << ", dim: " << dims[i] << ") "
                    << "exceeds dataset dimension " << dimsAll[i] << ".";
                delete[] dimsAll;dimsAll = NULL;
                throw std::length_error(msg.str());
            } // if
        } // for
        delete[] dimsAll;dimsAll = NULL;

        // Stride and count are 1 for contiguous slab.
        hsize_t* stride = (ndimsAll > 0) ? new hsize_t[ndimsAll] : NULL;
        hsize_t* count = (ndimsAll > 0) ? new hsize_t[ndimsAll] : NULL;
        for (int i = 0; i < ndimsAll; ++i) {
            stride[i] = 1;
            count[i] = 1;
        } // for

        hid_t memspace = H5Screate_simple(ndims, dims, dims);
        if (memspace < 0) { throw std::runtime_error("Could not create memory space."); }

        herr_t err = H5Sselect_hyperslab(h5access.dataspace, H5S_SELECT_SET, origin, stride, count, dims);
        delete[] stride;stride = NULL;
        delete[] count;count = NULL;
        if (err < 0) { throw std::runtime_error("Could not select hyperslab."); }
        err = H5Dread(h5access.dataset, datatype, memspace, h5access.dataspace, H5P_DEFAULT, values);
        if (err < 0) { throw std::runtime_error("Could not read hyperslab."); }

        H5Sclose(memspace);memspace = H5_NULL;
    } catch (const std::exception& err) {
        std::ostringstream msg;
        msg << "Error occurred while reading dataset '"
            << path << "':\n"
            << err.what();
        throw std::runtime_error(msg.str());
    } // try/catch
} // readDatasetHyperslab


// End of file
