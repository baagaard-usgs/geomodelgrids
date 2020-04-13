#include <portinfo>

#include "ModelPoints.hh" // Implementation of class methods

#include "proj.h" // USES PJ

#include <cmath> // USES sin(), cos()
#include <cassert> // USES assert()

namespace geomodelgrids {
    namespace serial {
        class _ModelPoints;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::_ModelPoints {
    // PUBLIC METHODS //////////////////////////////////////////////////////////////////////////////////////////////////
public:

    struct Domain {
        double originX;
        double originY;
        double yAzimuth;
        double zBottom;
        bool hasTopography;
    };

    static
    void toXYZ(double* const coordsDest,
               const char* destString,
               const char* srcString,
               const double* coordsSrc,
               const size_t numPoints,
               const Domain& domain);

}; // class _Models

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::ModelPoints::ModelPoints(const size_t numPoints) :
    _numPoints(numPoints),
    _pointsLLE(NULL),
    _pointsXYZ(new double[numPoints*3]),
    _inCRS("EPSG:4326"),
    _modelCRS(NULL) {
    assert(_pointsXYZ);
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor.
geomodelgrids::serial::ModelPoints::~ModelPoints(void) {
    delete[] _pointsXYZ;_pointsXYZ = NULL;
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Get number of points.
size_t
geomodelgrids::serial::ModelPoints::getNumPoints(void) const {
    return _numPoints;
} // getNumPoints


// ---------------------------------------------------------------------------------------------------------------------
// Get CRS for longitude, latitude, and elevation=.
const char*
geomodelgrids::serial::ModelPoints::getCRSLonLatElev(void) const {
    return _inCRS;
} // getCRSLonLatElev


// ---------------------------------------------------------------------------------------------------------------------
// Get geographic coordinates of sample points.
const double*
geomodelgrids::serial::ModelPoints::getLatLonElev(void) const {
    return _pointsLLE;
} // getLatLonElev


// ---------------------------------------------------------------------------------------------------------------------
/** Get model coordinates of sample points.
 *
 * @returns Coordinates (x, y, z) of sample points.
 */
const double*
geomodelgrids::serial::ModelPoints::getXYZ(void) const {
    return _pointsXYZ;
} // getXYZ


// ---------------------------------------------------------------------------------------------------------------------
// Compute elevation of ground surface at point.
double
geomodelgrids::serial::ModelPoints::computeElevation(const double x,
                                                     const double y) {
    return 1.5e+2 + 2.0e-5 * x - 1.2e-5 * y + 5.0e-10 * x * y;
} // computeTopography


// ---------------------------------------------------------------------------------------------------------------------
// Compute value 'one' at point.
double
geomodelgrids::serial::ModelPoints::computeValueOne(const double x,
                                                    const double y,
                                                    const double z) {
    return 2.0e+3 + 1.0 * x + 0.4 * y - 0.5 * z;
} // computeValueOne


// ---------------------------------------------------------------------------------------------------------------------
// Compute value 'two' at point.
double
geomodelgrids::serial::ModelPoints::computeValueTwo(const double x,
                                                    const double y,
                                                    const double z) {
    return -1.2e+3 + 2.1 * x - 0.9 * y + 0.3 * z;
} // computeValueTwo


#include <iostream>
// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_ModelPoints::toXYZ(double* const coordsDest,
                                           const char* destString,
                                           const char* srcString,
                                           const double* coordsSrc,
                                           const size_t numPoints,
                                           const Domain& domain) {
    assert(coordsDest);
    assert(destString);
    assert(srcString);
    assert(coordsSrc);

    PJ* proj = proj_create_crs_to_crs(NULL, srcString, destString, NULL);assert(proj);

    const size_t dim = 3;
    for (size_t i = 0; i < numPoints; ++i) {
        PJ_COORD xySrc = proj_coord(coordsSrc[i*dim+0], coordsSrc[i*dim+1], 0.0, 0.0);
        PJ_COORD xyDest = proj_trans(proj, PJ_FWD, xySrc);
        const double x = xyDest.xyzt.x - domain.originX;
        const double y = xyDest.xyzt.y - domain.originY;

        const double yAzimuthRad = domain.yAzimuth * M_PI / 180.0;
        coordsDest[i*dim+0] = x * cos(yAzimuthRad) - y * sin(yAzimuthRad);
        coordsDest[i*dim+1] = x * sin(yAzimuthRad) + y * cos(yAzimuthRad);

        double zGroundSurf = 0.0;
        if (domain.hasTopography) {
            zGroundSurf = ModelPoints::computeElevation(coordsDest[i*dim+0], coordsDest[i*dim+1]);
        } // if
        coordsDest[i*dim+2] = domain.zBottom * (zGroundSurf - coordsSrc[i*dim+2]) / (zGroundSurf - domain.zBottom);
    } // for

    proj_destroy(proj);proj = NULL;
} // toXYZ


// ---------------------------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::OneBlockFlatPoints::OneBlockFlatPoints(void) :
    ModelPoints(5) {
    const size_t numPoints = 5;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        37.455, -121.941, 0.0,
        37.479, -121.734, -5.0e+3,
        37.381, -121.581, -3.0e+3,
        37.283, -121.959, -1.5e+3,
        37.262, -121.684, -4.0e+3,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:26910";

    _ModelPoints::Domain domain;
    domain.yAzimuth = 90.0;
    domain.originX = 590000.0;
    domain.originY = 4150000.0;
    domain.zBottom = -5.0e+3;
    domain.hasTopography = false;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, domain);
} // Constructor


// ---------------------------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::OneBlockTopoPoints::OneBlockTopoPoints(void) :
    ModelPoints(5) {
    const size_t numPoints = 5;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        37.455, -121.941, 8.0,
        37.479, -121.734, -5.0e+3,
        37.381, -121.581, -3.0e+3,
        37.283, -121.959, -1.5e+3,
        37.262, -121.684, -4.0e+3,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:26910";

    _ModelPoints::Domain domain;
    domain.yAzimuth = 90.0;
    domain.originX = 590000.0;
    domain.originY = 4150000.0;
    domain.zBottom = -5.0e+3;
    domain.hasTopography = true;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, domain);
} // Constructor


// ---------------------------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::OneBlockSquashPoints::OneBlockSquashPoints(const double squashMinElev) :
    ModelPoints(5) {
    const size_t numPoints = 5;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        37.455, -121.941, -8.0,
        37.479, -121.734, -5.0e+3,
        37.381, -121.581, -3.0e+3,
        37.283, -121.959, -1.5e+3,
        37.262, -121.684, -4.0e+3,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:26910";

    _ModelPoints::Domain domain;
    domain.yAzimuth = 90.0;
    domain.originX = 590000.0;
    domain.originY = 4150000.0;
    domain.zBottom = -5.0e+3;
    domain.hasTopography = true;

    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, domain);

    // Adjust elevation before recomputing model coordinates (x, y, z).
    double pointsLLESquash[numPoints*3];
    for (size_t i = 0; i < numPoints; ++i) {
        const double x = _pointsXYZ[i*3+0];
        const double y = _pointsXYZ[i*3+1];
        const double elevOrig = _pointsLLE[i*3+2];
        const double elevGround = computeElevation(x, y);

        pointsLLESquash[i*3+0] = pointsLLE[i*3+0];
        pointsLLESquash[i*3+1] = pointsLLE[i*3+1];
        pointsLLESquash[i*3+2] = (elevOrig > squashMinElev) ? elevOrig + elevGround : elevOrig;
    } // for
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLESquash, numPoints, domain);
} // Constructor


// ---------------------------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::ThreeBlocksFlatPoints::ThreeBlocksFlatPoints(void) :
    ModelPoints(6) {
    const size_t numPoints = 6;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        35.3, -118.2, 0.0,
        35.5, -117.9, -45.0e+3,
        35.0, -118.1, -3.0e+3,
        35.1, -117.7, -15.0e+3,
        34.7, -117.9, -25.0e+3,
        34.7, -117.5, -8.4e+3,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:3311";

    _ModelPoints::Domain domain;
    domain.yAzimuth = 330.0;
    domain.originX = 200000.0;
    domain.originY = -400000.0;
    domain.zBottom = -45.0e+3;
    domain.hasTopography = false;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, domain);
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::ThreeBlocksTopoPoints::ThreeBlocksTopoPoints(void) :
    ModelPoints(6) {
    const size_t numPoints = 6;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        35.3, -118.2, 10.0,
        35.5, -117.9, -45.0e+3,
        35.0, -118.1, -3.0e+3,
        35.1, -117.7, -15.0e+3,
        34.7, -117.9, -25.0e+3,
        34.7, -117.5, 43.0,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:3311";

    _ModelPoints::Domain domain;
    domain.yAzimuth = 330.0;
    domain.originX = 200000.0;
    domain.originY = -400000.0;
    domain.zBottom = -45.0e+3;
    domain.hasTopography = true;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, domain);
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::ThreeBlocksSquashPoints::ThreeBlocksSquashPoints(const double squashMinElev) :
    ModelPoints(6) {
    const size_t numPoints = 6;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        35.3, -118.2, -10.0,
        35.5, -117.9, -45.0e+3,
        35.0, -118.1, -3.0e+3,
        35.1, -117.7, -15.0e+3,
        34.7, -117.9, -25.0e+3,
        34.7, -117.5, -43.0,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:3311";

    _ModelPoints::Domain domain;
    domain.yAzimuth = 330.0;
    domain.originX = 200000.0;
    domain.originY = -400000.0;
    domain.zBottom = -45.0e+3;
    domain.hasTopography = true;

    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, domain);

    // Adjust elevation before recomputing model coordinates (x, y, z).
    double pointsLLESquash[numPoints*3];
    for (size_t i = 0; i < numPoints; ++i) {
        const double x = _pointsXYZ[i*3+0];
        const double y = _pointsXYZ[i*3+1];
        const double elevOrig = _pointsLLE[i*3+2];
        const double elevGround = computeElevation(x, y);

        pointsLLESquash[i*3+0] = pointsLLE[i*3+0];
        pointsLLESquash[i*3+1] = pointsLLE[i*3+1];
        pointsLLESquash[i*3+2] = (elevOrig > squashMinElev) ? elevOrig + elevGround : elevOrig;
    } // for
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLESquash, numPoints, domain);
} // Constructor


// ---------------------------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::OutsideDomainPoints::OutsideDomainPoints(void) :
    ModelPoints(5) {
    const size_t numPoints = 5;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        34.7, -117.8, 1.0e+4,
        35.0, -117.6, -45.1e+3,
        34.3, -117.8, -3.0e+3,
        35.0, -113.0, -15.0e+3,
        42.0, -117.8, -25.0e+3,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:3311";

    _ModelPoints::Domain domain;
    domain.yAzimuth = 330.0;
    domain.originX = 200000.0;
    domain.originY = -400000.0;
    domain.zBottom = -45.0e+3;
    domain.hasTopography = false;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, domain);
} // constructor


// End of file
