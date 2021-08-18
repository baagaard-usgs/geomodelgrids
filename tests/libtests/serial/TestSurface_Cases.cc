/**
 * C++ unit testing of geomodelgrids::serial::Surface.
 */

#include <portinfo>

#include "TestSurface.hh"

namespace geomodelgrids {
    namespace serial {
        class TestSurface_UniformResolution;
        class TestSurface_VariableResolution;
    } // serial
} // geomodelgrids

// ================================================================================================
class geomodelgrids::serial::TestSurface_UniformResolution : public geomodelgrids::serial::TestSurface {
    CPPUNIT_TEST_SUB_SUITE(TestSurface_UniformResolution, TestSurface);
    CPPUNIT_TEST_SUITE_END();

protected:

    void setUp(void) {
        TestSurface::setUp();

        CPPUNIT_ASSERT(_data);

        _data->filename = "../../data/one-block-topo.h5";
        _data->resolutionX = 10.0e+3;
        _data->resolutionY = 10.0e+3;
        _data->numX = 4;
        _data->numY = 5;
    } // setUp

}; // TestSurface_UniformResolution
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestSurface_UniformResolution);

// ================================================================================================
class geomodelgrids::serial::TestSurface_VariableResolution : public geomodelgrids::serial::TestSurface {
    CPPUNIT_TEST_SUB_SUITE(TestSurface_VariableResolution, TestSurface);
    CPPUNIT_TEST_SUITE_END();

protected:

    void setUp(void) {
        TestSurface::setUp();

        CPPUNIT_ASSERT(_data);

        _data->filename = "../../data/one-block-topo-varxy.h5";

        const size_t numX = 4;
        static const double x[numX] = {0.0e+3, 10.0e+3, 20.0e+3, 30.0e+3};

        const size_t numY = 4;
        static const double y[numY] = {0.0e+3, 8.0e+3, 24.0e+3, 40.0e+3};

        _data->numX = numX;
        _data->numY = numY;

        _data->coordinatesX = const_cast<double*>(x);
        _data->coordinatesY = const_cast<double*>(y);
    } // setUp

}; // TestSurface_VariableResolution
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestSurface_VariableResolution);

// End of file
