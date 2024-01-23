/**
 * C++ unit testing of geomodelgrids::serial::Surface.
 */

#include <portinfo>

#include "TestSurface.hh"

#include "catch2/catch_test_macros.hpp"

#include <cassert>

// ------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace serial {
        class TestSurface_Cases;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestSurface_Cases {
public:

    static TestSurface_Data* UniformResolution(void);

    static TestSurface_Data* VariableResolution(void);

};

// ------------------------------------------------------------------------------------------------
// Static
TEST_CASE("TestSurface::testConstructor", "[TestSurface][testConstructor]") {
    geomodelgrids::serial::TestSurface::testConstructor();
}
TEST_CASE("TestSurface::testSetHyperslabDims", "[TestSurface][testSetHyperslabDims]") {
    geomodelgrids::serial::TestSurface::testSetHyperslabDims();
}
TEST_CASE("TestSurface::testLoadBadMetdata", "[TestSurface][testLoadBadMetdata]") {
    geomodelgrids::serial::TestSurface::testLoadBadMetadata();
}

// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestSurface_Data*
geomodelgrids::serial::TestSurface_Cases::UniformResolution(void) {
    TestSurface_Data* data = new TestSurface_Data();assert(data);

    data->filename = "../../data/one-block-topo.h5";
    data->resolutionX = 10.0e+3;
    data->resolutionY = 10.0e+3;
    data->numX = 4;
    data->numY = 5;

    return data;
} // UniformResolution


TEST_CASE("TestSurface::UniformResolution::testAccessors", "[TestSurface][UniformResolution][testAccessors]") {
    geomodelgrids::serial::TestSurface(geomodelgrids::serial::TestSurface_Cases::UniformResolution()).testAccessors();
}
TEST_CASE("TestSurface::UniformResolution::testLoadMetadata", "[TestSurface][UniformResolution][testLoadMetadata]") {
    geomodelgrids::serial::TestSurface(geomodelgrids::serial::TestSurface_Cases::UniformResolution()).testLoadMetadata();
}
TEST_CASE("TestSurface::UniformResolution::testQuery", "[TestSurface][UniformResolution][testQuery]") {
    geomodelgrids::serial::TestSurface(geomodelgrids::serial::TestSurface_Cases::UniformResolution()).testQuery();
}

// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestSurface_Data*
geomodelgrids::serial::TestSurface_Cases::VariableResolution(void) {
    TestSurface_Data* data = new TestSurface_Data();assert(data);

    data->filename = "../../data/one-block-topo-varxy.h5";

    const size_t numX = 4;
    static const double x[numX] = {0.0e+3, 10.0e+3, 20.0e+3, 30.0e+3};

    const size_t numY = 4;
    static const double y[numY] = {0.0e+3, 8.0e+3, 24.0e+3, 40.0e+3};

    data->numX = numX;
    data->numY = numY;

    data->coordinatesX = const_cast<double*>(x);
    data->coordinatesY = const_cast<double*>(y);

    return data;
} // VariableResolution


TEST_CASE("TestSurface::VariableResolution::testAccessors", "[TestSurface][VariableResolution][testAccessors]") {
    geomodelgrids::serial::TestSurface(geomodelgrids::serial::TestSurface_Cases::VariableResolution()).testAccessors();
}
TEST_CASE("TestSurface::VariableResolution::testLoadMetadata", "[TestSurface][VariableResolution][testLoadMetadata]") {
    geomodelgrids::serial::TestSurface(geomodelgrids::serial::TestSurface_Cases::VariableResolution()).testLoadMetadata();
}
TEST_CASE("TestSurface::VariableResolution::testQuery", "[TestSurface][VariableResolution][testQuery]") {
    geomodelgrids::serial::TestSurface(geomodelgrids::serial::TestSurface_Cases::VariableResolution()).testQuery();
}

// End of file
