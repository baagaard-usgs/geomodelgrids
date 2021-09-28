/**
 * C++ unit testing of geomodelgrids::serial::Block.
 */

#include <portinfo>

#include "ModelPoints.hh" // USES ModelPoints

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace serial {
        class TestBlock;
        class TestBlock_Data;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestBlock : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE -------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE(TestBlock);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testSetHyperslabDims);
    CPPUNIT_TEST(testLoadMetadata);
    CPPUNIT_TEST(testLoadBadMetadata);
    CPPUNIT_TEST(testQuery);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Setup.
    void setUp(void);

    /// Tear down.
    void tearDown(void);

    /// Test constructor.
    void testConstructor(void);

    /// Test getters.
    void testAccessors(void);

    /// Test setHyperslabDims.
    void testSetHyperslabDims(void);

    /// Test loadMetadata().
    void testLoadMetadata(void);

    /// Test loadMetadata() with bad variable resolution data.
    void testLoadBadMetadata(void);

    /// Test query().
    void testQuery(void);

    // PROTECTED MEMBERS --------------------------------------------------------------------------
protected:

    TestBlock_Data* _data; ///< Test data.

}; // class TestBlock

// ================================================================================================
class geomodelgrids::serial::TestBlock_Data {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Constructor
    TestBlock_Data(void);

    /// Destructor
    ~TestBlock_Data(void);

    // PUBLIC MEMBERS //////////////////////////////////////////////////////////////////////////////////////////////////
public:

    const char* filename; ///< Name of file with ASCII mesh.
    double resolutionX; ///< Block resolution along x axis.
    double resolutionY; ///< Block resolution along y axis.
    double resolutionZ; ///< Block resolution along z axis.
    double zTop; ///< Top of block z coordinate.
    double* coordinatesX; ///< Coordinages along x axis (only if variable resolution).
    double* coordinatesY; ///< Coordinages along y axis (only if variable resolution).
    double* coordinatesZ; ///< Coordinages along z axis (only if variable resolution).
    size_t numX; ///< Number of coordinates along x axis.
    size_t numY; ///< Number of coordinates along y axis.
    size_t numZ; ///< Number of coordinates along z axis.

    geomodelgrids::testdata::ModelPoints* points;

    static const size_t numValues; ///< Number of values in model.
}; // class TestBlock_Data

// End of file
