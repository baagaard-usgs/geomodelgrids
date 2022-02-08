#include <portinfo>

#include "CRSTransformer.hh" // implementation of class methods

#include <cmath> // USES HUGE_VAL
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()
#include <cstring> // USES strlen()
#include <strings.h> // USES stdcasecmp()

namespace geomodelgrids {
    namespace utils {
        class _CRSTransformer {
public:

            static
            void getUnits(std::string* xUnit,
                          std::string* yUnit,
                          std::string* zUnit,
                          const PJ* projCoordSys);

            static
            void getAxisUnit(std::string* unit,
                             const PJ* projCoordSys,
                             const int axisIndex);

        };

    }
}

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
    PJ_CONTEXT* context = PJ_DEFAULT_CTX;
    _proj = proj_create_crs_to_crs(context, _srcString.c_str(), _destString.c_str(), NULL);
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

    PJ_COORD xyzSrc = proj_coord(srcX, srcY, srcZ, HUGE_VAL);
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

    PJ_COORD xyzDest = proj_coord(destX, destY, destZ, HUGE_VAL);
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
    PJ_CONTEXT* context = PJ_DEFAULT_CTX;
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


// ------------------------------------------------------------------------------------------------
// Get units for CRS.
void
geomodelgrids::utils::CRSTransformer::getCRSUnits(std::string* xUnit,
                                                  std::string* yUnit,
                                                  std::string* zUnit,
                                                  const char* crsString) {
    if (xUnit) { *xUnit = "unknown"; }
    if (yUnit) { *yUnit = "unknown"; }
    if (zUnit) { *zUnit = "meter (assumed)"; }
    if (!crsString || (0 == strlen(crsString))) { return; }

    PJ_CONTEXT* context = PJ_DEFAULT_CTX;
    PJ* proj = proj_create(context, crsString);assert(proj);
    PJ* projCoordSys = proj_crs_get_coordinate_system(context, proj);
    if (projCoordSys) {
        _CRSTransformer::getUnits(xUnit, yUnit, zUnit, projCoordSys);
        proj_destroy(proj);
        proj_destroy(projCoordSys);
        return;
    } else if (proj_get_type(proj) == PJ_TYPE_BOUND_CRS) {
        PJ* projTmp = proj_get_source_crs(context, proj);
        proj_destroy(proj);
        if (!projTmp) {
            return;
        } // if
        const char* srcWKT = proj_as_wkt(context, projTmp, PJ_WKT2_2019, NULL);
        PJ* projSrc = proj_create(context, srcWKT);
        projCoordSys = proj_crs_get_coordinate_system(context, projSrc);
        proj_destroy(projTmp);
        proj_destroy(projSrc);
        if (!projCoordSys) {
            return;
        }
        _CRSTransformer::getUnits(xUnit, yUnit, zUnit, projCoordSys);
        proj_destroy(projCoordSys);
        return;
    }
}


// ------------------------------------------------------------------------------------------------
// Get units for CRS.
void
geomodelgrids::utils::_CRSTransformer::getUnits(std::string* xUnit,
                                                std::string* yUnit,
                                                std::string* zUnit,
                                                const PJ* projCoordSys) {
    const int numAxes = proj_cs_get_axis_count(NULL, projCoordSys);
    if (xUnit && (numAxes >= 1)) {
        _CRSTransformer::getAxisUnit(xUnit, projCoordSys, 0);
    }
    if (yUnit && (numAxes >= 2)) {
        _CRSTransformer::getAxisUnit(yUnit, projCoordSys, 1);
    }
    if (zUnit && (numAxes >= 3)) {
        _CRSTransformer::getAxisUnit(zUnit, projCoordSys, 2);
    }
}


// ------------------------------------------------------------------------------------------------
// Get units for CRS.
void
geomodelgrids::utils::_CRSTransformer::getAxisUnit(std::string* unit,
                                                   const PJ* projCoordSys,
                                                   const int axisIndex) {
    assert(unit);
    assert(projCoordSys);

    const char* buffer = NULL;
    proj_cs_get_axis_info(NULL, projCoordSys, 0, NULL, NULL, NULL, NULL, &buffer, NULL, NULL);
    if (strcasecmp("metre", buffer) == 0) {
        *unit = "meter";
    } else if (strcasecmp("meter", buffer) == 0) {
        *unit = "meter";
    } else if (strcasecmp("kilometre", buffer) == 0) {
        *unit = "kilometer";
    } else {
        *unit = buffer;
    }
}


// End of file
