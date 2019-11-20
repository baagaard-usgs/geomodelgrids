/**
 * C++ unit testing of geomodelgrids::serial::Model.
 */

#include <portinfo>

#include "geomodelgrids/serial/Model.hh" // USES Model
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Topography.hh" // USES Topography
#include "geomodelgrids/serial/Block.hh" // USES Block

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
    Model model;

    const size_t numValues = 3;
    const char* namesPtr[numValues] = {"one", "two", "three"};
    const char* unitsPtr[numValues] = {"m", "m/s", "MPa"};
    const std::vector<std::string> valueNames(namesPtr, namesPtr+numValues);model._valueNames = valueNames;
    const std::vector<std::string> valueUnits(unitsPtr, unitsPtr+numValues);model._valueNames = valueNames;
    const std::string projectionString("Projection String");model._projectionString = projectionString;
    const double origin[2] = { 10.0, 20.0 };
    model._origin[0] = origin[0];
    model._origin[1] = origin[1];
    const double yazimuth(34.0);model._yazimuth = yazimuth;
    const double dims[3] = { 100.0, 200.0, 50.0 };
    model._dims[0] = dims[0];
    model._dims[1] = dims[1];
    model._dims[2] = dims[2];

    ModelInfo info;model._info = &info;
    Topography topography;model._topography = &topography;

    Block one("one"), three("three"), five("five");
    const size_t numBlocks(3);
    Block* blocksPtr[numBlocks] = {&one, &three, &five};
    const std::vector<Block*> blocks(blocksPtr, blocksPtr+numBlocks);model._blocks = blocks;

    const double tolerance = 1.0e-6;

    const std::vector<std::string>& valueNamesT = model.getValueNames();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking names of values size", valueNames.size(), valueNamesT.size());
    for (size_t i = 0; i < valueNames.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking names of values", valueNames[i], valueNamesT[i]);
    } // for

    const std::vector<std::string>& valueUnitsT = model.getValueUnits();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking names of values size", valueUnits.size(), valueUnitsT.size());
    for (size_t i = 0; i < valueUnits.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking names of values", valueUnits[i], valueUnitsT[i]);
    } // for

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking projection string", projectionString, model.getProjectionString());

    const double* originT = model.getOrigin();
    CPPUNIT_ASSERT_MESSAGE("Checking origin pointer", originT);
    for (size_t i = 0; i < 2; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking origin", origin[i], originT[i]);
    } // for

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Checking yazimuth", yazimuth, model.getYAzimuth(), tolerance);

    const double* dimsT = model.getDims();
    CPPUNIT_ASSERT_MESSAGE("Checking dims pointer", dimsT);
    for (size_t i = 0; i < 2; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Checking dims", dims[i], dimsT[i], tolerance);
    } // for

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking info", const_cast<const ModelInfo*>(&info), model.getInfo());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking topography", const_cast<const Topography*>(&topography), model.getTopography());

    const std::vector<Block*>& blocksT = model.getBlocks();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking blocks size", blocks.size(), blocksT.size());
    for (size_t i = 0; i < blocks.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking blocks", blocks[i], blocksT[i]);
    } // for
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
