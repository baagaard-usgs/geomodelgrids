#include <portinfo>

#include "ModelPoints.hh" // Implementation of class methods

#include "proj.h" // USES PJ

#include <cmath> // USES sin(), cos(), HUGE_VAL
#include <cassert> // USES assert()

namespace geomodelgrids {
    namespace testdata {
        class _ModelPoints {
public:

            static
            void toXYZ(double* const coordsDest,
                       const char* destString,
                       const char* srcString,
                       const double* coordsSrc,
                       const size_t numPoints,
                       const ModelPoints::Domain& domain);

        };
    } // testdata
} // geomodelgrids

// ------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::testdata::ModelPoints::ModelPoints(const size_t numPoints) :
    _pointsLLE(NULL),
    _pointsXYZ(new double[numPoints*3]),
    _numPoints(numPoints),
    _inCRS("EPSG:4326"),
    _modelCRS(NULL) {
    assert(_pointsXYZ);
} // constructor


// ------------------------------------------------------------------------------------------------
// Destructor.
geomodelgrids::testdata::ModelPoints::~ModelPoints(void) {
    delete[] _pointsXYZ;_pointsXYZ = NULL;
} // destructor


// ------------------------------------------------------------------------------------------------
// Get domain metadata.
const geomodelgrids::testdata::ModelPoints::Domain&
geomodelgrids::testdata::ModelPoints::getDomain(void) const {
    return _domain;
}


// ------------------------------------------------------------------------------------------------
// Get number of points.
size_t
geomodelgrids::testdata::ModelPoints::getNumPoints(void) const {
    return _numPoints;
} // getNumPoints


// ------------------------------------------------------------------------------------------------
// Get CRS for longitude, latitude, and elevation=.
const char*
geomodelgrids::testdata::ModelPoints::getCRSLatLonElev(void) const {
    return _inCRS;
} // getCRSLatLonElev


// ------------------------------------------------------------------------------------------------
// Get geographic coordinates of sample points.
const double*
geomodelgrids::testdata::ModelPoints::getLatLonElev(void) const {
    return _pointsLLE;
} // getLatLonElev


// ------------------------------------------------------------------------------------------------
/** Get model coordinates of sample points.
 *
 * @returns Coordinates (x, y, z) of sample points.
 */
const double*
geomodelgrids::testdata::ModelPoints::getXYZ(void) const {
    return _pointsXYZ;
} // getXYZ


// ------------------------------------------------------------------------------------------------
// Compute elevation of top surface at point.
double
geomodelgrids::testdata::ModelPoints::computeTopElevation(const double x,
                                                          const double y) {
    return 1.5e+2 + 2.0e-5 * x - 1.2e-5 * y + 5.0e-10 * x * y;
} // computeTopElevation


// ------------------------------------------------------------------------------------------------
// Compute elevation of topography/bathymetry at point.
double
geomodelgrids::testdata::ModelPoints::computeTopoBathyElevation(const double x,
                                                                const double y) {
    return 1.5e+2 + 2.0e-5 * x - 1.2e-5 * y - 5.0e-10 * x * y;
} // computeTopoBathyElevation


// ------------------------------------------------------------------------------------------------
// Compute value 'one' at point.
double
geomodelgrids::testdata::ModelPoints::computeValueOne(const double x,
                                                      const double y,
                                                      const double z) {
    return 2.0e+3 + 0.3 * x + 0.4 * y - 4.0 * z;
} // computeValueOne


// ------------------------------------------------------------------------------------------------
// Compute value 'two' at point.
double
geomodelgrids::testdata::ModelPoints::computeValueTwo(const double x,
                                                      const double y,
                                                      const double z) {
    return -1.2e+3 + 0.1 * x - 0.2 * y - 4.8 * z;
} // computeValueTwo


// ------------------------------------------------------------------------------------------------
// Compute elevation of isosurface for value 'one' at point.
double
geomodelgrids::testdata::ModelPoints::computeIsosurfaceOne(const double x,
                                                           const double y,
                                                           const double isoValue,
                                                           const double zTop,
                                                           const double zBottom) {
    const double zModel = -1.0/4.0 * (isoValue - 2.0e+3 - 0.3 * x - 0.4 * y);
    return zTop - zModel * (zTop - zBottom) / zBottom;
} // computeIsosurfaceOne


// ------------------------------------------------------------------------------------------------
// Compute elevation of isosurface for value 'one' at point.
double
geomodelgrids::testdata::ModelPoints::computeIsosurfaceTwo(const double x,
                                                           const double y,
                                                           const double isoValue,
                                                           const double zTop,
                                                           const double zBottom) {
    const double zModel = -1.0/4.8 * (isoValue + 1.2e+3 - 0.1 * x + 0.2 * y);
    return zTop - zModel * (zTop - zBottom) / zBottom;
} // computeIsosurfaceTwo


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::testdata::_ModelPoints::toXYZ(double* const coordsDest,
                                             const char* destString,
                                             const char* srcString,
                                             const double* coordsSrc,
                                             const size_t numPoints,
                                             const ModelPoints::Domain& domain) {
    assert(coordsDest);
    assert(destString);
    assert(srcString);
    assert(coordsSrc);

    PJ_CONTEXT* context = PJ_DEFAULT_CTX;
    PJ* proj = proj_create_crs_to_crs(context, srcString, destString, NULL);assert(proj);

    const size_t dim = 3;
    for (size_t i = 0; i < numPoints; ++i) {
        PJ_COORD xySrc = proj_coord(coordsSrc[i*dim+0], coordsSrc[i*dim+1], 0.0, HUGE_VAL);
        PJ_COORD xyDest = proj_trans(proj, PJ_FWD, xySrc);
        const double x = xyDest.xyzt.x - domain.originX;
        const double y = xyDest.xyzt.y - domain.originY;

        const double yAzimuthRad = domain.yAzimuth * M_PI / 180.0;
        coordsDest[i*dim+0] = x * cos(yAzimuthRad) - y * sin(yAzimuthRad);
        coordsDest[i*dim+1] = x * sin(yAzimuthRad) + y * cos(yAzimuthRad);

        double zGroundSurf = 0.0;
        if (domain.hasTopSurface) {
            zGroundSurf = ModelPoints::computeTopElevation(coordsDest[i*dim+0], coordsDest[i*dim+1]);
        } // if
        coordsDest[i*dim+2] = domain.zBottom * (zGroundSurf - coordsSrc[i*dim+2]) / (zGroundSurf - domain.zBottom);
    } // for

    proj_destroy(proj);proj = NULL;
} // toXYZ


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::OneBlockFlatPoints::OneBlockFlatPoints(void) :
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

    _domain.yAzimuth = 90.0;
    _domain.originX = 590000.0;
    _domain.originY = 4150000.0;
    _domain.zBottom = -5.0e+3;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);
} // Constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::OneBlockFlatBorehole::OneBlockFlatBorehole(void) :
    ModelPoints(6) {
    const size_t numPoints = 6;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        37.381, -121.581, 0.0,
        37.381, -121.581, -1.0e+3,
        37.381, -121.581, -2.0e+3,
        37.381, -121.581, -3.0e+3,
        37.381, -121.581, -4.0e+3,
        37.381, -121.581, -5.0e+3,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:26910";

    _domain.yAzimuth = 90.0;
    _domain.originX = 590000.0;
    _domain.originY = 4150000.0;
    _domain.zBottom = -5.0e+3;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);
} // Constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::OneBlockFlatIsosurface::OneBlockFlatIsosurface(void) :
    ModelPoints(6) {
    const size_t numPoints = 6;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        37.325, -121.775, 0.0,
        37.325, -121.725, 0.0,
        37.325, -121.675, 0.0,
        37.375, -121.775, 0.0,
        37.375, -121.725, 0.0,
        37.375, -121.675, 0.0,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:26910";

    _domain.yAzimuth = 90.0;
    _domain.originX = 590000.0;
    _domain.originY = 4150000.0;
    _domain.zBottom = -5.0e+3;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);
} // Constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::OneBlockTopoPoints::OneBlockTopoPoints(void) :
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

    _domain.yAzimuth = 90.0;
    _domain.originX = 590000.0;
    _domain.originY = 4150000.0;
    _domain.zBottom = -5.0e+3;
    _domain.hasTopSurface = true;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);
} // Constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::OneBlockSquashPoints::OneBlockSquashPoints(const double squashMinElev) :
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

    _domain.yAzimuth = 90.0;
    _domain.originX = 590000.0;
    _domain.originY = 4150000.0;
    _domain.zBottom = -5.0e+3;
    _domain.hasTopSurface = true;

    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);

    // Adjust elevation before recomputing model coordinates (x, y, z).
    double pointsLLESquash[numPoints*3];
    for (size_t i = 0; i < numPoints; ++i) {
        const double x = _pointsXYZ[i*3+0];
        const double y = _pointsXYZ[i*3+1];
        const double elevOrig = _pointsLLE[i*3+2];
        const double elevTopSurface = computeTopElevation(x, y);

        pointsLLESquash[i*3+0] = pointsLLE[i*3+0];
        pointsLLESquash[i*3+1] = pointsLLE[i*3+1];
        pointsLLESquash[i*3+2] = (elevOrig > squashMinElev) ? elevOrig + elevTopSurface : elevOrig;
    } // for
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLESquash, numPoints, _domain);
} // Constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::ThreeBlocksFlatPoints::ThreeBlocksFlatPoints(void) :
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

    _domain.yAzimuth = 330.0;
    _domain.originX = 200000.0;
    _domain.originY = -400000.0;
    _domain.zBottom = -45.0e+3;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);
} // constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::ThreeBlocksTopoPoints::ThreeBlocksTopoPoints(void) :
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

    _domain.yAzimuth = 330.0;
    _domain.originX = 200000.0;
    _domain.originY = -400000.0;
    _domain.zBottom = -45.0e+3;
    _domain.hasTopSurface = true;
    _domain.hasTopoBathy = true;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);
} // constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::ThreeBlocksTopoBorehole::ThreeBlocksTopoBorehole(void) :
    ModelPoints(6) {
    const size_t numPoints = 6;assert(_numPoints == numPoints);
    const double groundSurf = 151.69036995974;
    static const double pointsLLE[numPoints*3] = {
        35.1, -117.7, 0.0 + groundSurf,
        35.1, -117.7, -5.0e+3 + groundSurf,
        35.1, -117.7, -10.0e+3 + groundSurf,
        35.1, -117.7, -15.0e+3 + groundSurf,
        35.1, -117.7, -20.0e+3 + groundSurf,
        35.1, -117.7, -25.0e+3 + groundSurf,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:3311";

    _domain.yAzimuth = 330.0;
    _domain.originX = 200000.0;
    _domain.originY = -400000.0;
    _domain.zBottom = -45.0e+3;
    _domain.hasTopSurface = true;
    _domain.hasTopoBathy = true;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);
} // constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::ThreeBlocksTopoIsosurface::ThreeBlocksTopoIsosurface(void) :
    ModelPoints(8) {
    const size_t numPoints = 8;assert(_numPoints == numPoints);
    static const double pointsLLE[numPoints*3] = {
        34.65, -117.65, 0.0,
        34.65, -117.55, 0.0,
        34.65, -117.45, 0.0,
        34.65, -117.35, 0.0,
        34.75, -117.65, 0.0,
        34.75, -117.55, 0.0,
        34.75, -117.45, 0.0,
        34.75, -117.35, 0.0,
    };_pointsLLE = pointsLLE;
    _modelCRS = "EPSG:3311";

    _domain.yAzimuth = 330.0;
    _domain.originX = 200000.0;
    _domain.originY = -400000.0;
    _domain.zBottom = -45.0e+3;
    _domain.hasTopSurface = true;
    _domain.hasTopoBathy = true;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);
} // constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::ThreeBlocksSquashTopPoints::ThreeBlocksSquashTopPoints(const double squashMinElev) :
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

    _domain.yAzimuth = 330.0;
    _domain.originX = 200000.0;
    _domain.originY = -400000.0;
    _domain.zBottom = -45.0e+3;
    _domain.hasTopSurface = true;
    _domain.hasTopoBathy = true;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);

    // Adjust elevation before recomputing model coordinates (x, y, z).
    double pointsLLESquash[numPoints*3];
    for (size_t i = 0; i < numPoints; ++i) {
        const double x = _pointsXYZ[i*3+0];
        const double y = _pointsXYZ[i*3+1];
        const double elevOrig = _pointsLLE[i*3+2];
        const double elevGround = computeTopElevation(x, y);

        pointsLLESquash[i*3+0] = pointsLLE[i*3+0];
        pointsLLESquash[i*3+1] = pointsLLE[i*3+1];
        pointsLLESquash[i*3+2] = (elevOrig > squashMinElev) ? elevOrig + elevGround : elevOrig;
    } // for
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLESquash, numPoints, _domain);
} // Constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::ThreeBlocksSquashTopoBathyPoints::ThreeBlocksSquashTopoBathyPoints(const double squashMinElev) :
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

    _domain.yAzimuth = 330.0;
    _domain.originX = 200000.0;
    _domain.originY = -400000.0;
    _domain.zBottom = -45.0e+3;
    _domain.hasTopSurface = true;
    _domain.hasTopoBathy = true;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);

    // Adjust elevation before recomputing model coordinates (x, y, z).
    double pointsLLESquash[numPoints*3];
    for (size_t i = 0; i < numPoints; ++i) {
        const double x = _pointsXYZ[i*3+0];
        const double y = _pointsXYZ[i*3+1];
        const double elevOrig = _pointsLLE[i*3+2];
        const double elevTopoBathy = computeTopoBathyElevation(x, y);

        pointsLLESquash[i*3+0] = pointsLLE[i*3+0];
        pointsLLESquash[i*3+1] = pointsLLE[i*3+1];
        pointsLLESquash[i*3+2] = (elevOrig > squashMinElev) ? elevOrig + elevTopoBathy : elevOrig;
    } // for
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLESquash, numPoints, _domain);
} // Constructor


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::testdata::OutsideDomainPoints::OutsideDomainPoints(void) :
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

    _domain.yAzimuth = 330.0;
    _domain.originX = 200000.0;
    _domain.originY = -400000.0;
    _domain.zBottom = -45.0e+3;
    _ModelPoints::toXYZ(_pointsXYZ, _modelCRS, _inCRS, pointsLLE, numPoints, _domain);
} // constructor


// End of file
