/**
 * C++ unit testing of geomodelgrids::serial::Model.
 */

#include <portinfo>

#include "geomodelgrids/serial/Model.hh" // USES Model

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace serial {
        class TestModel;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestModel : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestModel);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testLoadMetadata);
    CPPUNIT_TEST(testContains);
    CPPUNIT_TEST(testQueryElevation);
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

    /// Test contains().
    void testContains(void);

    /// Test queryElevation().
    void testQueryElevation(void);

    /// Test query().
    void testQuery(void);

}; // class TestModel
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestModel);

// ----------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestModel::testConstructor(void) {
    Model model;

    CPPUNIT_ASSERT_MESSAGE("Checking value names", model._valueNames.empty());
    CPPUNIT_ASSERT_MESSAGE("Checking value units", model._valueUnits.empty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking projection string", std::string(""), model._projectionString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking origin x", 0.0, model._origin[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking origin y", 0.0, model._origin[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y azimuth", 0.0, model._yazimuth);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[0]", 0.0, model._dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[1]", 0.0, model._dims[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[2]", 0.0, model._dims[2]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking h5", (HDF5*)NULL, model._h5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking info", (ModelInfo*)NULL, model._info);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking topography", (Topography*)NULL, model._topography);
    // CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking projection", (HDF5*)NULL, model._h5);
    CPPUNIT_ASSERT_MESSAGE("Checking blocks", model._blocks.empty());
} // testConstructor


// ----------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestModel::testAccessors(void) {
    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement testAccessors().", false);
} // testAccessors


// ----------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestModel::testLoadMetadata(void) {
    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement testLoadMetadata().", false);
} // testLoadMetadata


// ----------------------------------------------------------------------
// Test contains().
void
geomodelgrids::serial::TestModel::testContains(void) {
    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement testContains().", false);
} // testContains


// ----------------------------------------------------------------------
// Test queryElevation().
void
geomodelgrids::serial::TestModel::testQueryElevation(void) {
    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement testQueryElevation().", false);
} // testQueryElevation


// ----------------------------------------------------------------------
// Test query().
void
geomodelgrids::serial::TestModel::testQuery(void) {
    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement testQuery().", false);
} // testQuery


// End of file
