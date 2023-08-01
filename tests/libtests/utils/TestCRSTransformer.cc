/**
 * C++ unit testing of geomodelgrids::utils::CRSTransformer.
 */

#include <portinfo>

#include "geomodelgrids/utils/CRSTransformer.hh" // USES CRSTransformer

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace utils {
        class TestCRSTransformer;
    } // utils
} // geomodelgrids

class geomodelgrids::utils::TestCRSTransformer {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Test constructor.
    static
    void testConstructor(void);

    /// Test factories.
    static
    void testFactories(void);

    /// Test getters.
    static
    void testAccessors(void);

    /// Test initialize().
    static
    void testInitialize(void);

    /// Test transform().
    static
    void testTransform(void);

    /// Test getCRSUnits().
    static
    void testUnits(void);

}; // class TestCRSTransformer

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestCRSTransformer::testConstructor", "[TestCRSTransformer]") {
    geomodelgrids::utils::TestCRSTransformer::testConstructor();
}
TEST_CASE("TestCRSTransformer::testFactories", "[TestCRSTransformer]") {
    geomodelgrids::utils::TestCRSTransformer::testFactories();
}
TEST_CASE("TestCRSTransformer::testAccessors", "[TestCRSTransformer]") {
    geomodelgrids::utils::TestCRSTransformer::testAccessors();
}
TEST_CASE("TestCRSTransformer::testInitialize", "[TestCRSTransformer]") {
    geomodelgrids::utils::TestCRSTransformer::testInitialize();
}
TEST_CASE("TestCRSTransformer::testTransform", "[TestCRSTransformer]") {
    geomodelgrids::utils::TestCRSTransformer::testTransform();
}
TEST_CASE("TestCRSTransformer::testUnits", "[TestCRSTransformer]") {
    geomodelgrids::utils::TestCRSTransformer::testUnits();
}

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::utils::TestCRSTransformer::testConstructor(void) {
    CRSTransformer transformer;

    CHECK(std::string("EPSG:4326") == transformer._srcString);
    CHECK(std::string("EPSG:3488") == transformer._destString);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test factories.
void
geomodelgrids::utils::TestCRSTransformer::testFactories(void) {
    CRSTransformer* transformer = CRSTransformer::createGeoToXYAxisOrder("EPSG:4326");
    REQUIRE(transformer);

    CHECK_THROWS_AS(CRSTransformer::createGeoToXYAxisOrder("nonsense"), std::runtime_error);

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
    CHECK(userSrc == transformer._srcString);
    CHECK(defaultDest == transformer._destString);

    transformer.setDest(userDest.c_str());
    CHECK(userSrc == transformer._srcString);
    CHECK(userDest == transformer._destString);
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test initialize().
void
geomodelgrids::utils::TestCRSTransformer::testInitialize(void) {
    CRSTransformer transformer;

    transformer.initialize();
    REQUIRE(transformer._proj);

    transformer.setSrc("EPSG:ABCD");
    CHECK_THROWS_AS(transformer.initialize(), std::runtime_error);
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
        CHECK_THAT(destXYZ[0], Catch::Matchers::WithinAbs(destXYE[0], fabs(tolerance*destXYE[0])));
        CHECK_THAT(destXYZ[1], Catch::Matchers::WithinAbs(destXYE[1], fabs(tolerance*destXYE[1])));
    } // 2D

    { // 3D
        const double srcLonLatElev[3] = { 37.5, -122.0, 10.0 };
        const double destXYZE[3] = { -176555.43141012415, -55540.14575705351, 10.0 };

        transformer.transform(&destXYZ[0], &destXYZ[1], &destXYZ[2],
                              srcLonLatElev[0], srcLonLatElev[1], srcLonLatElev[2]);
        const double tolerance = 1.0e-6;
        CHECK_THAT(destXYZ[0], Catch::Matchers::WithinAbs(destXYZE[0], fabs(tolerance*destXYZE[0])));
        CHECK_THAT(destXYZ[1], Catch::Matchers::WithinAbs(destXYZE[1], fabs(tolerance*destXYZE[1])));
        CHECK_THAT(destXYZ[2], Catch::Matchers::WithinAbs(destXYZE[2], fabs(tolerance*destXYZE[2])));
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
    CHECK(std::string("degree") == xUnit);
    CHECK(std::string("degree") == yUnit);
    CHECK(std::string("meter (assumed)") == zUnit);

    CRSTransformer::getCRSUnits(&xUnit, &yUnit, NULL, "EPSG:3488"); // CA Albers equal area
    CHECK(std::string("meter") == xUnit);
    CHECK(std::string("meter") == yUnit);
    CHECK(std::string("meter (assumed)") == zUnit);

    const char* crsString = "+proj=tmerc +lon_0=-123.0 +lat_0-35.0 +k_0=0.9996 +datum=NAD83 +units=km +type=crs";
    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, crsString);
    CHECK(std::string("kilometer") == xUnit);
    CHECK(std::string("kilometer") == yUnit);
    CHECK(std::string("meter (assumed)") == zUnit);

    const char* crsString2 = "+proj=aea +lat_1=34 +lat_2=40.5 +lat_0=0 +lon_0=-120 +x_0=0 +y_0=-4000000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs +type=crs";
    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, crsString2);
    CHECK(std::string("meter") == xUnit);
    CHECK(std::string("meter") == yUnit);
    CHECK(std::string("meter (assumed)") == zUnit);

    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, "EPSG:4978"); // Earth-centered Earth-fixed
    CHECK(std::string("meter") == xUnit);
    CHECK(std::string("meter") == yUnit);
    CHECK(std::string("meter") == zUnit);

    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, "EPSG:26910"); // utm zone 10N
    CHECK(std::string("meter") == xUnit);
    CHECK(std::string("meter") == yUnit);
    CHECK(std::string("meter (assumed)") == zUnit);

    CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, "EPSG:2193"); // NZ
    CHECK(std::string("meter") == xUnit);
    CHECK(std::string("meter") == yUnit);
    CHECK(std::string("meter (assumed)") == zUnit);
} // testTransform


// End of file
