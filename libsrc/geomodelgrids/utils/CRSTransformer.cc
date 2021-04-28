#include <portinfo>

#include "CRSTransformer.hh" // implementation of class methods

#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::utils::CRSTransformer::CRSTransformer(void) :
    _srcString("EPSG:4326"), // latitude/longitude WGS84
    _destString("EPSG:3488"), // NAD83(HARN) California Albers
    _proj(NULL) {}


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::utils::CRSTransformer::~CRSTransformer(void) {
    if (_proj) {
        proj_destroy(_proj);_proj = NULL;
    } // if
} // destructor


// ------------------------------------------------------------------------------------------------
// Set source coordinate system.
void
geomodelgrids::utils::CRSTransformer::setSrc(const char* value) {
    _srcString = value;
} // setSrc


// ------------------------------------------------------------------------------------------------
// Set destination coordinate system.
void
geomodelgrids::utils::CRSTransformer::setDest(const char* value) {
    _destString = value;
} // setDest


// ------------------------------------------------------------------------------------------------
// Initialize CRS transformation.
void
geomodelgrids::utils::CRSTransformer::initialize(void) {
    if (_proj) {
        proj_destroy(_proj);_proj = NULL;
    } // if
    _proj = proj_create_crs_to_crs(NULL, _srcString.c_str(), _destString.c_str(), NULL);
    if (!_proj) {
        std::stringstream msg;
        msg << "Error creating CRS transformation from '" << _srcString << "' to '" << _destString << "'.\n"
            << proj_errno_string(proj_errno(_proj));
        throw std::runtime_error(msg.str());
    } // if
} // initialize


// ------------------------------------------------------------------------------------------------
// Compute from src CRS to dest CRS.
void
geomodelgrids::utils::CRSTransformer::transform(double* destX,
                                                double* destY,
                                                double* destZ,
                                                const double srcX,
                                                const double srcY,
                                                const double srcZ) {
    assert(destX);
    assert(destY);

    PJ_COORD xyzSrc = proj_coord(srcX, srcY, srcZ, 0.0);
    PJ_COORD xyzDest = proj_trans(_proj, PJ_FWD, xyzSrc);
    *destX = xyzDest.xyzt.x;
    *destY = xyzDest.xyzt.y;
    if (destZ) {
        *destZ = xyzDest.xyzt.z;
    } // if
} // transform


// ------------------------------------------------------------------------------------------------
// Compute from src CRS to dest CRS.
void
geomodelgrids::utils::CRSTransformer::inverse_transform(double* srcX,
                                                        double* srcY,
                                                        double* srcZ,
                                                        const double destX,
                                                        const double destY,
                                                        const double destZ) {
    assert(srcX);
    assert(srcY);

    PJ_COORD xyzDest = proj_coord(destX, destY, destZ, 0.0);
    PJ_COORD xyzSrc = proj_trans(_proj, PJ_INV, xyzDest);
    *srcX = xyzSrc.xyzt.x;
    *srcY = xyzSrc.xyzt.y;
    if (srcZ) {
        *srcZ = xyzSrc.xyzt.z;
    } // if
} // transform


// ------------------------------------------------------------------------------------------------
// Get boundary box in x/y order from bounding box in CRS.
geomodelgrids::utils::CRSTransformer*
geomodelgrids::utils::CRSTransformer::createGeoToXYAxisOrder(const char* crsString) {
    PJ_CONTEXT* context = NULL;
    PJ* projGeo = proj_create(context, crsString);
    if (!projGeo) {
        std::stringstream msg;
        msg << "Error creating CRS from '" << crsString << "'.\n"
            << proj_errno_string(proj_errno(projGeo));
        throw std::runtime_error(msg.str());
    } // if
    PJ* projXY = proj_normalize_for_visualization(context, projGeo);
    if (!projXY) {
        proj_destroy(projGeo);

        std::stringstream msg;
        msg << "Error creating normalized CRS from '" << crsString << "'.\n"
            << proj_errno_string(proj_errno(projGeo));
        throw std::runtime_error(msg.str());
    }
    PJ* transform = proj_create_crs_to_crs_from_pj(context, projGeo, projXY, NULL, NULL);
    proj_destroy(projGeo);
    proj_destroy(projXY);
    if (!transform) {
        std::stringstream msg;
        msg << "Error geo to xy transformation for CRS from '" << crsString << "'.\n"
            << proj_errno_string(proj_errno(transform));
        throw std::runtime_error(msg.str());
    } // if
    CRSTransformer* transformer = new CRSTransformer();
    transformer->_proj = transform;

    return transformer;
}


// End of file
