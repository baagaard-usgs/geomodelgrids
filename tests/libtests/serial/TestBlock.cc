/**
 * C++ unit testing of geomodelgrids::serial::Block.
 */

#include <portinfo>

#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace serial {
        class TestBlock;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestBlock : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestBlock);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testLoadMetadata);
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

    /// Test query().
    void testQuery(void);

}; // class TestBlock
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestBlock);

// ----------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestBlock::testConstructor(void) {
    const std::string blockName("myblock");
    Block block(blockName.c_str());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking name", blockName, block._name);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking horizontal resolution", 0.0, block._resolutionHoriz);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking vertical resolution", 0.0, block._resolutionVert);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking zero z top", 0.0, block._zTop);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking values buffer", (double*)NULL, block._values);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking numValues", size_t(0), block._numValues);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[0]", size_t(0), block._dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[1]", size_t(0), block._dims[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[2]", size_t(0), block._dims[2]);
} // testConstructor


// ----------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestBlock::testAccessors(void) {
    const std::string blockName("myblock");
    Block block(blockName.c_str());

    const double resolutionHoriz(8.0);block._resolutionHoriz = resolutionHoriz;
    const double resolutionVert(2.0);block._resolutionVert = resolutionVert;
    const double zTop(-3.0);block._zTop = zTop;
    const size_t dims[3] = { 5, 6, 7 };
    block._dims[0] = dims[0];
    block._dims[1] = dims[1];
    block._dims[2] = dims[2];
    const size_t numValues(5);block._numValues = numValues;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking name", blockName, block.getName());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking horizontal resolution", resolutionHoriz, block.getResolutionHoriz());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking vertical resolution", resolutionVert, block.getResolutionVert());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking z top", zTop, block.getZTop());
    const size_t* dimsT = block.getDims();
    CPPUNIT_ASSERT_MESSAGE("Checking dims pointer", dimsT);
    for (size_t i = 0; i < 3; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims", dims[i], dimsT[i]);
    } // for

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking numValues", numValues, block.getNumValues());
} // testAccessors


// ----------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestBlock::testLoadMetadata(void) {
    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDONLY);

    const std::string blockName("block");
    Block block(blockName.c_str());
    block.loadMetadata(&h5);

    const double resolutionHoriz(10.0e+3);
    const double resolutionVert(5.0e+3);
    const double zTop(0.0);
    const size_t dims[3] = { 4, 5, 2 };
    const size_t numValues(2);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking name", blockName, block.getName());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking horizontal resolution", resolutionHoriz, block.getResolutionHoriz());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking vertical resolution", resolutionVert, block.getResolutionVert());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking z top", zTop, block.getZTop());
    const size_t* dimsT = block.getDims();
    for (size_t i = 0; i < 3; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims", dims[i], dimsT[i]);
    } // for

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking numValues", numValues, block.getNumValues());
} // testLoadMetadata


// ----------------------------------------------------------------------
// Test query().
void
geomodelgrids::serial::TestBlock::testQuery(void) {
    const size_t numPoints = 5;
    const size_t spaceDim = 3;
    double xyz[numPoints*spaceDim] = {
        18.1e+3, 8.3e+3, -10.0,
        5.2e+3, 7.1e+3, -5.0e+3,
        0.2e+3, 23.2e+3, -3.0e+3,
        28.0e+3, 12.9e+3, -1.6e+3,
        9.3e+3, 0.5e+3, -2.8e+3,
    };

    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDONLY);

    const std::string blockName("block");
    Block block(blockName.c_str());
    block.loadMetadata(&h5);
    block.openQuery(&h5);

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double* values = block.query(xyz[iPt*spaceDim+0], xyz[iPt*spaceDim+1], xyz[iPt*spaceDim+2]);

        const double x = xyz[iPt*spaceDim+0];
        const double y = xyz[iPt*spaceDim+1];
        const double z = xyz[iPt*spaceDim+2];

        { // Value 0
            const double valueE = 2.0e+3 + 1.0 * x + 0.4 * y - 0.5 * z;

            std::ostringstream msg;
            msg << "Mismatch for point (" << xyz[iPt*spaceDim+0] << ", " << xyz[iPt*spaceDim+1]
                << ", " << xyz[iPt*spaceDim+2] << ") for value 0.";
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*valueE);
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[0], valueTolerance);
        } // Value 0

        { // Value 1
            const double valueE = -1.2e+3 + 2.1 * x - 0.9 * y + 0.3 * z;

            std::ostringstream msg;
            msg << "Mismatch for point (" << xyz[iPt*spaceDim+0] << ", " << xyz[iPt*spaceDim+1]
                << ", " << xyz[iPt*spaceDim+2] << ") for value 1.";
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*valueE);
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[1], valueTolerance);
        } // Value 1
    } // for
} // testQuery


// End of file
