/**
 * C++ unit testing of geomodelgrids::serial::Surface.
 */

#include <portinfo>

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace serial {
        class TestSurface;
        class TestSurface_Data;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestSurface : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE -------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE(TestSurface);

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

    TestSurface_Data* _data; ///< Test data.

}; // class TestSurface

// ================================================================================================
class geomodelgrids::serial::TestSurface_Data {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Constructor
    TestSurface_Data(void);

    /// Destructor
    ~TestSurface_Data(void);

    // PUBLIC MEMBERS //////////////////////////////////////////////////////////////////////////////////////////////////
public:

    const char* filename; ///< Name of file with ASCII mesh.
    double resolutionX; ///< Surface resolution along x axis.
    double resolutionY; ///< Surface resolution along y axis.
    double* coordinatesX; ///< Coordinages along x axis (only if variable resolution).
    double* coordinatesY; ///< Coordinages along y axis (only if variable resolution).
    size_t numX; ///< Number of coordinates along x axis.
    size_t numY; ///< Number of coordinates along y axis.
}; // class TestSurface_Data

// End of file
