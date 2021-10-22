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
    CPPUNIT_TEST(testFactories);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testTransform);
    CPPUNIT_TEST(testUnits);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test factories.
    void testFactories(void);

    /// Test getters.
    void testAccessors(void);

    /// Test initialize().
    void testInitialize(void);

    /// Test transform().
    void testTransform(void);

    /// Test getCRSUnits().
    void testUnits(void);

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
// Test factories.
void
geomodelgrids::utils::TestCRSTransformer::testFactories(void) {
    CRSTransformer* transformer = CRSTransformer::createGeoToXYAxisOrder("EPSG:4326");
    CPPUNIT_ASSERT_MESSAGE("createGeoToXYAxisOrder() failed with valid CRS.", transformer);

    CPPUNIT_ASSERT_THROW_MESSAGE("createGeoToXYAxisOrder() did not fail with invalid CRS.",
                                 CRSTransformer::createGeoToXYAxisOrder("nonsense"),
                                 std::runtime_error);

    delete transformer;transformer = NULL;
} // testFactories


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


// ------------------------------------------------------------------------------------------------
// Test getCRSUnits().
void
geomodelgrids::utils::TestCRSTransformer::testUnits(void) {
    std::string xUnit = "unknown";
    std::string yUnit = "unknown";
    std::string zUnit = "unknown";

    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, "EPSG:4326");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:4326 x axis unit", std::string("degree"), xUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:4326 y axis unit", std::string("degree"), yUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:4326 z axis unit", std::string("meter (assumed)"), zUnit);

    CRSTransformer::getCRSUnits(&xUnit, &yUnit, NULL, "EPSG:3488"); // CA Albers equal area
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:3488 x axis unit", std::string("meter"), xUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:3488 y axis unit", std::string("meter"), yUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:3488 y axis unit", std::string("meter (assumed)"), zUnit);

    const char* crsString = "+proj=tmerc +lon_0=-123.0 +lat_0-35.0 +k_0=0.9996 +datum=NAD83 +units=km +type=crs";
    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, crsString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in tmerc projection x axis unit", std::string("kilometer"), xUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in tmerc projection y axis unit", std::string("kilometer"), yUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in tmerc projection z axis unit", std::string("meter (assumed)"), zUnit);

    const char* crsString2 = "+proj=aea +lat_1=34 +lat_2=40.5 +lat_0=0 +lon_0=-120 +x_0=0 +y_0=-4000000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs +type=crs";
    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, crsString2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in aea projection x axis unit", std::string("meter"), xUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in aea projection y axis unit", std::string("meter"), yUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in aea projection y axis unit", std::string("meter (assumed)"), zUnit);

    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, "EPSG:4978"); // Earth-centered Earth-fixed
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:4978 x axis unit", std::string("meter"), xUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:4978 y axis unit", std::string("meter"), yUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:4978 y axis unit", std::string("meter"), zUnit);

    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, "EPSG:26910"); // utm zone 10N
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:26910 x axis unit", std::string("meter"), xUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:26910 y axis unit", std::string("meter"), yUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:26910 y axis unit", std::string("meter (assumed)"), zUnit);

    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, "EPSG:2193"); // NZ
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:2193 x axis unit", std::string("meter"), xUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:2193 y axis unit", std::string("meter"), yUnit);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in EPSG:2193 y axis unit", std::string("meter (assumed)"), zUnit);
} // testTransform


// End of file
