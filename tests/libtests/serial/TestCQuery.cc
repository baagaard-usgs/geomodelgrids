/**
 * C++ unit testing of C API for geomodelgrids::serial::Query.
 */

#include <portinfo>

#include "tests/data/ModelPoints.hh"

extern "C" {
#include "geomodelgrids/serial/cquery.h"
}
#include "geomodelgrids/serial/Query.hh" // USES Query
#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath>

namespace geomodelgrids {
    namespace serial {
        class TestCQuery;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestCQuery {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Test constructor.
    static
    void testCreateDestroy(void);

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

    /// Test query() for model with squashing using top surface.
    static
    void testQuerySquashTop(void);

    /// Test query() for model with squashing using topography/bathymetry.
    static
    void testQuerySquashTopoBathy(void);

}; // class TestCQuery

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestCQuery::testCreateDestroy", "[TestCQuery]") {
    geomodelgrids::serial::TestCQuery::testCreateDestroy();
}
TEST_CASE("TestCQuery::testAccessors", "[TestCQuery]") {
    geomodelgrids::serial::TestCQuery().testAccessors();
}
TEST_CASE("TestCQuery::testInitialize", "[TestCQuery]") {
    geomodelgrids::serial::TestCQuery().testInitialize();
}
TEST_CASE("TestCQuery::testQueryTopElevation", "[TestCQuery]") {
    geomodelgrids::serial::TestCQuery().testQueryTopElevation();
}
TEST_CASE("TestCQuery::testQueryTopoBathyElevation", "[TestCQuery]") {
    geomodelgrids::serial::TestCQuery().testQueryTopoBathyElevation();
}
TEST_CASE("TestCQuery::testQueryFlat", "[TestCQuery]") {
    geomodelgrids::serial::TestCQuery().testQueryFlat();
}
TEST_CASE("TestCQuery::testQueryTopo", "[TestCQuery]") {
    geomodelgrids::serial::TestCQuery().testQueryTopo();
}
TEST_CASE("TestCQuery::testQuerySquashTop", "[TestCQuery]") {
    geomodelgrids::serial::TestCQuery().testQuerySquashTop();
}
TEST_CASE("TestCQuery::testQuerySquashTopoBathy", "[TestCQuery]") {
    geomodelgrids::serial::TestCQuery().testQuerySquashTopoBathy();
}

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestCQuery::testCreateDestroy(void) {
    void* handle = geomodelgrids_squery_create();
    CHECK(handle);

    geomodelgrids_squery_destroy(&handle);
    CHECK(!handle);
} // testCreateDestroy


// ------------------------------------------------------------------------------------------------
// Test setters.
void
geomodelgrids::serial::TestCQuery::testAccessors(void) {
    void* handle = geomodelgrids_squery_create();REQUIRE(handle);
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;REQUIRE(query);

    void* errorHandler = geomodelgrids_squery_getErrorHandler(handle);
    CHECK((geomodelgrids::utils::ErrorHandler*)errorHandler == query->_errorHandler);

    const double minElev(-2.0e+3);
    int err = geomodelgrids_squery_setSquashMinElev(handle, minElev);REQUIRE(!err);

    CHECK(geomodelgrids::serial::Query::SQUASH_TOP_SURFACE == query->_squash);
    CHECK(minElev == query->_squashMinElev);

    err = geomodelgrids_squery_setSquashing(handle, GEOMODELGRIDS_SQUASH_TOPOGRAPHY_BATHYMETRY);REQUIRE(!err);
    CHECK(geomodelgrids::serial::Query::SQUASH_TOPOGRAPHY_BATHYMETRY == query->_squash);
    err = geomodelgrids_squery_setSquashing(handle, GEOMODELGRIDS_SQUASH_NONE);REQUIRE(!err);
    CHECK(geomodelgrids::serial::Query::SQUASH_NONE == query->_squash);
    err = geomodelgrids_squery_setSquashing(handle, GEOMODELGRIDS_SQUASH_TOP_SURFACE);REQUIRE(!err);
    CHECK(geomodelgrids::serial::Query::SQUASH_TOP_SURFACE == query->_squash);

    // Bad handles
    err = geomodelgrids_squery_setSquashMinElev(nullptr, minElev);
    CHECK(int(geomodelgrids::utils::ErrorHandler::ERROR) == err);

    err = geomodelgrids_squery_setSquashing(nullptr, false);
    CHECK(int(geomodelgrids::utils::ErrorHandler::ERROR) == err);

    geomodelgrids_squery_destroy(&handle);REQUIRE(!handle);
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test initialize().
void
geomodelgrids::serial::TestCQuery::testInitialize(void) {
    const size_t numModels = 2;
    const char* const filenames[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };

    const size_t numValues = 2;
    const char* const valueNames[numValues] = { "two", "one" };
    const size_t valueIndexE[numValues] = { 1, 0 };

    const char* const crs = "EPSG:4326";

    void* handle = geomodelgrids_squery_create();REQUIRE(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs);
    REQUIRE(!err);

    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;REQUIRE(query);
    REQUIRE(numModels == query->_models.size());
    REQUIRE(numModels == query->_valuesIndex.size());
    for (size_t iModel = 0; iModel < numModels; ++iModel) {
        REQUIRE(query->_models[iModel]);

        REQUIRE(numValues == query->_valuesIndex[iModel].size());
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            CHECK(valueIndexE[iValue] == query->_valuesIndex[iModel][iValue]);
        } // for

    } // for

    err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs);REQUIRE(!err);
    err = geomodelgrids_squery_finalize(handle);REQUIRE(!err);

    // Bad value
    const char* const valueNamesBad[numValues] = { "two", "blah" };
    err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNamesBad, numValues, crs);REQUIRE(err);

    // Bad handle
    err = geomodelgrids_squery_initialize(nullptr, filenames, numModels, valueNames, numValues, crs);
    CHECK(int(geomodelgrids::utils::ErrorHandler::ERROR) == err);

    err = geomodelgrids_squery_finalize(nullptr);
    CHECK(int(geomodelgrids::utils::ErrorHandler::ERROR) == err);

    geomodelgrids_squery_destroy(&handle);REQUIRE(!handle);
} // testInitialize


// ------------------------------------------------------------------------------------------------
// Test queryTopElevation().
void
geomodelgrids::serial::TestCQuery::testQueryTopElevation(void) {
    const size_t numModels = 2;
    const char* const filenames[numModels] = {
        "../../data/one-block-flat.h5",
        "../../data/three-blocks-topo.h5",
    };

    const size_t numValues = 0;
    const char* const* valueNames = nullptr;

    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();REQUIRE(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    REQUIRE(!err);

    const double tolerance = 1.0e-5;
    { // One Block Flat
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const double elevationE = 0.0;
            const double elevation = geomodelgrids_squery_queryTopElevation(handle, pointsLLE[iPt*spaceDim+0],
                                                                            pointsLLE[iPt*spaceDim+1]);

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
            const double elevation = geomodelgrids_squery_queryTopElevation(handle, pointsLLE[iPt*spaceDim+0],
                                                                            pointsLLE[iPt*spaceDim+1]);

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
            const double elevation = geomodelgrids_squery_queryTopElevation(handle, pointsLLE[iPt*spaceDim+0],
                                                                            pointsLLE[iPt*spaceDim+1]);

            INFO("Mismatch for point outside domains ("
                 << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, valueTolerance));
        } // for
    } // Outside domains

    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;REQUIRE(query);
    geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
    errorHandler.resetStatus();

    // Bad handle
    double elevation = geomodelgrids_squery_queryTopElevation(nullptr, 0.0, 0.0);
    CHECK(geomodelgrids::NODATA_VALUE == elevation);
    errorHandler.resetStatus();

    geomodelgrids_squery_destroy(&handle);REQUIRE(!handle);
} // testQueryTopElevation


// ------------------------------------------------------------------------------------------------
// Test queryTopoBathyElevation().
void
geomodelgrids::serial::TestCQuery::testQueryTopoBathyElevation(void) {
    const size_t numModels = 2;
    const char* const filenames[numModels] = {
        "../../data/one-block-flat.h5",
        "../../data/three-blocks-topo.h5",
    };

    const size_t numValues = 0;
    const char* const* valueNames = nullptr;

    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();REQUIRE(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    REQUIRE(!err);

    const double tolerance = 1.0e-5;
    { // One Block Flat
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const double elevationE = 0.0;
            const double elevation = geomodelgrids_squery_queryTopoBathyElevation(handle, pointsLLE[iPt*spaceDim+0],
                                                                                  pointsLLE[iPt*spaceDim+1]);

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
            const double elevationE = pointsThree.computeTopoBathyElevation(pointsXYZ[iPt*spaceDim+0], pointsXYZ[iPt*spaceDim+1]);
            const double elevation = geomodelgrids_squery_queryTopoBathyElevation(handle, pointsLLE[iPt*spaceDim+0],
                                                                                  pointsLLE[iPt*spaceDim+1]);

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
            const double elevation = geomodelgrids_squery_queryTopoBathyElevation(handle, pointsLLE[iPt*spaceDim+0],
                                                                                  pointsLLE[iPt*spaceDim+1]);

            INFO("Mismatch for point outside domains ("
                 << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, valueTolerance));
        } // for
    } // Outside domains

    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;REQUIRE(query);
    geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
    errorHandler.resetStatus();

    // Bad handle
    double elevation = geomodelgrids_squery_queryTopoBathyElevation(nullptr, 0.0, 0.0);
    CHECK(geomodelgrids::NODATA_VALUE == elevation);
    errorHandler.resetStatus();

    geomodelgrids_squery_destroy(&handle);REQUIRE(!handle);
} // testQueryTopElevation


// ------------------------------------------------------------------------------------------------
// Test query() for model with flat ground surface.
void
geomodelgrids::serial::TestCQuery::testQueryFlat(void) {
    const size_t numModels = 2;
    const char* const filenames[numModels] = {
        "../../data/one-block-flat.h5",
        "../../data/three-blocks-flat.h5",
    };

    const size_t numValues = 2;
    const char* const valueNames[numValues] = { "two", "one" };

    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();REQUIRE(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    REQUIRE(!err);

    const double tolerance = 2.0e-5;
    { // One Block Flat
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(!err);

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
            const int err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                                       pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(!err);

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
            const int err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                                       pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(NODATA_VALUE, toleranceV));
            } // for
        } // for
    } // Outside domain

    // Bad handle
    err = geomodelgrids_squery_query(nullptr, nullptr, 0.0, 0.0, 0.0);
    CHECK(int(geomodelgrids::utils::ErrorHandler::ERROR) == err);

    geomodelgrids_squery_destroy(&handle);REQUIRE(!handle);
} // testQueryFlat


// ------------------------------------------------------------------------------------------------
// Test query() for model with topography.
void
geomodelgrids::serial::TestCQuery::testQueryTopo(void) {
    const size_t numModels = 2;
    const char* const filenames[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };

    const size_t numValues = 2;
    const char* const valueNames[numValues] = { "two", "one" };

    geomodelgrids::testdata::OneBlockTopoPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();REQUIRE(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    REQUIRE(!err);

    const double tolerance = 1.0e-5;
    { // One Block Topo
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(!err);

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
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(!err);

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
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(NODATA_VALUE, toleranceV));
            } // for
        } // for
    } // Outside domain

    { // nullptr values
        err = geomodelgrids_squery_query(handle, nullptr, 0, 0, 0);
        CHECK(err);
    } // nullptr values

    geomodelgrids_squery_destroy(&handle);REQUIRE(!handle);
} // testQueryTopo


// ------------------------------------------------------------------------------------------------
// Test query() for model with squashing using top surface.
void
geomodelgrids::serial::TestCQuery::testQuerySquashTop(void) {
    const double squashMinElev = geomodelgrids::testdata::ModelPoints::squashMinElev;

    const size_t numModels = 2;
    const char* const filenames[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };

    const size_t numValues = 2;
    const char* const valueNames[numValues] = { "two", "one" };

    geomodelgrids::testdata::OneBlockSquashPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();REQUIRE(handle);
    int err = geomodelgrids_squery_setSquashMinElev(handle, squashMinElev);REQUIRE(!err);
    err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    REQUIRE(!err);

    const double tolerance = 1.0e-5;
    { // One Block Squash
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(!err);

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
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(!err);

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
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(NODATA_VALUE, toleranceV));
            } // for
        } // for
    } // Outside domain

    geomodelgrids_squery_destroy(&handle);REQUIRE(!handle);
} // testQuerySquashTop


// ------------------------------------------------------------------------------------------------
// Test query() for model with squashing using topography/bathymetry.
void
geomodelgrids::serial::TestCQuery::testQuerySquashTopoBathy(void) {
    const double squashMinElev = geomodelgrids::testdata::ModelPoints::squashMinElev;

    const size_t numModels = 2;
    const char* const filenames[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };

    const size_t numValues = 2;
    const char* const valueNames[numValues] = { "two", "one" };

    geomodelgrids::testdata::OneBlockSquashPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();REQUIRE(handle);
    int err = geomodelgrids_squery_setSquashing(handle, GEOMODELGRIDS_SQUASH_TOPOGRAPHY_BATHYMETRY);REQUIRE(!err);
    err = geomodelgrids_squery_setSquashMinElev(handle, squashMinElev);REQUIRE(!err);
    err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    REQUIRE(!err);

    const double tolerance = 1.0e-5;
    { // One Block Squash
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(!err);

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
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(!err);

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
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CHECK(err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                INFO("Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                           << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.");
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(NODATA_VALUE, toleranceV));
            } // for
        } // for
    } // Outside domain

    geomodelgrids_squery_destroy(&handle);REQUIRE(!handle);
} // testQuerySquashTopoBathy


// End of file
