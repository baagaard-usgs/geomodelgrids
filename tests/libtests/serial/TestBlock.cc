/**
 * C++ unit testing of geomodelgrids::serial::Block.
 */

#include <portinfo>

#include "TestBlock.hh" // Implementation of class methods.

#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/utils/Indexing.hh" // USES Indexing

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()
#include <cassert>

// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestBlock::TestBlock(TestBlock_Data* data) :
    _data(data) {
    assert(_data);
}


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestBlock::~TestBlock(void) {
    delete _data;_data = nullptr;
}


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestBlock::testConstructor(void) {
    const std::string blockName("myblock");
    Block block(blockName.c_str());

    CHECK(blockName == block._name);
    CHECK(0.0 == block._resolutionX);
    CHECK(0.0 == block._resolutionY);
    CHECK(0.0 == block._resolutionZ);
    CHECK((double*)nullptr == block._coordinatesX);
    CHECK((double*)nullptr == block._coordinatesY);
    CHECK((double*)nullptr == block._coordinatesZ);
    CHECK((geomodelgrids::utils::Indexing*)nullptr == block._indexingX);
    CHECK((geomodelgrids::utils::Indexing*)nullptr == block._indexingY);
    CHECK((geomodelgrids::utils::Indexing*)nullptr == block._indexingZ);
    CHECK(0.0 == block._zTop);
    CHECK((double*)nullptr == block._values);
    CHECK(size_t(0) == block._numValues);
    CHECK(size_t(0) == block._dims[0]);
    CHECK(size_t(0) == block._dims[1]);
    CHECK(size_t(0) == block._dims[2]);

    CHECK(true == Block::compare(&block, nullptr));
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test setHyperslabDims.
void
geomodelgrids::serial::TestBlock::testSetHyperslabDims(void) {
    const std::string blockName("myblock");
    Block block(blockName.c_str());

    const size_t ndims = 4;
    const size_t dimsDefault[ndims] = { 64, 64, 0, 0 };
    for (size_t i = 0; i < ndims; ++i) {
        CHECK(dimsDefault[i] == block._hyperslabDims[i]);
    } // for

    const size_t dims[ndims] = { 12, 12, 4, 0 };
    block.setHyperslabDims(dims, ndims-1);
    for (size_t i = 0; i < ndims; ++i) {
        CHECK(dims[i] == block._hyperslabDims[i]);
    } // for

    CHECK_THROWS_AS(block.setHyperslabDims(dims, 5), std::length_error);
} // testSetHyperslabDims


// ------------------------------------------------------------------------------------------------
// Test loadMetadata() with bad variable resolution data.
void
geomodelgrids::serial::TestBlock::testLoadBadMetadata(void) {
    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-block-topo-varxy-bad-block-coords.h5", H5F_ACC_RDONLY);

    Block block("block");
    CHECK_THROWS_AS(block.loadMetadata(&h5), std::runtime_error);
} // testLoadBadMetadata


// ------------------------------------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestBlock::testAccessors(void) {
    REQUIRE(_data);

    Block block("myblock");

    block._resolutionX = _data->resolutionX;
    block._resolutionY = _data->resolutionY;
    block._resolutionZ = _data->resolutionZ;
    block._zTop = _data->zTop;
    block._dims[0] = _data->numX;
    block._dims[1] = _data->numY;
    block._dims[2] = _data->numZ;
    block._numValues = _data->numValues;

    CHECK(_data->resolutionX == block.getResolutionX());
    CHECK(_data->resolutionY == block.getResolutionY());
    CHECK(_data->resolutionZ == block.getResolutionZ());

    const size_t* const dims = block.getDims();
    REQUIRE(_data->numX == dims[0]);
    REQUIRE(_data->numY == dims[1]);
    REQUIRE(_data->numZ == dims[2]);
    CHECK(_data->zTop == block.getZTop());

    block._coordinatesX = _data->coordinatesX;
    block._coordinatesY = _data->coordinatesY;
    block._coordinatesZ = _data->coordinatesZ;
    const double tolerance = 1.0e-6;
    double* x = block.getCoordinatesX();
    if (_data->coordinatesX) {
        for (size_t i = 0; i < _data->numX; ++i) {
            CHECK_THAT(x[i], Catch::Matchers::WithinAbs(_data->coordinatesX[i], tolerance));
        } // for
    } // if

    if (_data->coordinatesY) {
        double* y = block.getCoordinatesY();
        for (size_t i = 0; i < _data->numY; ++i) {
            CHECK_THAT(y[i], Catch::Matchers::WithinAbs(_data->coordinatesY[i], tolerance));
        } // for
    } // if

    if (_data->coordinatesZ) {
        double* z = block.getCoordinatesZ();
        for (size_t i = 0; i < _data->numZ; ++i) {
            CHECK_THAT(z[i], Catch::Matchers::WithinAbs(_data->coordinatesZ[i], tolerance));
        } // for
    } // if

    CHECK(_data->numValues == block.getNumValues());

    // Memory managed by _data.
    block._coordinatesX = nullptr;
    block._coordinatesY = nullptr;
    block._coordinatesZ = nullptr;
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestBlock::testLoadMetadata(void) {
    REQUIRE(_data);

    geomodelgrids::serial::HDF5 h5;
    h5.open(_data->filename, H5F_ACC_RDONLY);

    Block block("block");
    block.loadMetadata(&h5);

    CHECK(_data->resolutionX == block.getResolutionX());
    CHECK(_data->resolutionY == block.getResolutionY());
    CHECK(_data->resolutionZ == block.getResolutionZ());

    const size_t* const dims = block.getDims();
    REQUIRE(_data->numX == dims[0]);
    REQUIRE(_data->numY == dims[1]);
    REQUIRE(_data->numZ == dims[2]);

    const double tolerance = 1.0e-6;
    double* x = block.getCoordinatesX();
    if (_data->coordinatesX) {
        for (size_t i = 0; i < _data->numX; ++i) {
            CHECK_THAT(x[i], Catch::Matchers::WithinAbs(_data->coordinatesX[i], tolerance));
        } // for
    } // if

    if (_data->coordinatesY) {
        double* y = block.getCoordinatesY();
        for (size_t i = 0; i < _data->numY; ++i) {
            CHECK_THAT(y[i], Catch::Matchers::WithinAbs(_data->coordinatesY[i], tolerance));
        } // for
    } // if

    if (_data->coordinatesZ) {
        double* z = block.getCoordinatesZ();
        for (size_t i = 0; i < _data->numZ; ++i) {
            CHECK_THAT(z[i], Catch::Matchers::WithinAbs(_data->coordinatesZ[i], tolerance));
        } // for
        CHECK_THAT(block.getZBottom(), Catch::Matchers::WithinAbs(z[_data->numZ-1], tolerance));
    } // if

    CHECK(block._indexingX);
    CHECK(block._indexingY);
    CHECK(block._indexingZ);

    CHECK(_data->numValues == block.getNumValues());
} // testLoadMetadata


// ------------------------------------------------------------------------------------------------
// Test query().
void
geomodelgrids::serial::TestBlock::testQuery(void) {
    REQUIRE(_data);

    geomodelgrids::serial::HDF5 h5;
    h5.open(_data->filename, H5F_ACC_RDONLY);

    Block block("block");
    block.loadMetadata(&h5);
    block.openQuery(&h5);

    const size_t spaceDim = 3;
    REQUIRE(_data->points);
    const geomodelgrids::testdata::ModelPoints* points = _data->points;
    const size_t numPoints = points->getNumPoints();
    const double* pointsXYZ = points->getXYZ();
    const double* pointsLLE = points->getLatLonElev();

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double* values = block.query(pointsXYZ[iPt*spaceDim+0], pointsXYZ[iPt*spaceDim+1], pointsXYZ[iPt*spaceDim+2]);

        const double x = pointsXYZ[iPt*spaceDim+0];
        const double y = pointsXYZ[iPt*spaceDim+1];
        const double z = pointsXYZ[iPt*spaceDim+2];

        { // Value 'one'
            const double valueE = points->computeValueOne(x, y, z);

            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 'one'.");
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CHECK_THAT(values[0], Catch::Matchers::WithinAbs(valueE, valueTolerance));
        } // Value 'one'

        { // Value 'two'
            const double valueE = points->computeValueTwo(x, y, z);

            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 1.");
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CHECK_THAT(values[1], Catch::Matchers::WithinAbs(valueE, valueTolerance));
        } // Value 'two'
    } // for

    block.closeQuery();
} // testQuery


// ------------------------------------------------------------------------------------------------
const size_t geomodelgrids::serial::TestBlock_Data::numValues = 2;

// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestBlock_Data::TestBlock_Data(void) :
    filename(nullptr),
    resolutionX(0.0),
    resolutionY(0.0),
    resolutionZ(0.0),
    zTop(0.0),
    coordinatesX(nullptr),
    coordinatesY(nullptr),
    coordinatesZ(nullptr),
    numX(0),
    numY(0),
    numZ(0),
    points(nullptr) {}


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestBlock_Data::~TestBlock_Data(void) {
    delete points;points = nullptr;
}


// End of file
