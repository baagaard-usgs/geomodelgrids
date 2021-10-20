#include <portinfo>

#include "Block.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/serial/Hyperslab.hh" // USES Hyperslab
#include "geomodelgrids/utils/Indexing.hh" // USES Indexing

#include <cstring> // USES strlen()
#include <algorithm> // USES std::max()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Block::Block(const char* name) :
    _name(name),
    _hyperslab(NULL),
    _resolutionX(0.0),
    _resolutionY(0.0),
    _resolutionZ(0.0),
    _zTop(0.0),
    _coordinatesX(NULL),
    _coordinatesY(NULL),
    _coordinatesZ(NULL),
    _indexingX(NULL),
    _indexingY(NULL),
    _indexingZ(NULL),
    _values(NULL),
    _numValues(0) {
    _dims[0] = 0;
    _dims[1] = 0;
    _dims[2] = 0;

    _hyperslabDims[0] = 64;
    _hyperslabDims[1] = 64;
    _hyperslabDims[2] = 0;
    _hyperslabDims[3] = 0;

} // constructor


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Block::~Block(void) {
    delete[] _coordinatesX;_coordinatesX = NULL;
    delete[] _coordinatesY;_coordinatesY = NULL;
    delete[] _coordinatesZ;_coordinatesZ = NULL;

    delete _indexingX;_indexingX = NULL;
    delete _indexingY;_indexingY = NULL;
    delete _indexingZ;_indexingZ = NULL;

    delete _hyperslab;_hyperslab = NULL;
    delete[] _values;_values = NULL;
} // destructor


// ------------------------------------------------------------------------------------------------
// Load metadata.
void
geomodelgrids::serial::Block::loadMetadata(geomodelgrids::serial::HDF5* const h5) {
    assert(h5);
    delete[] _values;_values = NULL;

    const std::string& blockPath = std::string("blocks/") + _name;

    std::ostringstream msg;
    const char* indent = "            ";
    bool attributeErrors = false;

    size_t dims[3];
    if (h5->hasAttribute(blockPath.c_str(), "x_resolution")) {
        h5->readAttribute(blockPath.c_str(), "x_resolution", H5T_NATIVE_DOUBLE, (void*)&_resolutionX);
    } else {
        if (h5->hasAttribute(blockPath.c_str(), "x_coordinates")) {
            h5->readAttribute(blockPath.c_str(), "x_coordinates", H5T_NATIVE_DOUBLE, (void**)&_coordinatesX, &dims[0]);
            std::sort(_coordinatesX, _coordinatesX+dims[0], geomodelgrids::utils::IndexingVariable::less);
        } else {
            msg << indent << "    /" << blockPath << "/x_resolution or /" << blockPath << "/x_coordinates\n";
            attributeErrors = true;
        } // if/else
    } // if/else

    if (h5->hasAttribute(blockPath.c_str(), "y_resolution")) {
        h5->readAttribute(blockPath.c_str(), "y_resolution", H5T_NATIVE_DOUBLE, (void*)&_resolutionY);
    } else {
        if (h5->hasAttribute(blockPath.c_str(), "y_coordinates")) {
            h5->readAttribute(blockPath.c_str(), "y_coordinates", H5T_NATIVE_DOUBLE, (void**)&_coordinatesY, &dims[1]);
            std::sort(_coordinatesY, _coordinatesY+dims[1], geomodelgrids::utils::IndexingVariable::less);
        } else {
            msg << indent << "    /" << blockPath << "/y_resolution or /" << blockPath << "/y_coordinates\n";
            attributeErrors = true;
        } // if/else
    } // if/else

    if (h5->hasAttribute(blockPath.c_str(), "z_resolution")) {
        h5->readAttribute(blockPath.c_str(), "z_resolution", H5T_NATIVE_DOUBLE, (void*)&_resolutionZ);
    } else {
        if (h5->hasAttribute(blockPath.c_str(), "z_coordinates")) {
            h5->readAttribute(blockPath.c_str(), "z_coordinates", H5T_NATIVE_DOUBLE, (void**)&_coordinatesZ, &dims[2]);
            std::sort(_coordinatesZ, _coordinatesZ+dims[2], geomodelgrids::utils::IndexingVariable::greater);
        } else {
            msg << indent << "    /" << blockPath << "/z_resolution or /" << blockPath << "/z_coordinates\n";
            attributeErrors = true;
        } // if/else
    } // if/else

    if (h5->hasAttribute(blockPath.c_str(), "z_resolution")) {
        if (h5->hasAttribute(blockPath.c_str(), "z_top")) {
            h5->readAttribute(blockPath.c_str(), "z_top", H5T_NATIVE_DOUBLE, (void*)&_zTop);
        } else {
            msg << indent << "    " << blockPath << "/z_top\n";
            attributeErrors = true;
        } // if/else
    } else if (h5->hasAttribute(blockPath.c_str(), "z_coordinates")) {
        assert(_coordinatesZ);
        _zTop = _coordinatesZ[0];
    } // if/else

    hsize_t* hdims = NULL;
    int ndims = 0;
    h5->getDatasetDims(&hdims, &ndims, blockPath.c_str());
    assert(4 == ndims);
    for (int i = 0; i < 3; ++i) {
        _dims[i] = hdims[i];
    } // for

    if (0 == _hyperslabDims[2]) {
        _hyperslabDims[2] = hdims[2];
    } // if
    if (0 == _hyperslabDims[3]) {
        _hyperslabDims[3] = hdims[3];
    } // if

    _numValues = hdims[3];
    delete[] hdims;hdims = NULL;

    // Check to make sure dimensions of block match coordinates (if provided).
    if (_coordinatesX && (dims[0] != _dims[0])) {
        msg << indent << "    x dimension of block " << blockPath << " (" << _dims[0]
            << ") does not match number of x coordinates (" << dims[0] << ").\n";
        attributeErrors = true;
    } // if
    if (_coordinatesY && (dims[1] != _dims[1])) {
        msg << indent << "    y dimension of block " << blockPath << " (" << _dims[1]
            << ") does not match number of x coordinates (" << dims[1] << ").\n";
        attributeErrors = true;
    } // if
    if (_coordinatesZ && (dims[2] != _dims[2])) {
        msg << indent << "    z dimension of block " << blockPath << " (" << _dims[2]
            << ") does not match number of x coordinates (" << dims[2] << ").\n";
        attributeErrors = true;
    } // if

    if (attributeErrors) { throw std::runtime_error(msg.str().c_str()); }

    delete _indexingX;_indexingX = NULL;
    delete _indexingY;_indexingY = NULL;
    delete _indexingZ;_indexingZ = NULL;
    if (!_coordinatesX) {
        _indexingX = new geomodelgrids::utils::IndexingUniform(_resolutionX);
    } else {
        assert(_coordinatesX);
        _indexingX = new geomodelgrids::utils::IndexingVariable(_coordinatesX, _dims[0]);
    } // if/else
    if (!_coordinatesY) {
        _indexingY = new geomodelgrids::utils::IndexingUniform(_resolutionY);
    } else {
        assert(_coordinatesY);
        _indexingY = new geomodelgrids::utils::IndexingVariable(_coordinatesY, _dims[1]);
    } // if/else
    if (!_coordinatesZ) {
        _indexingZ = new geomodelgrids::utils::IndexingUniform(_resolutionZ);
    } else {
        assert(_coordinatesZ);
        _indexingZ = new geomodelgrids::utils::IndexingVariable(_coordinatesZ, _dims[2],
                                                                geomodelgrids::utils::IndexingVariable::DESCENDING);
    } // if/else
} // loadMetadata


// ------------------------------------------------------------------------------------------------
// Get name of block.
const std::string&
geomodelgrids::serial::Block::getName(void) const {
    return _name;
} // getName


// ------------------------------------------------------------------------------------------------
// Get resolution along x-axis.
double
geomodelgrids::serial::Block::getResolutionX(void) const {
    return _resolutionX;
} // getResolutionX


// ------------------------------------------------------------------------------------------------
// Get resolution along y-axis.
double
geomodelgrids::serial::Block::getResolutionY(void) const {
    return _resolutionY;
} // getResolutionY


// ------------------------------------------------------------------------------------------------
// Get resolution along z-axis.
double
geomodelgrids::serial::Block::getResolutionZ(void) const {
    return _resolutionZ;
} // getResolutionZ


// ------------------------------------------------------------------------------------------------
// Get elevation of top of block in topological space.
double
geomodelgrids::serial::Block::getZTop(void) const {
    return _zTop;
} // getZTop


// ------------------------------------------------------------------------------------------------
// Get elevation of bottom of block in topological space.
double
geomodelgrids::serial::Block::getZBottom(void) const {
    double zBottom = 0.0;
    if (_resolutionZ > 0.0) {
        zBottom = _zTop - _resolutionZ * (_dims[2] - 1);
    } else if (_coordinatesZ) {
        zBottom = _coordinatesZ[_dims[2]-1];
    } // if/else

    return zBottom;
} // getZBottom


// ------------------------------------------------------------------------------------------------
// Get coordinates along x axis.
double*
geomodelgrids::serial::Block::getCoordinatesX(void) const {
    return _coordinatesX;
} // getCoordinatesX


// ------------------------------------------------------------------------------------------------
// Get coordinates along y axis.
double*
geomodelgrids::serial::Block::getCoordinatesY(void) const {
    return _coordinatesY;
} // getCoordinatesY


// ------------------------------------------------------------------------------------------------
// Get coordinates along z axis.
double*
geomodelgrids::serial::Block::getCoordinatesZ(void) const {
    return _coordinatesZ;
} // getCoordinatesZ


// ------------------------------------------------------------------------------------------------
// Get number of values along each grid dimension.
const size_t*
geomodelgrids::serial::Block::getDims(void) const {
    return _dims;
} // getDims


// ------------------------------------------------------------------------------------------------
// Get number of values stored at each grid point.
size_t
geomodelgrids::serial::Block::getNumValues(void) const {
    return _numValues;
} // getNumValues


// ------------------------------------------------------------------------------------------------
// Set hyperslab size.
void
geomodelgrids::serial::Block::setHyperslabDims(const size_t dims[],
                                               const size_t ndimsIn) {
    const size_t ndims = 3; // 4th dimension is number of values.
    if (3 != ndimsIn) {
        std::ostringstream msg;
        msg << "Expected array of length " << ndims << " for hyperslab dimension, got array of length "
            << ndimsIn << ".";
        throw std::length_error(msg.str().c_str());
    } // if
    assert(dims);

    for (size_t i = 0; i < ndims; ++i) {
        _hyperslabDims[i] = dims[i];
    } // for
} // setHyperslabDims


// ------------------------------------------------------------------------------------------------
// Prepare for querying.
void
geomodelgrids::serial::Block::openQuery(geomodelgrids::serial::HDF5* const h5) {
    const size_t ndims = 4;
    hsize_t dims[ndims];
    for (size_t i = 0; i < ndims; ++i) {
        dims[i] = _hyperslabDims[i];
    } // for
    const std::string blockPath(std::string("/blocks/") + _name);
    delete _hyperslab;_hyperslab = new geomodelgrids::serial::Hyperslab(h5, blockPath.c_str(), dims, ndims);

    delete[] _values;_values = (_numValues > 0) ? new double[_numValues] : NULL;
} // openQuery


// ------------------------------------------------------------------------------------------------
// Query for values at a point using bilinear interpolation.
const double*
geomodelgrids::serial::Block::query(const double x,
                                    const double y,
                                    const double z) {
    assert(x >= 0.0);
    assert(y >= 0.0);
    assert(z <= 0.0);

    assert(_indexingX);
    assert(_indexingY);
    assert(_indexingZ);

    double index[3];
    index[0] = _indexingX->getIndex(x);
    index[1] = _indexingY->getIndex(y);
    index[2] = _indexingZ->getIndex(_zTop - z);

    assert( (_numValues > 0 && _values) || (!_numValues && !_values) );

    assert(_hyperslab);
    _hyperslab->interpolate(_values, index);

    return _values;
} // query


// ------------------------------------------------------------------------------------------------
// Cleanup after querying.
void
geomodelgrids::serial::Block::closeQuery(void) {
    delete _hyperslab;_hyperslab = NULL;
    delete[] _values;_values = NULL;
} // closeQuery


// ------------------------------------------------------------------------------------------------
// Compare order of blocks by z_top (descending order).
bool
geomodelgrids::serial::Block::compare(const Block* a,
                                      const Block* b) {
    if (a && b) { return a->_zTop > b->_zTop; }

    return true;
} // compare


// End of file
