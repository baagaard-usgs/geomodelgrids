/**
 * C++ unit testing of geomodelgrids::serial::Surface.
 */

#include <portinfo>

#include "ModelPoints.hh"

#include "geomodelgrids/serial/Surface.hh" // USES Surface
#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cppunit/extensions/HelperMacros.h>

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace serial {
        class TestSurface;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestSurface : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestSurface);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testSetHyperslabDims);
    CPPUNIT_TEST(testLoadMetadata);
    CPPUNIT_TEST(testQuery);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test getters.
    void testAccessors(void);

    /// Test setHyperslabDims.
    void testSetHyperslabDims(void);

    /// Test loadMetadata().
    void testLoadMetadata(void);

    /// Test query().
    void testQuery(void);

}; // class TestSurface
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestSurface);

// ---------------------------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestSurface::testConstructor(void) {
    Surface topo("top_surface");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking horizontal resolution", 0.0, topo._resolutionHoriz);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[0]", size_t(0), topo._dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[1]", size_t(0), topo._dims[1]);
} // testConstructor


// ---------------------------------------------------------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestSurface::testAccessors(void) {
    Surface topo("top_surface");

    const double resolutionHoriz(8.0);topo._resolutionHoriz = resolutionHoriz;
    const size_t dimsE[2] = { 5, 6 };
    topo._dims[0] = dimsE[0];
    topo._dims[1] = dimsE[1];

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch horizontal resolution", resolutionHoriz, topo.getResolutionHoriz());

    const size_t* const dims = topo.getDims();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in x dimension.", dimsE[0], dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in y dimension.", dimsE[1], dims[1]);
} // testAccessors


// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestSurface::testLoadMetadata(void) {
    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-block-topo.h5", H5F_ACC_RDONLY);

    Surface topo("top_surface");
    topo.loadMetadata(&h5);

    const double resolutionHoriz(10.0e+3);
    const size_t dims[2] = { 4, 5 };

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking horizontal resolution", resolutionHoriz, topo.getResolutionHoriz());
    for (size_t i = 0; i < 2; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims", dims[i], topo._dims[i]);
    } // for
} // testLoadMetadata


// ---------------------------------------------------------------------------------------------------------------------
// Test query().
void
geomodelgrids::serial::TestSurface::testQuery(void) {
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
    h5.open("../../data/one-block-topo.h5", H5F_ACC_RDONLY);

    Surface topo("top_surface");
    topo.loadMetadata(&h5);

    topo.openQuery(&h5);
    for (size_t i = 0; i < npoints; ++i) {
        const double tolerance = 1.0e-6;
        const double x = xy[i*spaceDim+0];
        const double y = xy[i*spaceDim+1];
        const double elevation = topo.query(x, y);

        const double elevationE = geomodelgrids::testdata::ModelPoints::computeTopElevation(x, y);

        std::ostringstream msg;
        msg << "Mismatch in elevation at (" << x << ", " << y << ").";
        const double toleranceV = std::max(tolerance, tolerance*fabs(elevationE));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, toleranceV);
    } // for
    topo.closeQuery();
} // testQuery


// End of file
