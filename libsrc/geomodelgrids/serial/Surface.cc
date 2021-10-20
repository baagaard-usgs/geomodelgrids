#include <portinfo>

#include "Surface.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/serial/Hyperslab.hh" // USES Hyperslab
#include "geomodelgrids/utils/Indexing.hh" // USES Resolution
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include <cstring> // USES strlen()
#include <algorithm> // USES std::sort
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Surface::Surface(const char* const name) :
    _hyperslab(NULL),
    _name(name),
    _resolutionX(0.0),
    _resolutionY(0.0),
    _coordinatesX(NULL),
    _coordinatesY(NULL),
    _indexingX(NULL),
    _indexingY(NULL) {
    _dims[0] = 0;
    _dims[1] = 0;

    _hyperslabDims[0] = 128;
    _hyperslabDims[1] = 128;
    _hyperslabDims[2] = 1;
} // constructor


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Surface::~Surface(void) {
    delete[] _coordinatesX;_coordinatesX = NULL;
    delete[] _coordinatesY;_coordinatesY = NULL;

    delete _indexingX;_indexingX = NULL;
    delete _indexingY;_indexingY = NULL;
    delete _hyperslab;_hyperslab = NULL;
} // destructor


// ------------------------------------------------------------------------------------------------
// Load metadata.
void
geomodelgrids::serial::Surface::loadMetadata(geomodelgrids::serial::HDF5* const h5) {
    assert(h5);

    const std::string& surfacePath = std::string("surfaces/") + _name;

    std::ostringstream msg;
    const char* indent = "            ";
    bool attributeErrors = false;

    size_t dims[2];
    if (h5->hasAttribute(surfacePath.c_str(), "x_resolution")) {
        h5->readAttribute(surfacePath.c_str(), "x_resolution", H5T_NATIVE_DOUBLE, (void*)&_resolutionX);
    } else {
        if (h5->hasAttribute(surfacePath.c_str(), "x_coordinates")) {
            h5->readAttribute(surfacePath.c_str(), "x_coordinates", H5T_NATIVE_DOUBLE, (void**)&_coordinatesX, &dims[0]);
            std::sort(_coordinatesX, _coordinatesX+dims[0], geomodelgrids::utils::IndexingVariable::less);
        } else {
            msg << indent << "    /" << surfacePath << "/x_resolution or /" << surfacePath << "/x_coordinates\n";
            attributeErrors = true;
        } // if/else
    } // if/else

    if (h5->hasAttribute(surfacePath.c_str(), "y_resolution")) {
        h5->readAttribute(surfacePath.c_str(), "y_resolution", H5T_NATIVE_DOUBLE, (void*)&_resolutionY);
    } else {
        if (h5->hasAttribute(surfacePath.c_str(), "y_coordinates")) {
            h5->readAttribute(surfacePath.c_str(), "y_coordinates", H5T_NATIVE_DOUBLE, (void**)&_coordinatesY, &dims[1]);
            std::sort(_coordinatesY, _coordinatesY+dims[1], geomodelgrids::utils::IndexingVariable::less);
        } else {
            msg << indent << "    /" << surfacePath << "/y_resolution or /" << surfacePath << "/y_coordinates\n";
            attributeErrors = true;
        } // if/else
    } // if/else

    hsize_t* hdims = NULL;
    int ndims = 0;
    h5->getDatasetDims(&hdims, &ndims, surfacePath.c_str());
    assert(3 == ndims);
    for (int i = 0; i < 2; ++i) {
        _dims[i] = hdims[i];
    } // for
    delete[] hdims;hdims = NULL;

    // Check to make sure dimensions of surface match coordinates (if provided).
    if (_coordinatesX && (dims[0] != _dims[0])) {
        msg << indent << "    x dimension of surface " << surfacePath << " (" << _dims[0]
            << ") does not match number of x coordinates (" << dims[0] << ").\n";
        attributeErrors = true;
    } // if
    if (_coordinatesY && (dims[1] != _dims[1])) {
        msg << indent << "    y dimension of surface " << surfacePath << " (" << _dims[1]
            << ") does not match number of x coordinates (" << dims[1] << ").\n";
        attributeErrors = true;
    } // if

    if (attributeErrors) { throw std::runtime_error(msg.str().c_str()); }

    delete _indexingX;_indexingX = NULL;
    delete _indexingY;_indexingY = NULL;
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
} // loadMetadata


// ------------------------------------------------------------------------------------------------
// Get resolution along x-axis.
double
geomodelgrids::serial::Surface::getResolutionX(void) const {
    return _resolutionX;
} // getResolutionX


// ------------------------------------------------------------------------------------------------
// Get resolution along y-axis.
double
geomodelgrids::serial::Surface::getResolutionY(void) const {
    return _resolutionY;
} // getResolutionY


// ------------------------------------------------------------------------------------------------
// Get coordinates along x-axis.
double*
geomodelgrids::serial::Surface::getCoordinatesX(void) const {
    return _coordinatesX;
} // getCoordinatesX


// ------------------------------------------------------------------------------------------------
// Get coordinates along y-axis.
double*
geomodelgrids::serial::Surface::getCoordinatesY(void) const {
    return _coordinatesY;
} // getCoordinatesY


// ------------------------------------------------------------------------------------------------
// Get number of values along each grid dimension.
const size_t*
geomodelgrids::serial::Surface::getDims(void) const {
    return _dims;
} // getDims


// ------------------------------------------------------------------------------------------------
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


// ------------------------------------------------------------------------------------------------
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


// ------------------------------------------------------------------------------------------------
// Cleanup after querying.
void
geomodelgrids::serial::Surface::closeQuery(void) {
    delete _hyperslab;_hyperslab = NULL;
} // closeQuery


// ------------------------------------------------------------------------------------------------
// Query for values at a point using bilinear interpolation.
double
geomodelgrids::serial::Surface::query(const double x,
                                      const double y) {
    assert(_indexingX);
    assert(_indexingY);

    double index[2];
    index[0] = _indexingX->getIndex(x);
    index[1] = _indexingY->getIndex(y);

    double elevation = geomodelgrids::NODATA_VALUE;
    if ((index[0] >= 0) && (index[0] <= double(_dims[0]-1))
        && (index[1] >= 0) && (index[1] <= double(_dims[1]-1))) {
        assert(_hyperslab);
        _hyperslab->interpolate(&elevation, index);
    } // if

    return elevation;
} // query


// End of file
