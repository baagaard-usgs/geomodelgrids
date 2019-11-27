#include <portinfo>

#include "Block.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Block::Block(const char* name) :
    _name(name),
    _resolutionHoriz(0.0),
    _resolutionVert(0.0),
    _zTop(0.0),
    _values(NULL),
    _numValues(0) {
    _dims[0] = 0;
    _dims[1] = 0;
    _dims[2] = 0;
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Block::~Block(void) {
    delete[] _values;_values = NULL;
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Load metadata.
void
geomodelgrids::serial::Block::loadMetadata(geomodelgrids::serial::HDF5* const h5) {
    assert(h5);

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

    _numValues = hdims[3];
    delete[] _values;_values = NULL;
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
// Query for values at a point using bilinear interpolation.
const double*
geomodelgrids::serial::Block::query(const double x,
                                    const double y,
                                    const double z) {
    return _values;
} // query


// ---------------------------------------------------------------------------------------------------------------------
// Compare order of blocks by z_top (descending order).
bool
geomodelgrids::serial::Block::compare(const Block* a,
                                      const Block* b) {
    if (a && b) { return a->_zTop > b->_zTop; }

    return true;
} // compare


// End of file
