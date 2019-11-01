#include <portinfo>

#include "Topography.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Topography::Topography(void) :
    _resolutionHoriz(0.0) {
    _dims[0] = 0;
    _dims[1] = 0;
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Topography::~Topography(void) {} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Load metadata.
void
geomodelgrids::serial::Topography::loadMetadata(geomodelgrids::serial::HDF5* const h5) {
    assert(h5);

    h5->readAttribute("topography", "resolution_horiz", H5T_NATIVE_DOUBLE, (void*)&_resolutionHoriz);

    hsize_t* hdims = NULL;
    int ndims = 0;
    h5->getDatasetDims(&hdims, &ndims, "topography");
    assert(2 == ndims);
    for (int i = 0; i < 2; ++i) {
        _dims[i] = hdims[i];
    } // for
} // loadMetadata


// ---------------------------------------------------------------------------------------------------------------------
// Get horizontal resolution.
double
geomodelgrids::serial::Topography::getResolutionHoriz(void) const {
    return _resolutionHoriz;
} // getResolutionHoriz


// ---------------------------------------------------------------------------------------------------------------------
// Query for values at a point using bilinear interpolation.
double
geomodelgrids::serial::Topography::query(const double x,
                                         const double y) {
    double elevation = 0.0;
    return elevation;
} // query


// End of file
