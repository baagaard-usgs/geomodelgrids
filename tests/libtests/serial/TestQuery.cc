/**
 * C++ unit testing of geomodelgrids::serial::Query.
 */

#include <portinfo>

#include "tests/data/ModelPoints.hh"

#include "geomodelgrids/serial/Query.hh" // USES Query
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath>

namespace geomodelgrids {
    namespace serial {
        class TestQuery;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestQuery {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Test constructor.
    static
    void testConstructor(void);

    /// Test getters.
    static
    void testAccessors(void);

    /// Test initialize() and finalize().
    static
    void testInitialize(void);

    /// Test queryTopElevation().
    static
    void testQueryTopElevation(void);

    /// Test queryTopoBathyElevation().
    static
    void testQueryTopoBathyElevation(void);

    /// Test query() for model with flat ground surface.
    static
    void testQueryFlat(void);

    /// Test query() for model with topography.
    static
    void testQueryTopo(void);

    /// Test query() for model using top surface for squashing.
    static
    void testQuerySquashTop(void);

    /// Test query() for model using topography/bathymetry for squashing.
    static
    void testQuerySquashTopoBathy(void);

}; // class TestQuery

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestQuery::testConstructor", "[TestQuery]") {
    geomodelgrids::serial::TestQuery::testConstructor();
}
TEST_CASE("TestQuery::testAccessors", "[TestQuery]") {
    geomodelgrids::serial::TestQuery::testAccessors();
}
TEST_CASE("TestQuery::testInitialize", "[TestQuery]") {
    geomodelgrids::serial::TestQuery::testInitialize();
}
TEST_CASE("TestQuery::testQueryTopElevation", "[TestQuery]") {
    geomodelgrids::serial::TestQuery::testQueryTopElevation();
}
TEST_CASE("TestQuery::testQueryTopoBathyElevation", "[TestQuery]") {
    geomodelgrids::serial::TestQuery::testQueryTopoBathyElevation();
}
TEST_CASE("TestQuery::testQueryFlat", "[TestQuery]") {
    geomodelgrids::serial::TestQuery::testQueryFlat();
}
TEST_CASE("TestQuery::testQueryTopo", "[TestQuery]") {
    geomodelgrids::serial::TestQuery::testQueryTopo();
}
TEST_CASE("TestQuery::testQuerySquashTop", "[TestQuery]") {
    geomodelgrids::serial::TestQuery::testQuerySquashTop();
}
TEST_CASE("TestQuery::testQuerySquashTopoBathy", "[TestQuery]") {
    geomodelgrids::serial::TestQuery::testQuerySquashTopoBathy();
}

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestQuery::testConstructor(void) {
    Query query;

    CHECK(0.0 == query._squashMinElev);
    CHECK(Query::SQUASH_NONE == query._squash);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test setters.
void
geomodelgrids::serial::TestQuery::testAccessors(void) {
    Query query;

    const double minElev(-2.0e+3);
    query.setSquashMinElev(minElev);
    CHECK(Query::SQUASH_TOP_SURFACE == query._squash);
    CHECK(minElev == query._squashMinElev);

    query.setSquashing(Query::SQUASH_TOPOGRAPHY_BATHYMETRY);
    CHECK(Query::SQUASH_TOPOGRAPHY_BATHYMETRY == query._squash);
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test initialize().
void
geomodelgrids::serial::TestQuery::testInitialize(void) {
    const size_t numModels = 2;
    const char* const filenamesArray[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };
    std::vector<std::string> filenames(filenamesArray, filenamesArray+numModels);

    const size_t numValues = 2;
    const char* const valueNamesArray[numValues] = { "two", "one" };
    std::vector<std::string> valueNames(valueNamesArray, valueNamesArray+numValues);
    const size_t valueIndexE[numValues] = { 1, 0 };

    const char* const crs = "EPSG:4326";

    Query query;
    query.initialize(filenames, valueNames, crs);

    REQUIRE(numModels == query._models.size());
    REQUIRE(numModels == query._valuesIndex.size());
    for (size_t iModel = 0; iModel < numModels; ++iModel) {
        REQUIRE(query._models[iModel]);

        REQUIRE(numValues == query._valuesIndex[iModel].size());
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            CHECK(valueIndexE[iValue] == query._valuesIndex[iModel][iValue]);
        } // for

    } // for

    CHECK_NOTHROW(query.initialize(filenames, valueNames, crs));
    query.finalize();

    // Bad value
    const char* const valueNamesArrayBad[numValues] = { "two", "blah" };
    std::vector<std::string> valueNamesBad(valueNamesArrayBad, valueNamesArrayBad+numValues);
    CHECK_THROWS_AS(query.initialize(filenames, valueNamesBad, crs), std::invalid_argument);
} // testInitialize


// ------------------------------------------------------------------------------------------------
// Test queryTopElevation().
void
geomodelgrids::serial::TestQuery::testQueryTopElevation(void) {
    const size_t numModels = 2;
    const char* const filenamesArray[numModels] = {
        "../../data/one-block-flat.h5",
        "../../data/three-blocks-topo.h5",
    };
    std::vector<std::string> filenames(filenamesArray, filenamesArray+numModels);

    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    Query query;
    std::vector<std::string> valueNames;
    query.initialize(filenames, valueNames, crs);

    const double tolerance = 1.0e-5;
    { // One Block Flat
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const double elevationE = 0.0;
            const double elevation = query.queryTopElevation(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1]);

            INFO("Mismatch for point in one-block-flat ("
                 << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, valueTolerance));
        } // for
    } // One Block Flat

    { // Three Blocks Topo
        geomodelgrids::testdata::ThreeBlocksTopoPoints pointsThree;
        const size_t numPoints = pointsThree.getNumPoints();
        const double* pointsLLE = pointsThree.getLatLonElev();
        const double* pointsXYZ = pointsThree.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const double elevationE = pointsThree.computeTopElevation(pointsXYZ[iPt*spaceDim+0], pointsXYZ[iPt*spaceDim+1]);
            const double elevation = query.queryTopElevation(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1]);

            INFO("Mismatch for point in three-blocks-topo ("
                 << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, valueTolerance));
        } // for
    } // Three Blocks Topo

    { // Outside domains
        geomodelgrids::testdata::OutsideDomainPoints pointsOutside;
        const size_t numPoints = pointsOutside.getNumPoints();
        const double* pointsLLE = pointsOutside.getLatLonElev();

        // Skip first 2 points with z causing point to be outside domain.
        for (size_t iPt = 2; iPt < numPoints; ++iPt) {
            const double elevationE = NODATA_VALUE;
            const double elevation = query.queryTopElevation(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1]);

            INFO("Mismatch for point outside domains ("
                 << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, valueTolerance));
        } // for
    } // Outside domains
} // testQueryTopElevation


// ------------------------------------------------------------------------------------------------
// Test queryTopoBathyElevation().
void
geomodelgrids::serial::TestQuery::testQueryTopoBathyElevation(void) {
    const size_t numModels = 1;
    const char* const filenamesArray[numModels] = {
        "../../data/three-blocks-topo.h5",
    };
    std::vector<std::string> filenames(filenamesArray, filenamesArray+numModels);

    geomodelgrids::testdata::ThreeBlocksTopoPoints pointsThree;
    const std::string& crs = pointsThree.getCRSLatLonElev();
    const size_t spaceDim = 3;

    Query query;
    std::vector<std::string> valueNames;
    query.initialize(filenames, valueNames, crs);

    const double tolerance = 1.0e-5;
    { // Three Blocks Topo
        const size_t numPoints = pointsThree.getNumPoints();
        const double* pointsLLE = pointsThree.getLatLonElev();
        const double* pointsXYZ = pointsThree.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const double elevationE = pointsThree.computeTopoBathyElevation(pointsXYZ[iPt*spaceDim+0], pointsXYZ[iPt*spaceDim+1]);
            const double elevation = query.queryTopoBathyElevation(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1]);

            INFO("Mismatch for point in three-blocks-topo ("
                 << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, valueTolerance));
        } // for
    } // Three Blocks Topo

    { // Outside domains
        geomodelgrids::testdata::OutsideDomainPoints pointsOutside;
        const size_t numPoints = pointsOutside.getNumPoints();
        const double* pointsLLE = pointsOutside.getLatLonElev();

        // Skip first 2 points with z causing point to be outside domain.
        for (size_t iPt = 2; iPt < numPoints; ++iPt) {
            const double elevationE = NODATA_VALUE;
            const double elevation = query.queryTopoBathyElevation(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1]);

            INFO("Mismatch for point outside domains ("
                 << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, valueTolerance));
        } // for
    } // Outside domains
} // testQueryTopoBathyElevation


// ------------------------------------------------------------------------------------------------
// Test query() for model with flat ground surface.
void
geomodelgrids::serial::TestQuery::testQueryFlat(void) {
    const size_t numModels = 2;
    const char* const filenamesArray[numModels] = {
        "../../data/one-block-flat.h5",
        "../../data/three-blocks-flat.h5",
    };
    std::vector<std::string> filenames(filenamesArray, filenamesArray+numModels);

    const size_t numValues = 2;
    const char* const valueNamesArray[numValues] = { "two", "one" };
    std::vector<std::string> valueNames(valueNamesArray, valueNamesArray+numValues);

    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    Query query;
    query.initialize(filenames, valueNames, crs);

    const double tolerance = 2.0e-5;
    { // One Block Flat
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(!err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];
            double valuesE[numValues];
            valuesE[0] = pointsOne.computeValueTwo(x, y, z);
            valuesE[1] = pointsOne.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in one-block-flat.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valuesE[iValue], toleranceV));
            } // for
        } // for
    } // One Block Flat

    { // Three Block Flat
        geomodelgrids::testdata::ThreeBlocksFlatPoints pointsThree;
        const size_t numPoints = pointsThree.getNumPoints();
        const double* pointsLLE = pointsThree.getLatLonElev();
        const double* pointsXYZ = pointsThree.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(!err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];
            double valuesE[numValues];
            valuesE[0] = pointsThree.computeValueTwo(x, y, z);
            valuesE[1] = pointsThree.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue]
                                           << "' in three-blocks-flat.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valuesE[iValue], toleranceV));
            } // for
        } // for
    } // Three Block Flat

    { // Outside domain
        geomodelgrids::testdata::OutsideDomainPoints pointsOutisde;
        const size_t numPoints = pointsOutisde.getNumPoints();
        const double* pointsLLE = pointsOutisde.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(NODATA_VALUE, toleranceV));
            } // for
        } // for
    } // Outside domain

} // testQueryFlat


// ------------------------------------------------------------------------------------------------
// Test query() for model with topography.
void
geomodelgrids::serial::TestQuery::testQueryTopo(void) {
    const size_t numModels = 2;
    const char* const filenamesArray[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };
    std::vector<std::string> filenames(filenamesArray, filenamesArray+numModels);

    const size_t numValues = 2;
    const char* const valueNamesArray[numValues] = { "two", "one" };
    std::vector<std::string> valueNames(valueNamesArray, valueNamesArray+numValues);

    geomodelgrids::testdata::OneBlockTopoPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    Query query;
    query.initialize(filenames, valueNames, crs);

    const double tolerance = 1.0e-5;
    { // One Block Topo
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(!err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];
            double valuesE[numValues];
            valuesE[0] = pointsOne.computeValueTwo(x, y, z);
            valuesE[1] = pointsOne.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in one-block-topo.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valuesE[iValue], toleranceV));
            } // for
        } // for
    } // One Block Topo

    { // Three Block Topo
        geomodelgrids::testdata::ThreeBlocksTopoPoints pointsThree;
        const size_t numPoints = pointsThree.getNumPoints();
        const double* pointsLLE = pointsThree.getLatLonElev();
        const double* pointsXYZ = pointsThree.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(!err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];
            double valuesE[numValues];
            valuesE[0] = pointsThree.computeValueTwo(x, y, z);
            valuesE[1] = pointsThree.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue]
                                           << "' in three-blocks-topo.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valuesE[iValue], toleranceV));
            } // for
        } // for
    } // Three Block Topo

    { // Outside domain
        geomodelgrids::testdata::OutsideDomainPoints pointsOutisde;
        const size_t numPoints = pointsOutisde.getNumPoints();
        const double* pointsLLE = pointsOutisde.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(NODATA_VALUE, toleranceV));
            } // for
        } // for
    } // Outside domain
} // testQueryTopo


// ------------------------------------------------------------------------------------------------
// Test query() for model using top surface for squashing.
void
geomodelgrids::serial::TestQuery::testQuerySquashTop(void) {
    const double squashMinElev = geomodelgrids::testdata::ModelPoints::squashMinElev;

    const size_t numModels = 2;
    const char* const filenamesArray[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };
    std::vector<std::string> filenames(filenamesArray, filenamesArray+numModels);

    const size_t numValues = 2;
    const char* const valueNamesArray[numValues] = { "two", "one" };
    std::vector<std::string> valueNames(valueNamesArray, valueNamesArray+numValues);

    geomodelgrids::testdata::OneBlockSquashPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    Query query;
    query.setSquashMinElev(squashMinElev);
    query.initialize(filenames, valueNames, crs);

    const double tolerance = 1.0e-5;
    { // One Block Squash
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(!err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];

            double valuesE[numValues];
            valuesE[0] = pointsOne.computeValueTwo(x, y, z);
            valuesE[1] = pointsOne.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in one-block-topo.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valuesE[iValue], toleranceV));
            } // for
        } // for
    } // One Block Squash

    { // Three Block Squash
        geomodelgrids::testdata::ThreeBlocksSquashTopPoints pointsThree;
        const size_t numPoints = pointsThree.getNumPoints();
        const double* pointsLLE = pointsThree.getLatLonElev();
        const double* pointsXYZ = pointsThree.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(!err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];

            double valuesE[numValues];
            valuesE[0] = pointsThree.computeValueTwo(x, y, z);
            valuesE[1] = pointsThree.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue]
                                           << "' in three-blocks-topo.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valuesE[iValue], toleranceV));
            } // for
        } // for
    } // Three Block Squash

    { // Outside domain
        geomodelgrids::testdata::OutsideDomainPoints pointsOutisde;
        const size_t numPoints = pointsOutisde.getNumPoints();
        const double* pointsLLE = pointsOutisde.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(NODATA_VALUE, toleranceV));
            } // for
        } // for
    } // Outside domain
} // TestQuerySquashTop


// ------------------------------------------------------------------------------------------------
// Test query() for model using topography/bathymetry for squashing.
void
geomodelgrids::serial::TestQuery::testQuerySquashTopoBathy(void) {
    const double squashMinElev = geomodelgrids::testdata::ModelPoints::squashMinElev;

    const size_t numModels = 2;
    const char* const filenamesArray[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };
    std::vector<std::string> filenames(filenamesArray, filenamesArray+numModels);

    const size_t numValues = 2;
    const char* const valueNamesArray[numValues] = { "two", "one" };
    std::vector<std::string> valueNames(valueNamesArray, valueNamesArray+numValues);

    geomodelgrids::testdata::OneBlockSquashPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    Query query;
    query.setSquashing(Query::SQUASH_TOPOGRAPHY_BATHYMETRY);
    query.setSquashMinElev(squashMinElev);
    query.initialize(filenames, valueNames, crs);

    const double tolerance = 1.0e-5;
    { // One Block Squash
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(!err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];

            double valuesE[numValues];
            valuesE[0] = pointsOne.computeValueTwo(x, y, z);
            valuesE[1] = pointsOne.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in one-block-topo.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valuesE[iValue], toleranceV));
            } // for
        } // for
    } // One Block Squash

    { // Three Block Squash
        geomodelgrids::testdata::ThreeBlocksSquashTopoBathyPoints pointsThree;
        const size_t numPoints = pointsThree.getNumPoints();
        const double* pointsLLE = pointsThree.getLatLonElev();
        const double* pointsXYZ = pointsThree.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(!err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];

            double valuesE[numValues];
            valuesE[0] = pointsThree.computeValueTwo(x, y, z);
            valuesE[1] = pointsThree.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue]
                                           << "' in three-blocks-topo.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valuesE[iValue], toleranceV));
            } // for
        } // for
    } // Three Block Squash

    { // Outside domain
        geomodelgrids::testdata::OutsideDomainPoints pointsOutisde;
        const size_t numPoints = pointsOutisde.getNumPoints();
        const double* pointsLLE = pointsOutisde.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = query.query(values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            REQUIRE(err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(NODATA_VALUE, toleranceV));
            } // for
        } // for
    } // Outside domain
} // TestQuerySquash


// End of file
