/**
 * C++ unit testing of geomodelgrids::serial::Block.
 */

#include <portinfo>

#include "tests/data/ModelPoints.hh" // USES ModelPoints

#include <cstddef> // HASA size_t

namespace geomodelgrids {
    namespace serial {
        class TestBlock;
        class TestBlock_Data;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestBlock {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Constructor.
    TestBlock(TestBlock_Data* data);

    /// Destructor.
    ~TestBlock(void);

    /// Test constructor.
    static
    void testConstructor(void);

    /// Test setHyperslabDims.
    static
    void testSetHyperslabDims(void);

    /// Test loadMetadata() with bad variable resolution data.
    static
    void testLoadBadMetadata(void);

    /// Test getters.
    void testAccessors(void);

    /// Test loadMetadata().
    void testLoadMetadata(void);

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

    // PUBLIC MEMBERS -----------------------------------------------------------------------------
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
