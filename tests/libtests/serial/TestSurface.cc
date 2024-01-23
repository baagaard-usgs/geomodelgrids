/**
 * C++ unit testing of geomodelgrids::serial::Surface.
 */

#include <portinfo>

#include "TestSurface.hh" // Implementation of class methods.

#include "tests/data/ModelPoints.hh" // USES ModelPoints

#include "geomodelgrids/serial/Surface.hh" // USES Surface
#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/utils/Indexing.hh" // USES Indexing

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()
#include <cassert>

// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestSurface::TestSurface(TestSurface_Data* data) :
    _data(data) {
    assert(_data);
}


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestSurface::~TestSurface(void) {
    delete _data;_data = nullptr;
}


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestSurface::testConstructor(void) {
    const std::string surfaceName("top_surface");
    Surface surf(surfaceName.c_str());

    CHECK(surfaceName == surf._name);
    CHECK(0.0 == surf._resolutionX);
    CHECK(0.0 == surf._resolutionY);
    CHECK((double*)nullptr == surf._coordinatesX);
    CHECK((double*)nullptr == surf._coordinatesY);
    CHECK((geomodelgrids::utils::Indexing*)nullptr == surf._indexingX);
    CHECK((geomodelgrids::utils::Indexing*)nullptr == surf._indexingY);
    CHECK(size_t(0) == surf._dims[0]);
    CHECK(size_t(0) == surf._dims[1]);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test setHyperslabDims.
void
geomodelgrids::serial::TestSurface::testSetHyperslabDims(void) {
    Surface topo("top_surface");

    const size_t ndims = 3;
    const size_t dimsDefault[ndims] = { 128, 128, 1 };
    for (size_t i = 0; i < ndims; ++i) {
        CHECK(dimsDefault[i] == topo._hyperslabDims[i]);
    } // for

    const size_t dims[ndims] = { 12, 12, 1 };
    topo.setHyperslabDims(dims, ndims-1);
    for (size_t i = 0; i < ndims; ++i) {
        CHECK(dims[i] == topo._hyperslabDims[i]);
    } // for

    CHECK_THROWS_AS(topo.setHyperslabDims(dims, 5), std::length_error);
} // testSetHyperslabDims


// ------------------------------------------------------------------------------------------------
// Test loadMetadata() with bad variable resolution data.
void
geomodelgrids::serial::TestSurface::testLoadBadMetadata(void) {
    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-block-topo-varxy-bad-surf-coords.h5", H5F_ACC_RDONLY);

    Surface surf("top_surface");
    CHECK_THROWS_AS(surf.loadMetadata(&h5), std::runtime_error);
} // testLoadBadMetadata


// ------------------------------------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestSurface::testAccessors(void) {
    REQUIRE(_data);

    Surface surf("top_surface");

    surf._resolutionX = _data->resolutionX;
    surf._resolutionY = _data->resolutionY;
    surf._dims[0] = _data->numX;
    surf._dims[1] = _data->numY;

    CHECK(_data->resolutionX == surf.getResolutionX());
    CHECK(_data->resolutionY == surf.getResolutionY());

    const size_t* const dims = surf.getDims();
    REQUIRE(_data->numX == dims[0]);
    REQUIRE(_data->numY == dims[1]);

    surf._coordinatesX = _data->coordinatesX;
    surf._coordinatesY = _data->coordinatesY;
    const double tolerance = 1.0e-6;
    double* x = surf.getCoordinatesX();
    if (_data->coordinatesX) {
        for (size_t i = 0; i < _data->numX; ++i) {
            CHECK_THAT(x[i], Catch::Matchers::WithinAbs(_data->coordinatesX[i], tolerance));
        } // for
    } // if

    if (_data->coordinatesY) {
        double* y = surf.getCoordinatesY();
        for (size_t i = 0; i < _data->numY; ++i) {
            CHECK_THAT(y[i], Catch::Matchers::WithinAbs(_data->coordinatesY[i], tolerance));
        } // for
    } // if

    // Memory managed by _data
    surf._coordinatesX = nullptr;
    surf._coordinatesY = nullptr;
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestSurface::testLoadMetadata(void) {
    REQUIRE(_data);

    geomodelgrids::serial::HDF5 h5;
    h5.open(_data->filename, H5F_ACC_RDONLY);

    Surface surf("top_surface");
    surf.loadMetadata(&h5);

    CHECK(_data->resolutionX == surf.getResolutionX());
    CHECK(_data->resolutionY == surf.getResolutionY());

    const size_t* const dims = surf.getDims();
    REQUIRE(_data->numX == dims[0]);
    REQUIRE(_data->numY == dims[1]);

    const double tolerance = 1.0e-6;
    double* x = surf.getCoordinatesX();
    if (_data->coordinatesX) {
        for (size_t i = 0; i < _data->numX; ++i) {
            CHECK_THAT(x[i], Catch::Matchers::WithinAbs(_data->coordinatesX[i], tolerance));
        } // for
    } // if

    if (_data->coordinatesY) {
        double* y = surf.getCoordinatesY();
        for (size_t i = 0; i < _data->numY; ++i) {
            CHECK_THAT(y[i], Catch::Matchers::WithinAbs(_data->coordinatesY[i], tolerance));
        } // for
    } // if

    CHECK(surf._indexingX);
    CHECK(surf._indexingY);
} // testLoadMetadata


// ------------------------------------------------------------------------------------------------
// Test query() for surface with uniform resolution.
void
geomodelgrids::serial::TestSurface::testQuery(void) {
    REQUIRE(_data);

    const size_t npoints = 5;
    const size_t spaceDim = 2;
    const double xy[npoints*spaceDim] = {
        2.0e+3, 1.2e+3,
        22.0e+3, 0.0e+3,
        0.2e+3, 34.0e+3,
        17.0e+3, 25.0e+3,
        29.0e+3, 40.0e+3,
    };

    geomodelgrids::serial::HDF5 h5;
    h5.open(_data->filename, H5F_ACC_RDONLY);

    Surface surf("top_surface");
    surf.loadMetadata(&h5);

    surf.openQuery(&h5);
    for (size_t i = 0; i < npoints; ++i) {
        const double tolerance = 1.0e-6;
        const double x = xy[i*spaceDim+0];
        const double y = xy[i*spaceDim+1];
        const double elevation = surf.query(x, y);

        const double elevationE = geomodelgrids::testdata::ModelPoints::computeTopElevation(x, y);

        INFO("Mismatch in elevation at (" << x << ", " << y << ").");
        const double toleranceV = std::max(tolerance, tolerance*fabs(elevationE));
        CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, toleranceV));
    } // for
    surf.closeQuery();
} // testQuery


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestSurface_Data::TestSurface_Data(void) :
    filename(nullptr),
    resolutionX(0.0),
    resolutionY(0.0),
    coordinatesX(nullptr),
    coordinatesY(nullptr),
    numX(0),
    numY(0) {}


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestSurface_Data::~TestSurface_Data(void) {}


// End of file
