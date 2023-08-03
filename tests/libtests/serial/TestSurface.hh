/**
 * C++ unit testing of geomodelgrids::serial::Surface.
 */

#include <portinfo>

#include <cstddef> // HASA size_t

namespace geomodelgrids {
    namespace serial {
        class TestSurface;
        class TestSurface_Data;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestSurface {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Constructor.
    TestSurface(TestSurface_Data* data);

    /// Destructor.
    ~TestSurface(void);

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

    // PUBLIC MEMBERS -----------------------------------------------------------------------------
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
