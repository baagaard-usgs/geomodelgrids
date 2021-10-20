/**
 * C++ unit testing of geomodelgrids::serial::Surface.
 */

#include <portinfo>

#include "TestSurface.hh" // Implementation of class methods.

#include "ModelPoints.hh" // USES ModelPoints

#include "geomodelgrids/serial/Surface.hh" // USES Surface
#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/utils/Indexing.hh" // USES Indexing

#include <cmath> // USES fabs()

// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::TestSurface::setUp(void) {
    _data = new TestSurface_Data();
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::TestSurface::tearDown(void) {
    delete _data;_data = NULL;
}


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestSurface::testConstructor(void) {
    const std::string surfaceName("top_surface");
    Surface surf(surfaceName.c_str());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking name", surfaceName, surf._name);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking x resolution", 0.0, surf._resolutionX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y resolution", 0.0, surf._resolutionY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking x coordinates", (double*)NULL, surf._coordinatesX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y coordinates", (double*)NULL, surf._coordinatesY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checknig x indexing", (geomodelgrids::utils::Indexing*)NULL, surf._indexingX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checknig y indexing", (geomodelgrids::utils::Indexing*)NULL, surf._indexingY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[0]", size_t(0), surf._dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[1]", size_t(0), surf._dims[1]);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestSurface::testAccessors(void) {
    CPPUNIT_ASSERT(_data);

    Surface surf("top_surface");

    surf._resolutionX = _data->resolutionX;
    surf._resolutionY = _data->resolutionY;
    surf._dims[0] = _data->numX;
    surf._dims[1] = _data->numY;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch x resolution", _data->resolutionX, surf.getResolutionX());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch y resolution", _data->resolutionY, surf.getResolutionY());

    const size_t* const dims = surf.getDims();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in x dimension.", _data->numX, dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in y dimension.", _data->numY, dims[1]);

    surf._coordinatesX = _data->coordinatesX;
    surf._coordinatesY = _data->coordinatesY;
    const double tolerance = 1.0e-6;
    double* x = surf.getCoordinatesX();
    if (_data->coordinatesX) {
        for (size_t i = 0; i < _data->numX; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in x coordinates", _data->coordinatesX[i], x[i], tolerance);
        } // for
    } // if

    if (_data->coordinatesY) {
        double* y = surf.getCoordinatesY();
        for (size_t i = 0; i < _data->numY; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in y coordinates", _data->coordinatesY[i], y[i], tolerance);
        } // for
    } // if

    // Memory managed by _data
    surf._coordinatesX = NULL;
    surf._coordinatesY = NULL;
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test setHyperslabDims.
void
geomodelgrids::serial::TestSurface::testSetHyperslabDims(void) {
    Surface topo("top_surface");

    const size_t ndims = 3;
    const size_t dimsDefault[ndims] = { 128, 128, 1 };
    for (size_t i = 0; i < ndims; ++i) {
        std::ostringstream msg;
        msg << "Mismath in default hyperslab dim " << i << ".";
        CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str().c_str(), dimsDefault[i], topo._hyperslabDims[i]);
    } // for

    const size_t dims[ndims] = { 12, 12, 1 };
    topo.setHyperslabDims(dims, ndims-1);
    for (size_t i = 0; i < ndims; ++i) {
        std::ostringstream msg;
        msg << "Mismath in user hyperslab dim " << i << ".";
        CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str().c_str(), dims[i], topo._hyperslabDims[i]);
    } // for

    CPPUNIT_ASSERT_THROW(topo.setHyperslabDims(dims, 5), std::length_error);
} // testSetHyperslabDims


// ------------------------------------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestSurface::testLoadMetadata(void) {
    CPPUNIT_ASSERT(_data);

    geomodelgrids::serial::HDF5 h5;
    h5.open(_data->filename, H5F_ACC_RDONLY);

    Surface surf("top_surface");
    surf.loadMetadata(&h5);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking x resolution", _data->resolutionX, surf.getResolutionX());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y resolution", _data->resolutionY, surf.getResolutionY());

    const size_t* const dims = surf.getDims();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in x dimension.", _data->numX, dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in y dimension.", _data->numY, dims[1]);

    const double tolerance = 1.0e-6;
    double* x = surf.getCoordinatesX();
    if (_data->coordinatesX) {
        for (size_t i = 0; i < _data->numX; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in x coordinates", _data->coordinatesX[i], x[i], tolerance);
        } // for
    } // if

    if (_data->coordinatesY) {
        double* y = surf.getCoordinatesY();
        for (size_t i = 0; i < _data->numY; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in y coordinates", _data->coordinatesY[i], y[i], tolerance);
        } // for
    } // if

    CPPUNIT_ASSERT_MESSAGE("Mismatch in x indexing", surf._indexingX);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in y indexing", surf._indexingY);
} // testLoadMetadata


// ------------------------------------------------------------------------------------------------
// Test loadMetadata() with bad variable resolution data.
void
geomodelgrids::serial::TestSurface::testLoadBadMetadata(void) {
    CPPUNIT_ASSERT(_data);

    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-block-topo-varxy-bad-surf-coords.h5", H5F_ACC_RDONLY);

    Surface surf("top_surface");
    CPPUNIT_ASSERT_THROW_MESSAGE("Failed to detect wrong number of coordinates.",
                                 surf.loadMetadata(&h5), std::runtime_error);
} // testLoadBadMetadata


// ------------------------------------------------------------------------------------------------
// Test query() for surface with uniform resolution.
void
geomodelgrids::serial::TestSurface::testQuery(void) {
    CPPUNIT_ASSERT(_data);

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

        std::ostringstream msg;
        msg << "Mismatch in elevation at (" << x << ", " << y << ").";
        const double toleranceV = std::max(tolerance, tolerance*fabs(elevationE));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, toleranceV);
    } // for
    surf.closeQuery();
} // testQuery


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestSurface_Data::TestSurface_Data(void) :
    filename(NULL),
    resolutionX(0.0),
    resolutionY(0.0),
    coordinatesX(NULL),
    coordinatesY(NULL),
    numX(0),
    numY(0) {}


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestSurface_Data::~TestSurface_Data(void) {}


// End of file
