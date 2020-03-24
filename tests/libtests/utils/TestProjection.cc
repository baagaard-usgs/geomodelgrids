/**
 * C++ unit testing of geomodelgrids::utils::Projection.
 */

#include <portinfo>

#include "geomodelgrids/utils/Projection.hh" // USES Projection

#include <cppunit/extensions/HelperMacros.h>

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace utils {
        class TestProjection;
    } // utils
} // geomodelgrids

class geomodelgrids::utils::TestProjection : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestProjection);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testProject);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test getters.
    void testAccessors(void);

    /// Test initialize().
    void testInitialize(void);

    /// Test project().
    void testProject(void);

}; // class TestProjection
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::utils::TestProjection);

// ----------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::utils::TestProjection::testConstructor(void) {
    Projection proj;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default src coordsys.", std::string("EPSG:4326"), proj._srcString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default dest coordsys.", std::string("EPSG:3488"), proj._destString);
} // testConstructor


// ----------------------------------------------------------------------
// Test setters.
void
geomodelgrids::utils::TestProjection::testAccessors(void) {
    Projection proj;

    const std::string defaultSrc("EPSG:4326");
    const std::string defaultDest("EPSG:3488");

    const std::string userSrc("EPSG:1234");
    const std::string userDest("EPSG:4321");

    proj.setSrc(userSrc.c_str());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user src coordsys.", userSrc, proj._srcString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default dest coordsys (setting src).", defaultDest, proj._destString);

    proj.setDest(userDest.c_str());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user src coordsys (setting dest).", userSrc, proj._srcString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user dest coordsys.", userDest, proj._destString);
} // testAccessors


// ----------------------------------------------------------------------
// Test initialize().
void
geomodelgrids::utils::TestProjection::testInitialize(void) {
    Projection proj;

    proj.initialize();
    CPPUNIT_ASSERT_MESSAGE("Projection object not created.", proj._proj);

    proj.setSrc("EPSG:ABCD");
    CPPUNIT_ASSERT_THROW_MESSAGE("Invalid src coordsys should throw exception.", proj.initialize(), std::runtime_error);
} // testInitialize


// ----------------------------------------------------------------------
// Test project().
void
geomodelgrids::utils::TestProjection::testProject(void) {
    Projection proj;

    const double srcLonLat[2] = { 37.5, -122.0 };
    const double destXYE[2] = { -176555.43141012415, -55540.14575705351 };
    double destXY[2];

    proj.initialize();
    proj.project(&destXY[0], &destXY[1], srcLonLat[0], srcLonLat[1]);
    const double tolerance = 1.0e-6;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in x coordinate.", destXYE[0], destXY[0], fabs(tolerance*destXYE[0]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in y coordinate.", destXYE[1], destXY[1], fabs(tolerance*destXYE[1]));
} // testProject


// End of file
