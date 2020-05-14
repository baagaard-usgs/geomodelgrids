#include <portinfo>

#include "Block.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/serial/Hyperslab.hh" // USES Hyperslab

#include <cstring> // USES strlen()
#include <algorithm> // USES std::max()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Block::Block(const char* name) :
    _name(name),
    _hyperslab(NULL),
    _resolutionHoriz(0.0),
    _resolutionVert(0.0),
    _zTop(0.0),
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


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Block::~Block(void) {
    delete _hyperslab;_hyperslab = NULL;
    delete[] _values;_values = NULL;
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Load metadata.
void
geomodelgrids::serial::Block::loadMetadata(geomodelgrids::serial::HDF5* const h5) {
    assert(h5);
    delete[] _values;_values = NULL;

    const std::string blockpath = std::string("blocks/") + _name;

    h5->readAttribute(blockpath.c_str(), "resolution_horiz", H5T_NATIVE_DOUBLE, (void*)&_resolutionHoriz);
    h5->readAttribute(blockpath.c_str(), "resolution_vert", H5T_NATIVE_DOUBLE, (void*)&_resolutionVert);
    h5->readAttribute(blockpath.c_str(), "z_top", H5T_NATIVE_DOUBLE, (void*)&_zTop);

    hsize_t* hdims = NULL;
    int ndims = 0;
    h5->getDatasetDims(&hdims, &ndims, blockpath.c_str());
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
} // loadMetadata


// ---------------------------------------------------------------------------------------------------------------------
// Get name of block.
const std::string&
geomodelgrids::serial::Block::getName(void) const {
    return _name;
} // getName


// ---------------------------------------------------------------------------------------------------------------------
// Get horizontal resolution.
double
geomodelgrids::serial::Block::getResolutionHoriz(void) const {
    return _resolutionHoriz;
} // getResolutionHoriz


// ---------------------------------------------------------------------------------------------------------------------
// Get vertical resolution.
double
geomodelgrids::serial::Block::getResolutionVert(void) const {
    return _resolutionVert;
} // getResolutionVert


// ---------------------------------------------------------------------------------------------------------------------
// Get elevation of top of block in topological space.
double
geomodelgrids::serial::Block::getZTop(void) const {
    return _zTop;
} // getZTop


// ---------------------------------------------------------------------------------------------------------------------
// Get elevation of bottom of block in topological space.
double
geomodelgrids::serial::Block::getZBottom(void) const {
    return _zTop - _resolutionVert * (_dims[2] - 1);
} // getZBottom


// ---------------------------------------------------------------------------------------------------------------------
// Get number of values along each grid dimension.
const size_t*
geomodelgrids::serial::Block::getDims(void) const {
    return _dims;
} // getDims


// ---------------------------------------------------------------------------------------------------------------------
// Get number of values stored at each grid point.
size_t
geomodelgrids::serial::Block::getNumValues(void) const {
    return _numValues;
} // getNumValues


// ---------------------------------------------------------------------------------------------------------------------
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

    for (int i = 0; i < ndims; ++i) {
        _hyperslabDims[i] = dims[i];
    } // for
} // setHyperslabDims


// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
// Query for values at a point using bilinear interpolation.
const double*
geomodelgrids::serial::Block::query(const double x,
                                    const double y,
                                    const double z) {
    assert(x >= 0.0);
    assert(y >= 0.0);
    assert(z <= 0.0);

    assert( (_numValues > 0 && _values) || (!_numValues && !_values) );

    double index[3];
    index[0] = x / _resolutionHoriz;assert(index[0] < double(_dims[0]));
    index[1] = y / _resolutionHoriz;assert(index[1] < double(_dims[1]));
    index[2] = (_zTop - z)/ _resolutionVert;assert(index[2] < double(_dims[2]));
    _hyperslab->interpolate(_values, index);

    return _values;
} // query


// ---------------------------------------------------------------------------------------------------------------------
// Cleanup after querying.
void
geomodelgrids::serial::Block::closeQuery(void) {
    delete _hyperslab;_hyperslab = NULL;
    delete[] _values;_values = NULL;
} // closeQuery


// ---------------------------------------------------------------------------------------------------------------------
// Compare order of blocks by z_top (descending order).
bool
geomodelgrids::serial::Block::compare(const Block* a,
                                      const Block* b) {
    if (a && b) { return a->_zTop > b->_zTop; }

    return true;
} // compare


// End of file
