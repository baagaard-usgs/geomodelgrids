#include <portinfo>

#include "Projection.hh" // implementation of class methods

#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::utils::Projection::Projection(void) :
    _srcString("EPSG:4326"), // latitude/longitude WGS84
    _destString("EPSG:3488"), // NAD83(HARN) California Albers
    _proj(NULL) {}


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::utils::Projection::~Projection(void) {
    if (_proj) {
        proj_destroy(_proj);_proj = NULL;
    } // if
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Set source coordinate system.
void
geomodelgrids::utils::Projection::setSrc(const char* value) {
    _srcString = value;
} // setSrc


// ---------------------------------------------------------------------------------------------------------------------
// Set destination coordinate system.
void
geomodelgrids::utils::Projection::setDest(const char* value) {
    _destString = value;
} // setDest


// ---------------------------------------------------------------------------------------------------------------------
// Initialize projection.
void
geomodelgrids::utils::Projection::initialize(void) {
    if (_proj) {
        proj_destroy(_proj);_proj = NULL;
    } // if
    _proj = proj_create_crs_to_crs(NULL, _srcString.c_str(), _destString.c_str(), NULL);
    if (!_proj) {
        std::stringstream msg;
        msg << "Error creating projection from '" << _srcString << "' to '" << _destString << "'.";
        throw std::runtime_error(msg.str());
    } // if
} // initialize


// ---------------------------------------------------------------------------------------------------------------------
// Compute xy coordinates in geographic projection.
void
geomodelgrids::utils::Projection::project(double* x,
                                          double* y,
                                          const double longitude,
                                          const double latitude) {
    assert(x);
    assert(y);

    PJ_COORD xySrc = proj_coord(latitude, longitude, 0.0, 0.0);
    PJ_COORD xyDest = proj_trans(_proj, PJ_FWD, xySrc);
    *x = xyDest.xyzt.x;
    *y = xyDest.xyzt.y;
} // project


// End of file
