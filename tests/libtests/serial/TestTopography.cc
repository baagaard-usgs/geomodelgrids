/**
 * C++ unit testing of geomodelgrids::serial::Topography.
 */

#include <portinfo>

#include "geomodelgrids/serial/Topography.hh" // USES Topography
#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace serial {
        class TestTopography;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestTopography : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestTopography);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testLoadMetadata);
    CPPUNIT_TEST(testQuery);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test getters.
    void testAccessors(void);

    /// Test loadMetadata().
    void testLoadMetadata(void);

    /// Test query().
    void testQuery(void);

}; // class TestTopography
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestTopography);

// ----------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestTopography::testConstructor(void) {
    Topography topo;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking horizontal resolution", 0.0, topo._resolutionHoriz);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[0]", size_t(0), topo._dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[1]", size_t(0), topo._dims[1]);
} // testConstructor


// ----------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestTopography::testAccessors(void) {
    Topography topo;

    const double resolutionHoriz(8.0);topo._resolutionHoriz = resolutionHoriz;
    const size_t dims[2] = { 5, 6 };
    topo._dims[0] = dims[0];
    topo._dims[1] = dims[1];

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking horizontal resolution", resolutionHoriz, topo.getResolutionHoriz());
} // testAccessors


// ----------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestTopography::testLoadMetadata(void) {
    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-topo-topo.h5", H5F_ACC_RDONLY);

    Topography topo;
    topo.loadMetadata(&h5);

    const double resolutionHoriz(10.0);
    const size_t dims[2] = { 4, 5 };

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking horizontal resolution", resolutionHoriz, topo.getResolutionHoriz());
    for (size_t i = 0; i < 2; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims", dims[i], topo._dims[i]);
    } // for
} // testLoadMetadata


// ----------------------------------------------------------------------
// Test query().
void
geomodelgrids::serial::TestTopography::testQuery(void) {
    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-topo-topo.h5", H5F_ACC_RDONLY);

    Topography topo;
    topo.loadMetadata(&h5);
    // topo.query(x, y, z, h5);

    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement testQuery().", false);
} // testQuery


// End of file
