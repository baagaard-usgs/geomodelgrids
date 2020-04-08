/**
 * C++ unit testing of geomodelgrids::serial::Query.
 */

#include <portinfo>

#include "geomodelgrids/serial/Query.hh" // USES Query

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace serial {
        class TestQuery;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestQuery : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestQuery);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testQueryElevation);
    CPPUNIT_TEST(testQueryFlat);
    CPPUNIT_TEST(testQueryTopo);
    CPPUNIT_TEST(testQuerySquash);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test getters.
    void testAccessors(void);

    /// Test initialize() and finalize().
    void testInitialize(void);

    /// Test queryElevation().
    void testQueryElevation(void);

    /// Test query() for model with flat ground surface.
    void testQueryFlat(void);

    /// Test query() for model with topography.
    void testQueryTopo(void);

    /// Test query() for model with topography using squashing.
    void testQuerySquash(void);

}; // class TestQuery
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestQuery);

// ----------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestQuery::testConstructor(void) {
    Query query;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in default minimum squashing elevation.", 0.0, query._squashMinElev,
                                         1.0e-6);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default squahing flag.", false, query._squash);
} // testConstructor


// ----------------------------------------------------------------------
// Test setters.
void
geomodelgrids::serial::TestQuery::testAccessors(void) {
    Query query;

    const double minElev(-2.0e+3);
    query.setSquashMinElev(minElev);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in squashing flag.", true, query._squash);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in minimum squashing elevation.", minElev, query._squashMinElev,
                                         1.0e-6);
} // testAccessors


// ---------------------------------------------------------------------------------------------------------------------
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

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", numModels, query._models.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in size of model values map.", numModels, query._valuesIndex.size());
    for (size_t iModel = 0; iModel < numModels; ++iModel) {
        CPPUNIT_ASSERT(query._models[iModel]);

        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of values.", numValues, query._valuesIndex[iModel].size());
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in value index.", valueIndexE[iValue], query._valuesIndex[iModel][iValue]);
        } // for

    } // for

    query.finalize();
} // testInitialize


// ---------------------------------------------------------------------------------------------------------------------
// Test queryElevation().
void
geomodelgrids::serial::TestQuery::testQueryElevation(void) {
    const size_t numModels = 2;
    const char* const filenamesArray[numModels] = {
        "../../data/one-block-flat.h5",
        "../../data/three-blocks-topo.h5",
    };
    std::vector<std::string> filenames(filenamesArray, filenamesArray+numModels);

    const std::string crs("EPSG:4326"); // WGS84
    const size_t numPoints = 7;
    const size_t spaceDim = 2;
    const double latlon[numPoints*spaceDim] = {
        34.7, -117.8, // three blocks
        34.5, -117.8,
        34.6, -117.5,
        35.0, -117.5,
        34.7, -118.0,
        50.0, -100.0, // outside models
        37.4, -122.3, // one flat
    };
    double elevationE[numPoints] = {
        26883.65457072,
        4162.76549694,
        17518.58422866,
        137391.81153092,
        754.11098391,
        0.0,
        0.0,
    };

    Query query;
    std::vector<std::string> valueNames;
    query.initialize(filenames, valueNames, crs);

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double elevation = query.queryElevation(latlon[iPt*spaceDim+0], latlon[iPt*spaceDim+1]);

        std::ostringstream msg;
        msg << "Mismatch for point (" << latlon[iPt*spaceDim+0] << ", " << latlon[iPt*spaceDim+1] << ").";
        const double tolerance = 1.0e-6;
        const double valueTolerance = std::max(tolerance, tolerance*elevationE[iPt]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE[iPt], elevation, valueTolerance);
    } // for
} // testQueryElevation


// ---------------------------------------------------------------------------------------------------------------------
// Test query() for model with flat ground surface.
void
geomodelgrids::serial::TestQuery::testQueryFlat(void) {
    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement test.", false);
} // testQueryFlat


// ---------------------------------------------------------------------------------------------------------------------
// Test query() for model with topography.
void
geomodelgrids::serial::TestQuery::testQueryTopo(void) {
#if 1

    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement test.", false);
#else
    Query model;
    model.open("../../data/three-blocks-topo.h5", Query::READ);
    model.loadMetadata();
    model.initialize();

    const std::string inputCRS("EPSG:4326"); // WGS84
    const size_t numPoints = 5;
    const size_t spaceDim = 3;
    const double lle[numPoints*spaceDim] = {
        34.7, -117.8, 26867.680454533336,
        35.0, -117.6, -45.0e+3,
        35.1, -117.8, 122620.64106666666,
        35.0, -117.5, 72541.38995555555,
        35.0, -118.2, -27551.479865333335,
    };
    double xyzE[numPoints*spaceDim] = {
        18157.12318227833, 28596.959586967772, -10.0,
        50151.20052049957, 49082.89449952264, -45.0e+3,
        39462.97248734834, 67560.54921972206, -3.0e+3,
        58165.78933216298, 44727.815689240764, -16.0e+3,
        2160.5375531014906, 75390.66860725963, -28.0e+3,
    };

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double* values = model.query(lle[iPt*spaceDim+0], lle[iPt*spaceDim+1], lle[iPt*spaceDim+2]);

        const double x = xyzE[iPt*spaceDim+0];
        const double y = xyzE[iPt*spaceDim+1];
        const double z = xyzE[iPt*spaceDim+2];

        { // Value 0
            const double valueE = 2.0e+3 + 1.0 * x + 0.4 * y - 0.5 * z;

            std::ostringstream msg;
            msg << "Mismatch for point (" << lle[iPt*spaceDim+0] << ", " << lle[iPt*spaceDim+1]
                << ", " << lle[iPt*spaceDim+2] << ") for value 0.";
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*valueE);
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[0], valueTolerance);
        } // Value 0

        { // Value 1
            const double valueE = -1.2e+3 + 2.1 * x - 0.9 * y + 0.3 * z;

            std::ostringstream msg;
            msg << "Mismatch for point (" << lle[iPt*spaceDim+0] << ", " << lle[iPt*spaceDim+1]
                << ", " << lle[iPt*spaceDim+2] << ") for value 1.";
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*valueE);
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[1], valueTolerance);
        } // Value 1
    } // for
#endif

} // testQueryTopo


// ---------------------------------------------------------------------------------------------------------------------
// Test query() for model with topography using squashing.
void
geomodelgrids::serial::TestQuery::testQuerySquash(void) {
    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement test.", false);
} // TestQuerySquash


// End of file
