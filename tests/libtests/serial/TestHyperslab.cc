/**
 * C++ unit testing of geomodelgrids::serial::Model.
 */

#include <portinfo>

#include "ModelPoints.hh"

#include "geomodelgrids/serial/Hyperslab.hh" // Test subject

#include "geomodelgrids/serial/HDF5.hh" // HASA HDF5

#include <cppunit/extensions/HelperMacros.h>

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace serial {
        class TestHyperslab;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestHyperslab : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE -------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE(TestHyperslab);

    CPPUNIT_TEST(testConstructor2D);
    CPPUNIT_TEST(testConstructor3D);
    CPPUNIT_TEST(testConstructorOversize3D);
    CPPUNIT_TEST(testConstructorBadDims);
    CPPUNIT_TEST(testInterpolate2D);
    CPPUNIT_TEST(testInterpolate3D);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Set up test data.
    void setUp(void);

    /// Tear down test data.
    void tearDown(void);

    /// Test constructor in 2D.
    void testConstructor2D(void);

    /// Test constructor in 3D.
    void testConstructor3D(void);

    /// Test constructor with oversize dims in 3D.
    void testConstructorOversize3D(void);

    /// Test constructor bad dimensions.
    void testConstructorBadDims(void);

    /// Test interpolate in 2D.
    void testInterpolate2D(void);

    /// Test interpolate in 2D.
    void testInterpolate3D(void);

    // PRIVATE MEMBERS ----------------------------------------------------------------------------
private:

    geomodelgrids::serial::HDF5 _h5;

}; // class TestHyperslab
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestHyperslab);

// ------------------------------------------------------------------------------------------------
// Set up test data.
void
geomodelgrids::serial::TestHyperslab::setUp(void) {
    _h5.open("../../data/one-block-topo.h5", H5F_ACC_RDONLY);
} // setUp


// ------------------------------------------------------------------------------------------------
// Tear down test data.
void
geomodelgrids::serial::TestHyperslab::tearDown(void) {
    _h5.close();
} // tearDown


// ------------------------------------------------------------------------------------------------
// Test constructor in 2D.
void
geomodelgrids::serial::TestHyperslab::testConstructor2D(void) {
    const std::string dataset("/surfaces/top_surface");
    const size_t ndims(3);
    const hsize_t dims[ndims] = { 3, 4, 1 };
    const hsize_t dimsAll[ndims] = { 4, 5, 1 };

    Hyperslab hyperslab(&_h5, dataset.c_str(), dims, ndims);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in HDF5 file object.", &_h5, hyperslab._h5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in dataset path.", dataset, hyperslab._datasetPath);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of dimensions for hyperslab.", ndims, hyperslab._ndims);
    CPPUNIT_ASSERT_MESSAGE("Expected NULL origin.", !hyperslab._origin);

    for (size_t i = 0; i < ndims; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in hyperslab dimensions.", dims[i], hyperslab._dims[i]);
    } // for

    for (size_t i = 0; i < ndims; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in dataset dimensions.", dimsAll[i], hyperslab._dimsAll[i]);
    } // for

    CPPUNIT_ASSERT_MESSAGE("Expected non-NULL values.", hyperslab._values);
    CPPUNIT_ASSERT_MESSAGE("Expected non-NULL hyperslab.", hyperslab._hyperslab);
} // testConstructor2D


// ------------------------------------------------------------------------------------------------
// Test constructor in 3D.
void
geomodelgrids::serial::TestHyperslab::testConstructor3D(void) {
    const std::string dataset("/blocks/block");
    const size_t ndims(4);
    const hsize_t dims[ndims] = { 3, 2, 1, 2 };
    const hsize_t dimsAll[ndims] = { 4, 5, 2, 2 };

    Hyperslab hyperslab(&_h5, dataset.c_str(), dims, ndims);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in HDF5 file object.", &_h5, hyperslab._h5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in dataset path.", dataset, hyperslab._datasetPath);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of dimensions for hyperslab.", ndims, hyperslab._ndims);
    CPPUNIT_ASSERT_MESSAGE("Expected NULL origin.", !hyperslab._origin);

    for (size_t i = 0; i < ndims; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in hyperslab dimensions.", dims[i], hyperslab._dims[i]);
    } // for

    for (size_t i = 0; i < ndims; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in dataset dimensions.", dimsAll[i], hyperslab._dimsAll[i]);
    } // for

    CPPUNIT_ASSERT_MESSAGE("Expected non-NULL values.", hyperslab._values);
    CPPUNIT_ASSERT_MESSAGE("Expected non-NULL hyperslab.", hyperslab._hyperslab);
} // testConstructor3D


// ------------------------------------------------------------------------------------------------
// Test constructor with oversize dimensions in 3D.
void
geomodelgrids::serial::TestHyperslab::testConstructorOversize3D(void) {
    const std::string dataset("/blocks/block");
    const size_t ndims(4);
    const hsize_t dims[ndims] = { 6, 6, 3, 4 };
    const hsize_t dimsAll[ndims] = { 4, 5, 2, 2 };

    Hyperslab hyperslab(&_h5, dataset.c_str(), dims, ndims);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in HDF5 file object.", &_h5, hyperslab._h5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in dataset path.", dataset, hyperslab._datasetPath);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of dimensions for hyperslab.", ndims, hyperslab._ndims);
    CPPUNIT_ASSERT_MESSAGE("Expected NULL origin.", !hyperslab._origin);

    for (size_t i = 0; i < ndims; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in hyperslab dimensions.", dimsAll[i], hyperslab._dims[i]);
    } // for

    for (size_t i = 0; i < ndims; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in dataset dimensions.", dimsAll[i], hyperslab._dimsAll[i]);
    } // for

    CPPUNIT_ASSERT_MESSAGE("Expected non-NULL values.", hyperslab._values);
    CPPUNIT_ASSERT_MESSAGE("Expected non-NULL hyperslab.", hyperslab._hyperslab);
} // testConstructorOversize3D


// ------------------------------------------------------------------------------------------------
// Test constructor with bad dims.
void
geomodelgrids::serial::TestHyperslab::testConstructorBadDims(void) {
    const std::string dataset("/blocks/block");
    const size_t ndims(2);
    const hsize_t dims[ndims] = { 1, 1 };

    CPPUNIT_ASSERT_THROW(Hyperslab hyperslab(&_h5, dataset.c_str(), dims, ndims), std::length_error);
} // testConstructorBadDims


// ------------------------------------------------------------------------------------------------
// Test interpolate in 2D.
void
geomodelgrids::serial::TestHyperslab::testInterpolate2D(void) {
    const std::string dataset("/surfaces/top_surface");
    const size_t ndims(3);
    const hsize_t dims[ndims] = { 3, 2, 1 };

    const size_t npoints(7);
    const size_t spaceDim = 2;
    const double index[npoints*spaceDim] = {
        0.0, 1.0,
        1.3, 1.2,
        2.4, 2.5,
        1.2, 3.9,
        0.6, 4.0,
        3.0, 1.5,
        2.1, 0.3,
    };

    Hyperslab hyperslab(&_h5, dataset.c_str(), dims, ndims);

    double dx = 0.0;
    double dy = 0.0;
    _h5.readAttribute(dataset.c_str(), "x_resolution", H5T_NATIVE_DOUBLE, &dx);
    _h5.readAttribute(dataset.c_str(), "y_resolution", H5T_NATIVE_DOUBLE, &dy);

    double elevation = -999.0;
    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < npoints; ++i) {
        hyperslab.interpolate(&elevation, &index[i*spaceDim]);

        const double x = dx * index[i*spaceDim + 0];
        const double y = dy * index[i*spaceDim + 1];
        const double elevationE = geomodelgrids::testdata::ModelPoints::computeTopElevation(x, y);

        std::ostringstream msg;
        msg << "Mismatch in elevation for index (" << index[i*spaceDim+0] << ", " << index[i*spaceDim+1] << ").";
        const double toleranceV = std::max(tolerance, tolerance*fabs(elevationE));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, toleranceV);
    } // for
} // testInterplate2D


// ------------------------------------------------------------------------------------------------
// Test interpolate in 3D.
void
geomodelgrids::serial::TestHyperslab::testInterpolate3D(void) {
    const std::string dataset("/blocks/block");
    const size_t ndims(4);
    const hsize_t dims[ndims] = { 2, 3, 2, 2 };

    const size_t npoints(7);
    const size_t spaceDim = 3;
    const double index[npoints*spaceDim] = {
        0.0, 1.0, 0.2,
        1.3, 1.2, 0.3,
        2.4, 2.5, 0.9,
        1.2, 3.9, 0.0,
        0.6, 4.0, 0.5,
        3.0, 1.5, 0.8,
        2.1, 0.3, 0.3,
    };

    Hyperslab hyperslab(&_h5, dataset.c_str(), dims, ndims);

    double dx = 0.0;
    double dy = 0.0;
    double dz = 0.0;
    double zTop = 0.0;
    _h5.readAttribute(dataset.c_str(), "x_resolution", H5T_NATIVE_DOUBLE, &dx);
    _h5.readAttribute(dataset.c_str(), "y_resolution", H5T_NATIVE_DOUBLE, &dy);
    _h5.readAttribute(dataset.c_str(), "z_resolution", H5T_NATIVE_DOUBLE, &dz);
    _h5.readAttribute(dataset.c_str(), "z_top", H5T_NATIVE_DOUBLE, &zTop);

    double values[2] = { -999.0, -999.0 };
    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < npoints; ++i) {
        hyperslab.interpolate(values, &index[i*spaceDim]);

        const double x = dx * index[i*spaceDim + 0];
        const double y = dx * index[i*spaceDim + 1];
        const double z = zTop - dz * index[i*spaceDim + 2];

        { // Value 0
            const double valueE = geomodelgrids::testdata::ModelPoints::computeValueOne(x, y, z);
            std::ostringstream msg;
            msg << "Mismatch in value 'one' for index (" << index[i*spaceDim+0] << ", " << index[i*spaceDim+1]
                << ", " << index[i*spaceDim+2] << ").";
            const double toleranceV = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[0], toleranceV);
        } // Value 0

        { // Value 1
            const double valueE = geomodelgrids::testdata::ModelPoints::computeValueTwo(x, y, z);
            std::ostringstream msg;
            msg << "Mismatch in value 'two' for index (" << index[i*spaceDim+0] << ", " << index[i*spaceDim+1]
                << ", " << index[i*spaceDim+2] << ").";
            const double toleranceV = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[1], toleranceV);
        } // Value 1
    } // for
} // testInterplate3D


// End of file
