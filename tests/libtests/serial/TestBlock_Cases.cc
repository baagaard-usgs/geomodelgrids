/**
 * C++ unit testing of geomodelgrids::serial::Block.
 */

#include <portinfo>

#include "TestBlock.hh"

namespace geomodelgrids {
    namespace serial {
        class TestBlock_Uniform;
        class TestBlock_VarZ;
        class TestBlock_VarXY;
    } // serial
} // geomodelgrids

// ================================================================================================
class geomodelgrids::serial::TestBlock_Uniform : public geomodelgrids::serial::TestBlock {
    CPPUNIT_TEST_SUB_SUITE(TestBlock_Uniform, TestBlock);
    CPPUNIT_TEST_SUITE_END();

protected:

    void setUp(void) {
        TestBlock::setUp();

        CPPUNIT_ASSERT(_data);

        _data->filename = "../../data/one-block-flat.h5";
        _data->resolutionX = 8.0e+3;
        _data->resolutionY = 10.0e+3;
        _data->resolutionZ = 5.0e+3;
        _data->zTop = 0.0;
        _data->numX = 5;
        _data->numY = 5;
        _data->numZ = 2;

        _data->points = new geomodelgrids::testdata::OneBlockFlatPoints;
    } // setUp

}; // TestBlock_Uniform
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestBlock_Uniform);

// ================================================================================================
class geomodelgrids::serial::TestBlock_VarZ : public geomodelgrids::serial::TestBlock {
    CPPUNIT_TEST_SUB_SUITE(TestBlock_VarZ, TestBlock);
    CPPUNIT_TEST_SUITE_END();

protected:

    void setUp(void) {
        TestBlock::setUp();

        CPPUNIT_ASSERT(_data);

        _data->filename = "../../data/one-block-flat-varz.h5";
        _data->resolutionX = 8.0e+3;
        _data->resolutionY = 10.0e+3;
        static const size_t numZ = 3;
        static const double z[numZ] = { 0.0e+3, -2.0e+3, -5.0e+3 };
        _data->coordinatesZ = const_cast<double*>(z);
        _data->zTop = z[0];
        _data->numX = 5;
        _data->numY = 5;
        _data->numZ = numZ;

        _data->points = new geomodelgrids::testdata::OneBlockFlatPoints;
    } // setUp

}; // TestBlock_VarZ
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestBlock_VarZ);

// ================================================================================================
class geomodelgrids::serial::TestBlock_VarXY : public geomodelgrids::serial::TestBlock {
    CPPUNIT_TEST_SUB_SUITE(TestBlock_VarXY, TestBlock);
    CPPUNIT_TEST_SUITE_END();

protected:

    void setUp(void) {
        TestBlock::setUp();

        CPPUNIT_ASSERT(_data);

        _data->filename = "../../data/one-block-topo-varxy.h5";

        static const size_t numX = 4;
        static const double x[numX] = { 0.0e+3, 10.0e+3, 20.0e+3, 30.0e+3 };
        _data->coordinatesX = const_cast<double*>(x);

        static const size_t numY = 4;
        static const double y[numY] = { 0.0e+3, 8.0e+3, 16.0e+3, 40.0e+3 };
        _data->coordinatesY = const_cast<double*>(y);

        _data->resolutionZ = 5.0e+3;
        _data->zTop = 0.0;
        _data->numX = numX;
        _data->numY = numY;
        _data->numZ = 2;

        _data->points = new geomodelgrids::testdata::OneBlockTopoPoints;
    } // setUp

}; // TestBlock_VarXY
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestBlock_VarXY);

// End of file
