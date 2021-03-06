/**
 * C++ unit testing of geomodelgrids::utils::CRSTransformer.
 */

#include <portinfo>

#include "geomodelgrids/utils/CRSTransformer.hh" // USES CRSTransformer

#include <cppunit/extensions/HelperMacros.h>

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace utils {
        class TestCRSTransformer;
    } // utils
} // geomodelgrids

class geomodelgrids::utils::TestCRSTransformer : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestCRSTransformer);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testTransform);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test getters.
    void testAccessors(void);

    /// Test initialize().
    void testInitialize(void);

    /// Test transform().
    void testTransform(void);

}; // class TestCRSTransformer
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::utils::TestCRSTransformer);

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::utils::TestCRSTransformer::testConstructor(void) {
    CRSTransformer transformer;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default src coordsys.", std::string("EPSG:4326"), transformer._srcString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default dest coordsys.", std::string("EPSG:3488"), transformer._destString);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test setters.
void
geomodelgrids::utils::TestCRSTransformer::testAccessors(void) {
    CRSTransformer transformer;

    const std::string defaultSrc("EPSG:4326");
    const std::string defaultDest("EPSG:3488");

    const std::string userSrc("EPSG:1234");
    const std::string userDest("EPSG:4321");

    transformer.setSrc(userSrc.c_str());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user src coordsys.", userSrc, transformer._srcString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default dest coordsys (setting src).", defaultDest, transformer._destString);

    transformer.setDest(userDest.c_str());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user src coordsys (setting dest).", userSrc, transformer._srcString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user dest coordsys.", userDest, transformer._destString);
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test initialize().
void
geomodelgrids::utils::TestCRSTransformer::testInitialize(void) {
    CRSTransformer transformer;

    transformer.initialize();
    CPPUNIT_ASSERT_MESSAGE("CRSTransformer object not created.", transformer._proj);

    transformer.setSrc("EPSG:ABCD");
    CPPUNIT_ASSERT_THROW_MESSAGE("Invalid src coordsys should throw exception.", transformer.initialize(), std::runtime_error);
} // testInitialize


// ------------------------------------------------------------------------------------------------
// Test transform().
void
geomodelgrids::utils::TestCRSTransformer::testTransform(void) {
    double destXYZ[3];

    CRSTransformer transformer;
    transformer.initialize();

    { // 2D
        const double srcLonLat[2] = { 37.5, -122.0 };
        const double destXYE[2] = { -176555.43141012415, -55540.14575705351 };

        transformer.transform(&destXYZ[0], &destXYZ[1], NULL, srcLonLat[0], srcLonLat[1], 0.0);
        const double tolerance = 1.0e-6;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in x coordinate in 2D check.", destXYE[0], destXYZ[0],
                                             fabs(tolerance*destXYE[0]));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in y coordinate in 2D check.", destXYE[1], destXYZ[1],
                                             fabs(tolerance*destXYE[1]));
    } // 2D

    { // 3D
        const double srcLonLatElev[3] = { 37.5, -122.0, 10.0 };
        const double destXYZE[3] = { -176555.43141012415, -55540.14575705351, 10.0 };

        transformer.transform(&destXYZ[0], &destXYZ[1], &destXYZ[2],
                              srcLonLatElev[0], srcLonLatElev[1], srcLonLatElev[2]);
        const double tolerance = 1.0e-6;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in x coordinate in 3D check.", destXYZE[0], destXYZ[0],
                                             fabs(tolerance*destXYZE[0]));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in y coordinate in 3D check.", destXYZE[1], destXYZ[1],
                                             fabs(tolerance*destXYZE[1]));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in z coordinate in 3D check.", destXYZE[2], destXYZ[2],
                                             fabs(tolerance*destXYZE[2]));
    } // 3D
} // testTransform


// End of file
