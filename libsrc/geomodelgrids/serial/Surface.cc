#include <portinfo>

#include "Surface.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/serial/Hyperslab.hh" // USES Hyperslab
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Surface::Surface(const char* const name) :
    _hyperslab(NULL),
    _name(name),
    _resolutionHoriz(0.0) {
    _dims[0] = 0;
    _dims[1] = 0;

    _hyperslabDims[0] = 128;
    _hyperslabDims[1] = 128;
    _hyperslabDims[2] = 1;
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Surface::~Surface(void) {
    delete _hyperslab;_hyperslab = NULL;
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Load metadata.
void
geomodelgrids::serial::Surface::loadMetadata(geomodelgrids::serial::HDF5* const h5) {
    assert(h5);

    const std::string& surfacePath = std::string("surfaces/") + _name;

    std::ostringstream msg;
    const char* indent = "            ";
    bool missingAttributes = false;

    if (h5->hasAttribute(surfacePath.c_str(), "resolution_horiz")) {
        h5->readAttribute(surfacePath.c_str(), "resolution_horiz", H5T_NATIVE_DOUBLE, (void*)&_resolutionHoriz);
    } else {
        msg << indent << "    /" << surfacePath << "/resolution_horiz\n";
        missingAttributes = true;
    } // if/else

    hsize_t* hdims = NULL;
    int ndims = 0;
    h5->getDatasetDims(&hdims, &ndims, surfacePath.c_str());
    assert(3 == ndims);
    for (int i = 0; i < 2; ++i) {
        _dims[i] = hdims[i];
    } // for
    delete[] hdims;hdims = NULL;

    if (missingAttributes) { throw std::runtime_error(msg.str().c_str()); }
} // loadMetadata


// ---------------------------------------------------------------------------------------------------------------------
// Get horizontal resolution.
double
geomodelgrids::serial::Surface::getResolutionHoriz(void) const {
    return _resolutionHoriz;
} // getResolutionHoriz


// ---------------------------------------------------------------------------------------------------------------------
// Get number of values along each grid dimension.
const size_t*
geomodelgrids::serial::Surface::getDims(void) const {
    return _dims;
} // getDims


// ---------------------------------------------------------------------------------------------------------------------
// Set hyperslab size.
void
geomodelgrids::serial::Surface::setHyperslabDims(const size_t dims[],
                                                 const size_t ndimsIn) {
    const size_t ndims = 2; // 3rd dimension is 1.
    if (2 != ndimsIn) {
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


// ---------------------------------------------------------------------------------------------------------------------
// Prepare for querying.
void
geomodelgrids::serial::Surface::openQuery(geomodelgrids::serial::HDF5* const h5) {
    const size_t ndims = 3;
    hsize_t dims[ndims];
    dims[0] = 128;
    dims[1] = 128;
    dims[2] = 1;
    const std::string& surfacePath = std::string("surfaces/") + _name;
    delete _hyperslab;_hyperslab = new geomodelgrids::serial::Hyperslab(h5, surfacePath.c_str(), dims, ndims);
} // openQuery


// ---------------------------------------------------------------------------------------------------------------------
// Cleanup after querying.
void
geomodelgrids::serial::Surface::closeQuery(void) {
    delete _hyperslab;_hyperslab = NULL;
} // closeQuery


// ---------------------------------------------------------------------------------------------------------------------
// Query for values at a point using bilinear interpolation.
double
geomodelgrids::serial::Surface::query(const double x,
                                      const double y) {
    double index[2];
    index[0] = x / _resolutionHoriz;
    index[1] = y / _resolutionHoriz;

    double elevation = geomodelgrids::NODATA_VALUE;
    if ((index[0] >= 0) && (index[0] <= double(_dims[0]-1))
        && (index[1] >= 0) && (index[1] <= double(_dims[1]-1))) {
        assert(_hyperslab);
        _hyperslab->interpolate(&elevation, index);
    } // if

    return elevation;
} // query


// End of file
