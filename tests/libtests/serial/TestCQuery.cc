/**
 * C++ unit testing of C API for geomodelgrids::serial::Query.
 */

#include <portinfo>

#include "ModelPoints.hh"

extern "C" {
#include "geomodelgrids/serial/cquery.h"
}
#include "geomodelgrids/serial/Query.hh" // USES Query
#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include <cppunit/extensions/HelperMacros.h>

#include <cmath>

namespace geomodelgrids {
    namespace serial {
        class TestCQuery;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestCQuery : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE -------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE(TestCQuery);

    CPPUNIT_TEST(testCreateDestroy);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testQueryTopElevation);
    CPPUNIT_TEST(testQueryTopoBathyElevation);
    CPPUNIT_TEST(testQueryFlat);
    CPPUNIT_TEST(testQueryTopo);
    CPPUNIT_TEST(testQuerySquashTop);
    CPPUNIT_TEST(testQuerySquashTopoBathy);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Test constructor.
    void testCreateDestroy(void);

    /// Test getters.
    void testAccessors(void);

    /// Test initialize() and finalize().
    void testInitialize(void);

    /// Test queryTopElevation().
    void testQueryTopElevation(void);

    /// Test queryTopoBathyElevation().
    void testQueryTopoBathyElevation(void);

    /// Test query() for model with flat ground surface.
    void testQueryFlat(void);

    /// Test query() for model with topography.
    void testQueryTopo(void);

    /// Test query() for model with squashing using top surface.
    void testQuerySquashTop(void);

    /// Test query() for model with squashing using topography/bathymetry.
    void testQuerySquashTopoBathy(void);

}; // class TestCQuery
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestCQuery);

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestCQuery::testCreateDestroy(void) {
    void* handle = geomodelgrids_squery_create();
    CPPUNIT_ASSERT_MESSAGE("Create failed.", handle);

    geomodelgrids_squery_destroy(&handle);
    CPPUNIT_ASSERT_MESSAGE("Destroy failed.", !handle);
} // testCreateDestroy


// ------------------------------------------------------------------------------------------------
// Test setters.
void
geomodelgrids::serial::TestCQuery::testAccessors(void) {
    void* handle = geomodelgrids_squery_create();CPPUNIT_ASSERT(handle);
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;CPPUNIT_ASSERT(query);

    void* errorHandler = geomodelgrids_squery_getErrorHandler(handle);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in error handler.", (geomodelgrids::utils::ErrorHandler*)errorHandler,
                                 query->_errorHandler);

    const double minElev(-2.0e+3);
    int err = geomodelgrids_squery_setSquashMinElev(handle, minElev);CPPUNIT_ASSERT(!err);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in squashing flag.", geomodelgrids::serial::Query::SQUASH_TOP_SURFACE, query->_squash);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in minimum squashing elevation.", minElev, query->_squashMinElev,
                                         1.0e-6);

    err = geomodelgrids_squery_setSquashing(handle, GEOMODELGRIDS_SQUASH_TOPOGRAPHY_BATHYMETRY);CPPUNIT_ASSERT(!err);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in squashing flag.", geomodelgrids::serial::Query::SQUASH_TOPOGRAPHY_BATHYMETRY, query->_squash);
    err = geomodelgrids_squery_setSquashing(handle, GEOMODELGRIDS_SQUASH_NONE);CPPUNIT_ASSERT(!err);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in squashing flag.", geomodelgrids::serial::Query::SQUASH_NONE, query->_squash);
    err = geomodelgrids_squery_setSquashing(handle, GEOMODELGRIDS_SQUASH_TOP_SURFACE);CPPUNIT_ASSERT(!err);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in squashing flag.", geomodelgrids::serial::Query::SQUASH_TOP_SURFACE, query->_squash);

    // Bad handles
    err = geomodelgrids_squery_setSquashMinElev(NULL, minElev);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in bad handle error code.", int(geomodelgrids::utils::ErrorHandler::ERROR),
                                 err);

    err = geomodelgrids_squery_setSquashing(NULL, false);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in bad handle error code.", int(geomodelgrids::utils::ErrorHandler::ERROR),
                                 err);

    geomodelgrids_squery_destroy(&handle);CPPUNIT_ASSERT(!handle);
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

    void* handle = geomodelgrids_squery_create();CPPUNIT_ASSERT(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs);
    CPPUNIT_ASSERT(!err);

    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;CPPUNIT_ASSERT(query);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", numModels, query->_models.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in size of model values map.", numModels, query->_valuesIndex.size());
    for (size_t iModel = 0; iModel < numModels; ++iModel) {
        CPPUNIT_ASSERT(query->_models[iModel]);

        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of values.", numValues, query->_valuesIndex[iModel].size());
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in value index.", valueIndexE[iValue], query->_valuesIndex[iModel][iValue]);
        } // for

    } // for

    err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs);CPPUNIT_ASSERT(!err);
    err = geomodelgrids_squery_finalize(handle);CPPUNIT_ASSERT(!err);

    // Bad value
    const char* const valueNamesBad[numValues] = { "two", "blah" };
    err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNamesBad, numValues, crs);CPPUNIT_ASSERT(err);

    // Bad handle
    err = geomodelgrids_squery_initialize(NULL, filenames, numModels, valueNames, numValues, crs);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in bad handle error code.", int(geomodelgrids::utils::ErrorHandler::ERROR),
                                 err);

    err = geomodelgrids_squery_finalize(NULL);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in bad handle error code.", int(geomodelgrids::utils::ErrorHandler::ERROR),
                                 err);

    geomodelgrids_squery_destroy(&handle);CPPUNIT_ASSERT(!handle);
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
    const char* const* valueNames = NULL;

    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();CPPUNIT_ASSERT(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    CPPUNIT_ASSERT(!err);

    const double tolerance = 1.0e-5;
    { // One Block Flat
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const double elevationE = 0.0;
            const double elevation = geomodelgrids_squery_queryTopElevation(handle, pointsLLE[iPt*spaceDim+0],
                                                                            pointsLLE[iPt*spaceDim+1]);

            std::ostringstream msg;
            msg << "Mismatch for point in one-block-flat ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, valueTolerance);
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

            std::ostringstream msg;
            msg << "Mismatch for point in three-blocks-topo ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, valueTolerance);
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

            std::ostringstream msg;
            msg << "Mismatch for point outside domains ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, valueTolerance);
        } // for
    } // Outside domains

    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;CPPUNIT_ASSERT(query);
    geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
    errorHandler.resetStatus();

    // Bad handle
    double elevation = geomodelgrids_squery_queryTopElevation(NULL, 0.0, 0.0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in elevation for bad handle.", geomodelgrids::NODATA_VALUE, elevation);
    errorHandler.resetStatus();

    geomodelgrids_squery_destroy(&handle);CPPUNIT_ASSERT(!handle);
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
    const char* const* valueNames = NULL;

    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();CPPUNIT_ASSERT(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    CPPUNIT_ASSERT(!err);

    const double tolerance = 1.0e-5;
    { // One Block Flat
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const double elevationE = 0.0;
            const double elevation = geomodelgrids_squery_queryTopoBathyElevation(handle, pointsLLE[iPt*spaceDim+0],
                                                                                  pointsLLE[iPt*spaceDim+1]);

            std::ostringstream msg;
            msg << "Mismatch for point in one-block-flat ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, valueTolerance);
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

            std::ostringstream msg;
            msg << "Mismatch for point in three-blocks-topo ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, valueTolerance);
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

            std::ostringstream msg;
            msg << "Mismatch for point outside domains ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, valueTolerance);
        } // for
    } // Outside domains

    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;CPPUNIT_ASSERT(query);
    geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
    errorHandler.resetStatus();

    // Bad handle
    double elevation = geomodelgrids_squery_queryTopoBathyElevation(NULL, 0.0, 0.0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in elevation for bad handle.", geomodelgrids::NODATA_VALUE, elevation);
    errorHandler.resetStatus();

    geomodelgrids_squery_destroy(&handle);CPPUNIT_ASSERT(!handle);
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

    void* handle = geomodelgrids_squery_create();CPPUNIT_ASSERT(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    CPPUNIT_ASSERT(!err);

    const double tolerance = 2.0e-5;
    { // One Block Flat
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            const int err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in one-block-flat", !err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];
            double valuesE[numValues];
            valuesE[0] = pointsOne.computeValueTwo(x, y, z);
            valuesE[1] = pointsOne.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in one-block-flat.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valuesE[iValue], values[iValue],
                                                     toleranceV);
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
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in three-blocks-flat", !err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];
            double valuesE[numValues];
            valuesE[0] = pointsThree.computeValueTwo(x, y, z);
            valuesE[1] = pointsThree.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue]
                    << "' in three-blocks-flat.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valuesE[iValue], values[iValue],
                                                     toleranceV);
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
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in outside-domain", err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), NODATA_VALUE, values[iValue],
                                                     toleranceV);
            } // for
        } // for
    } // Outside domain

    // Bad handle
    err = geomodelgrids_squery_query(NULL, NULL, 0.0, 0.0, 0.0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in bad handle error code.", int(geomodelgrids::utils::ErrorHandler::ERROR),
                                 err);

    geomodelgrids_squery_destroy(&handle);CPPUNIT_ASSERT(!handle);
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

    void* handle = geomodelgrids_squery_create();CPPUNIT_ASSERT(handle);
    int err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    CPPUNIT_ASSERT(!err);

    const double tolerance = 1.0e-5;
    { // One Block Topo
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in one-block-topo.", !err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];
            double valuesE[numValues];
            valuesE[0] = pointsOne.computeValueTwo(x, y, z);
            valuesE[1] = pointsOne.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in one-block-topo.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valuesE[iValue], values[iValue],
                                                     toleranceV);
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
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in three-blocks-topo.", !err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];
            double valuesE[numValues];
            valuesE[0] = pointsThree.computeValueTwo(x, y, z);
            valuesE[1] = pointsThree.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue]
                    << "' in three-blocks-topo.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valuesE[iValue], values[iValue],
                                                     toleranceV);
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
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in outside-domain.", err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), NODATA_VALUE, values[iValue],
                                                     toleranceV);
            } // for
        } // for
    } // Outside domain

    { // NULL values
        err = geomodelgrids_squery_query(handle, NULL, 0, 0, 0);
        CPPUNIT_ASSERT_MESSAGE("Mismatch in err value for NULL values.", err);
    } // NULL values

    geomodelgrids_squery_destroy(&handle);CPPUNIT_ASSERT(!handle);
} // testQueryTopo


// ------------------------------------------------------------------------------------------------
// Test query() for model with squashing using top surface.
void
geomodelgrids::serial::TestCQuery::testQuerySquashTop(void) {
    const double squashMinElev = -4.999e+3;

    const size_t numModels = 2;
    const char* const filenames[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };

    const size_t numValues = 2;
    const char* const valueNames[numValues] = { "two", "one" };

    geomodelgrids::testdata::OneBlockSquashPoints pointsOne(squashMinElev);
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();CPPUNIT_ASSERT(handle);
    int err = geomodelgrids_squery_setSquashMinElev(handle, squashMinElev);CPPUNIT_ASSERT(!err);
    err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    CPPUNIT_ASSERT(!err);

    const double tolerance = 1.0e-5;
    { // One Block Squash
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in one-block-topo.", !err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];

            double valuesE[numValues];
            valuesE[0] = pointsOne.computeValueTwo(x, y, z);
            valuesE[1] = pointsOne.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in one-block-topo.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valuesE[iValue], values[iValue],
                                                     toleranceV);
            } // for
        } // for
    } // One Block Squash

    { // Three Block Squash
        geomodelgrids::testdata::ThreeBlocksSquashTopPoints pointsThree(squashMinElev);
        const size_t numPoints = pointsThree.getNumPoints();
        const double* pointsLLE = pointsThree.getLatLonElev();
        const double* pointsXYZ = pointsThree.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in three-blocks-topo.", !err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];

            double valuesE[numValues];
            valuesE[0] = pointsThree.computeValueTwo(x, y, z);
            valuesE[1] = pointsThree.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue]
                    << "' in three-blocks-topo.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valuesE[iValue], values[iValue],
                                                     toleranceV);
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
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in outside-domain.", err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), NODATA_VALUE, values[iValue],
                                                     toleranceV);
            } // for
        } // for
    } // Outside domain

    geomodelgrids_squery_destroy(&handle);CPPUNIT_ASSERT(!handle);
} // testQuerySquashTop


// ------------------------------------------------------------------------------------------------
// Test query() for model with squashing using topography/bathymetry.
void
geomodelgrids::serial::TestCQuery::testQuerySquashTopoBathy(void) {
    const double squashMinElev = -4.999e+3;

    const size_t numModels = 2;
    const char* const filenames[numModels] = {
        "../../data/one-block-topo.h5",
        "../../data/three-blocks-topo.h5",
    };

    const size_t numValues = 2;
    const char* const valueNames[numValues] = { "two", "one" };

    geomodelgrids::testdata::OneBlockSquashPoints pointsOne(squashMinElev);
    const std::string& crs = pointsOne.getCRSLatLonElev();
    const size_t spaceDim = 3;

    void* handle = geomodelgrids_squery_create();CPPUNIT_ASSERT(handle);
    int err = geomodelgrids_squery_setSquashing(handle, GEOMODELGRIDS_SQUASH_TOPOGRAPHY_BATHYMETRY);CPPUNIT_ASSERT(!err);
    err = geomodelgrids_squery_setSquashMinElev(handle, squashMinElev);CPPUNIT_ASSERT(!err);
    err = geomodelgrids_squery_initialize(handle, filenames, numModels, valueNames, numValues, crs.c_str());
    CPPUNIT_ASSERT(!err);

    const double tolerance = 1.0e-5;
    { // One Block Squash
        const size_t numPoints = pointsOne.getNumPoints();
        const double* pointsLLE = pointsOne.getLatLonElev();
        const double* pointsXYZ = pointsOne.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in one-block-topo.", !err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];

            double valuesE[numValues];
            valuesE[0] = pointsOne.computeValueTwo(x, y, z);
            valuesE[1] = pointsOne.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in one-block-topo.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valuesE[iValue], values[iValue],
                                                     toleranceV);
            } // for
        } // for
    } // One Block Squash

    { // Three Block Squash
        geomodelgrids::testdata::ThreeBlocksSquashTopoBathyPoints pointsThree(squashMinElev);
        const size_t numPoints = pointsThree.getNumPoints();
        const double* pointsLLE = pointsThree.getLatLonElev();
        const double* pointsXYZ = pointsThree.getXYZ();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            double values[numValues];
            err = geomodelgrids_squery_query(handle, values, pointsLLE[iPt*spaceDim+0],
                                             pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in three-blocks-topo.", !err);

            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];
            const double z = pointsXYZ[iPt*spaceDim+2];

            double valuesE[numValues];
            valuesE[0] = pointsThree.computeValueTwo(x, y, z);
            valuesE[1] = pointsThree.computeValueOne(x, y, z);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue]
                    << "' in three-blocks-topo.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(valuesE[iValue]));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valuesE[iValue], values[iValue],
                                                     toleranceV);
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
            CPPUNIT_ASSERT_MESSAGE("Mismatch in err value in outside-domain.", err);

            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                std::ostringstream msg;
                msg << "Mismatch at point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                    << ", " << pointsLLE[iPt*spaceDim+2] << ") for value '" << valueNames[iValue] << "' in outside-domain.";
                const double toleranceV = std::max(tolerance, tolerance*fabs(NODATA_VALUE));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), NODATA_VALUE, values[iValue],
                                                     toleranceV);
            } // for
        } // for
    } // Outside domain

    geomodelgrids_squery_destroy(&handle);CPPUNIT_ASSERT(!handle);
} // testQuerySquashTopoBathy


// End of file
