/**
 * C++ unit testing of geomodelgrids::serial::Block.
 */

#include <portinfo>

#include "TestBlock.hh" // Implementation of class methods.

#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/utils/Indexing.hh" // USES Indexing

#include <cmath> // USES fabs()

// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::TestBlock::setUp(void) {
    _data = new TestBlock_Data();
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::TestBlock::tearDown(void) {
    delete _data;_data = NULL;
}


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestBlock::testConstructor(void) {
    const std::string blockName("myblock");
    Block block(blockName.c_str());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking name", blockName, block._name);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking x resolution", 0.0, block._resolutionX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y resolution", 0.0, block._resolutionY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking z resolution", 0.0, block._resolutionZ);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking x coordinates", (double*)NULL, block._coordinatesX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y coordinates", (double*)NULL, block._coordinatesY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y coordinates", (double*)NULL, block._coordinatesZ);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checknig x indexing", (geomodelgrids::utils::Indexing*)NULL, block._indexingX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checknig y indexing", (geomodelgrids::utils::Indexing*)NULL, block._indexingY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checknig z indexing", (geomodelgrids::utils::Indexing*)NULL, block._indexingZ);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking zero z top", 0.0, block._zTop);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking values buffer", (double*)NULL, block._values);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking numValues", size_t(0), block._numValues);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[0]", size_t(0), block._dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[1]", size_t(0), block._dims[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[2]", size_t(0), block._dims[2]);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking Block::compare()", true, Block::compare(&block, NULL));
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestBlock::testAccessors(void) {
    CPPUNIT_ASSERT(_data);

    Block block("myblock");

    block._resolutionX = _data->resolutionX;
    block._resolutionY = _data->resolutionY;
    block._resolutionZ = _data->resolutionZ;
    block._zTop = _data->zTop;
    block._dims[0] = _data->numX;
    block._dims[1] = _data->numY;
    block._dims[2] = _data->numZ;
    block._numValues = _data->numValues;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking x resolution", _data->resolutionX, block.getResolutionX());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y resolution", _data->resolutionY, block.getResolutionY());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking z resolution", _data->resolutionZ, block.getResolutionZ());

    const size_t* const dims = block.getDims();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in x dimension.", _data->numX, dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in y dimension.", _data->numY, dims[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in z dimension.", _data->numZ, dims[2]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking z top", _data->zTop, block.getZTop());

    block._coordinatesX = _data->coordinatesX;
    block._coordinatesY = _data->coordinatesY;
    block._coordinatesZ = _data->coordinatesZ;
    const double tolerance = 1.0e-6;
    double* x = block.getCoordinatesX();
    if (_data->coordinatesX) {
        for (size_t i = 0; i < _data->numX; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in x coordinates", _data->coordinatesX[i], x[i], tolerance);
        } // for
    } // if

    if (_data->coordinatesY) {
        double* y = block.getCoordinatesY();
        for (size_t i = 0; i < _data->numY; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in y coordinates", _data->coordinatesY[i], y[i], tolerance);
        } // for
    } // if

    if (_data->coordinatesZ) {
        double* z = block.getCoordinatesZ();
        for (size_t i = 0; i < _data->numZ; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in z coordinates", _data->coordinatesZ[i], z[i], tolerance);
        } // for
    } // if

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking numValues", _data->numValues, block.getNumValues());

    // Memory managed by _data.
    block._coordinatesX = NULL;
    block._coordinatesY = NULL;
    block._coordinatesZ = NULL;
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test setHyperslabDims.
void
geomodelgrids::serial::TestBlock::testSetHyperslabDims(void) {
    const std::string blockName("myblock");
    Block block(blockName.c_str());

    const size_t ndims = 4;
    const size_t dimsDefault[ndims] = { 64, 64, 0, 0 };
    for (size_t i = 0; i < ndims; ++i) {
        std::ostringstream msg;
        msg << "Mismath in default hyperslab dim " << i << ".";
        CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str().c_str(), dimsDefault[i], block._hyperslabDims[i]);
    } // for

    const size_t dims[ndims] = { 12, 12, 4, 0 };
    block.setHyperslabDims(dims, ndims-1);
    for (size_t i = 0; i < ndims; ++i) {
        std::ostringstream msg;
        msg << "Mismath in user hyperslab dim " << i << ".";
        CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str().c_str(), dims[i], block._hyperslabDims[i]);
    } // for

    CPPUNIT_ASSERT_THROW(block.setHyperslabDims(dims, 5), std::length_error);
} // testSetHyperslabDims


// ------------------------------------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestBlock::testLoadMetadata(void) {
    CPPUNIT_ASSERT(_data);

    geomodelgrids::serial::HDF5 h5;
    h5.open(_data->filename, H5F_ACC_RDONLY);

    Block block("block");
    block.loadMetadata(&h5);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking x resolution", _data->resolutionX, block.getResolutionX());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y resolution", _data->resolutionY, block.getResolutionY());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking z resolution", _data->resolutionZ, block.getResolutionZ());

    const size_t* const dims = block.getDims();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in x dimension.", _data->numX, dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in y dimension.", _data->numY, dims[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in z dimension.", _data->numZ, dims[2]);

    const double tolerance = 1.0e-6;
    double* x = block.getCoordinatesX();
    if (_data->coordinatesX) {
        for (size_t i = 0; i < _data->numX; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in x coordinates", _data->coordinatesX[i], x[i], tolerance);
        } // for
    } // if

    if (_data->coordinatesY) {
        double* y = block.getCoordinatesY();
        for (size_t i = 0; i < _data->numY; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in y coordinates", _data->coordinatesY[i], y[i], tolerance);
        } // for
    } // if

    if (_data->coordinatesZ) {
        double* z = block.getCoordinatesZ();
        for (size_t i = 0; i < _data->numZ; ++i) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in z coordinates", _data->coordinatesZ[i], z[i], tolerance);
        } // for
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in z bottom", z[_data->numZ-1], block.getZBottom(), tolerance);
    } // if

    CPPUNIT_ASSERT_MESSAGE("Mismatch in x indexing", block._indexingX);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in y indexing", block._indexingY);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in z indexing", block._indexingZ);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking numValues", _data->numValues, block.getNumValues());
} // testLoadMetadata


// ------------------------------------------------------------------------------------------------
// Test loadMetadata() with bad variable resolution data.
void
geomodelgrids::serial::TestBlock::testLoadBadMetadata(void) {
    CPPUNIT_ASSERT(_data);

    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-block-topo-varxy-bad-block-coords.h5", H5F_ACC_RDONLY);

    Block block("block");
    CPPUNIT_ASSERT_THROW_MESSAGE("Failed to detect wrong number of coordinates.",
                                 block.loadMetadata(&h5), std::runtime_error);
} // testLoadBadMetadata


// ------------------------------------------------------------------------------------------------
// Test query().
void
geomodelgrids::serial::TestBlock::testQuery(void) {
    CPPUNIT_ASSERT(_data);

    geomodelgrids::serial::HDF5 h5;
    h5.open(_data->filename, H5F_ACC_RDONLY);

    Block block("block");
    block.loadMetadata(&h5);
    block.openQuery(&h5);

    const size_t spaceDim = 3;
    CPPUNIT_ASSERT(_data->points);
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

            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 'one'.";
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[0], valueTolerance);
        } // Value 'one'

        { // Value 'two'
            const double valueE = points->computeValueTwo(x, y, z);

            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 1.";
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[1], valueTolerance);
        } // Value 'two'
    } // for

    block.closeQuery();
} // testQuery


// ------------------------------------------------------------------------------------------------
const size_t geomodelgrids::serial::TestBlock_Data::numValues = 2;

// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestBlock_Data::TestBlock_Data(void) :
    filename(NULL),
    resolutionX(0.0),
    resolutionY(0.0),
    resolutionZ(0.0),
    zTop(0.0),
    coordinatesX(NULL),
    coordinatesY(NULL),
    coordinatesZ(NULL),
    numX(0),
    numY(0),
    numZ(0),
    points(NULL) {}


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::TestBlock_Data::~TestBlock_Data(void) {
    delete points;points = NULL;
}


// End of file
