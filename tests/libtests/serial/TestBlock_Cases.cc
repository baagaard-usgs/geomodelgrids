/**
 * C++ unit testing of geomodelgrids::serial::Block.
 */

#include <portinfo>

#include "TestBlock.hh"

#include "catch2/catch_test_macros.hpp"

// ------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace serial {
        class TestBlock_Cases;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestBlock_Cases {
public:

    static TestBlock_Data* Uniform(void);

    static TestBlock_Data* VarZ(void);

    static TestBlock_Data* VarXY(void);

};

// ------------------------------------------------------------------------------------------------
// Static
TEST_CASE("TestBlock::testConstructor", "[TestBlock][testConstructor]") {
    geomodelgrids::serial::TestBlock::testConstructor();
}
TEST_CASE("TestBlock::testSetHyperslabDims", "[TestBlock][testSetHyperslabDims]") {
    geomodelgrids::serial::TestBlock::testSetHyperslabDims();
}
TEST_CASE("TestBlock::testLoadBadMetdata", "[TestBlock][testLoadBadMetdata]") {
    geomodelgrids::serial::TestBlock::testLoadBadMetadata();
}

// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestBlock_Data*
geomodelgrids::serial::TestBlock_Cases::Uniform(void) {
    TestBlock_Data* data = new TestBlock_Data();assert(data);

    data->filename = "../../data/one-block-flat.h5";
    data->resolutionX = 8.0e+3;
    data->resolutionY = 10.0e+3;
    data->resolutionZ = 5.0e+3;
    data->zTop = 0.0;
    data->numX = 5;
    data->numY = 5;
    data->numZ = 2;

    data->points = new geomodelgrids::testdata::OneBlockFlatPoints;

    return data;
} // Uniform


TEST_CASE("TestBlock::Uniform::testAccessors", "[TestBlock][Uniform][testAccessors]") {
    geomodelgrids::serial::TestBlock(geomodelgrids::serial::TestBlock_Cases::Uniform()).testAccessors();
}
TEST_CASE("TestBlock::Uniform::testLoadMetadata", "[TestBlock][Uniform][testLoadMetadata]") {
    geomodelgrids::serial::TestBlock(geomodelgrids::serial::TestBlock_Cases::Uniform()).testLoadMetadata();
}
TEST_CASE("TestBlock::Uniform::testQuery", "[TestBlock][Uniform][testQuery]") {
    geomodelgrids::serial::TestBlock(geomodelgrids::serial::TestBlock_Cases::Uniform()).testQuery();
}

// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestBlock_Data*
geomodelgrids::serial::TestBlock_Cases::VarZ(void) {
    TestBlock_Data* data = new TestBlock_Data();assert(data);

    data->filename = "../../data/one-block-flat-varz.h5";
    data->resolutionX = 8.0e+3;
    data->resolutionY = 10.0e+3;
    static const size_t numZ = 3;
    static const double z[numZ] = { 0.0e+3, -2.0e+3, -5.0e+3 };
    data->coordinatesZ = const_cast<double*>(z);
    data->zTop = z[0];
    data->numX = 5;
    data->numY = 5;
    data->numZ = numZ;

    data->points = new geomodelgrids::testdata::OneBlockFlatPoints;

    return data;
} // VarZ


TEST_CASE("TestBlock::VarZ::testAccessors", "[TestBlock][VarZ][testAccessors]") {
    geomodelgrids::serial::TestBlock(geomodelgrids::serial::TestBlock_Cases::VarZ()).testAccessors();
}
TEST_CASE("TestBlock::VarZ::testLoadMetadata", "[TestBlock][VarZ][testLoadMetadata]") {
    geomodelgrids::serial::TestBlock(geomodelgrids::serial::TestBlock_Cases::VarZ()).testLoadMetadata();
}
TEST_CASE("TestBlock::VarZ::testQuery", "[TestBlock][VarZ][testQuery]") {
    geomodelgrids::serial::TestBlock(geomodelgrids::serial::TestBlock_Cases::VarZ()).testQuery();
}

// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestBlock_Data*
geomodelgrids::serial::TestBlock_Cases::VarXY(void) {
    TestBlock_Data* data = new TestBlock_Data();assert(data);

    data->filename = "../../data/one-block-topo-varxy.h5";

    static const size_t numX = 4;
    static const double x[numX] = { 0.0e+3, 10.0e+3, 20.0e+3, 30.0e+3 };
    data->coordinatesX = const_cast<double*>(x);

    static const size_t numY = 4;
    static const double y[numY] = { 0.0e+3, 8.0e+3, 16.0e+3, 40.0e+3 };
    data->coordinatesY = const_cast<double*>(y);

    data->resolutionZ = 5.0e+3;
    data->zTop = 0.0;
    data->numX = numX;
    data->numY = numY;
    data->numZ = 2;

    data->points = new geomodelgrids::testdata::OneBlockTopoPoints;

    return data;
} // VarXY


TEST_CASE("TestBlock::VarXY::testAccessors", "[TestBlock][VarXY][testAccessors]") {
    geomodelgrids::serial::TestBlock(geomodelgrids::serial::TestBlock_Cases::VarXY()).testAccessors();
}
TEST_CASE("TestBlock::VarXY::testLoadMetadata", "[TestBlock][VarXY][testLoadMetadata]") {
    geomodelgrids::serial::TestBlock(geomodelgrids::serial::TestBlock_Cases::VarXY()).testLoadMetadata();
}
TEST_CASE("TestBlock::VarXY::testQuery", "[TestBlock][VarXY][testQuery]") {
    geomodelgrids::serial::TestBlock(geomodelgrids::serial::TestBlock_Cases::VarXY()).testQuery();
}

// End of file
