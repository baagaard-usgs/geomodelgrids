/**
 * C++ unit testing of geomodelgrids::serial::HDF5.
 */

#include <portinfo>

#include "ModelPoints.hh" // USES ModelPoints

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cppunit/extensions/HelperMacros.h>

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace serial {
        class TestHDF5;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestHDF5 : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE -------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE(TestHDF5);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testOpenClose);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testGetDatasetDims);
    CPPUNIT_TEST(testGetGroupDatasets);
    CPPUNIT_TEST(testHasAttribute);
    CPPUNIT_TEST(testReadAttribute);
    CPPUNIT_TEST(testReadAttributeString);
    CPPUNIT_TEST(testReadAttributeStringArray);
    CPPUNIT_TEST(testReadDatasetHyperslab);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Setup test.
    void setUp(void);

    /// Clean up test.
    void tearDown(void);

    /// Test constructor.
    void testConstructor(void);

    /// Test open(), isOpen(), close().
    void testOpenClose(void);

    /// Test getters.
    void testAccessors(void);

    /// Test getDatasetDims().
    void testGetDatasetDims(void);

    /// Test getGroupDatasets().
    void testGetGroupDatasets(void);

    /// Test hasAttribute.
    void testHasAttribute(void);

    /// Test readAttribute.
    void testReadAttribute(void);

    /// Test readAttributeString().
    void testReadAttributeString(void);

    /// Test readAttributeStringArray().
    void testReadAttributeStringArray(void);

    /// Test readDatasetHyperslab().
    void testReadDatasetHyperslab(void);

private:

    H5E_auto2_t _errFunc;
    void* _errData;

}; // class TestHDF5
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestHDF5);

// ------------------------------------------------------------------------------------------------
// Setup test.
void
geomodelgrids::serial::TestHDF5::setUp(void) {
    // Temporarily turn off HDF5 error handler.
    H5Eget_auto(H5E_DEFAULT, &_errFunc, &_errData);
    H5Eset_auto(H5E_DEFAULT, NULL, NULL);
} // setUp


// ------------------------------------------------------------------------------------------------
// Clean up after test.
void
geomodelgrids::serial::TestHDF5::tearDown(void) {
    // Restore default HDF5 error handler.
    H5Eset_auto(H5E_DEFAULT, _errFunc, _errData);
} // tearDown


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestHDF5::testConstructor(void) {
    HDF5 h5;

    CPPUNIT_ASSERT_EQUAL(hid_t(-1), h5._file);

    // Check default cache parameters.
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in cache size.", size_t(128*1048576), h5._cacheSize);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of slots for cache.", size_t(63997), h5._cacheNumSlots);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in cache preemption policy.", 0.75, h5._cachePreemption);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test open(), isOpen(), close().
void
geomodelgrids::serial::TestHDF5::testOpenClose(void) {
    HDF5 h5;

    CPPUNIT_ASSERT(!h5.isOpen());

    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDONLY);CPPUNIT_ASSERT(h5.isOpen());
    h5.close();CPPUNIT_ASSERT(!h5.isOpen());

    const size_t cacheSize = 1048576;
    const size_t cacheNumSlots = 37;
    const double cachePreemption = 0.6;
    h5.setCache(cacheSize, cacheNumSlots, cachePreemption);

    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDWR);CPPUNIT_ASSERT(h5.isOpen());
    h5.close();CPPUNIT_ASSERT(!h5.isOpen());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in cache size.", cacheSize, h5._cacheSize);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of slots for cache.", cacheNumSlots, h5._cacheNumSlots);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in cache preemption policy.", cachePreemption, h5._cachePreemption);

    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDWR);CPPUNIT_ASSERT(h5.isOpen());
    CPPUNIT_ASSERT_THROW(h5.open("../../data/one-block-flat.h5", H5F_ACC_RDONLY), std::runtime_error); // already open
    h5.close();CPPUNIT_ASSERT(!h5.isOpen());

    CPPUNIT_ASSERT_THROW(h5.open("abc", H5F_ACC_RDONLY), std::runtime_error); // file doesn't exist
} // testOpenClose


// ------------------------------------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestHDF5::testAccessors(void) {
    HDF5 h5;

    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    CPPUNIT_ASSERT(h5.hasGroup("blocks"));
    CPPUNIT_ASSERT(!h5.hasGroup("abc"));

    CPPUNIT_ASSERT(h5.hasDataset("/blocks/bottom"));
    CPPUNIT_ASSERT(!h5.hasDataset("/topography"));

    h5.close();
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test getDatasetDims().
void
geomodelgrids::serial::TestHDF5::testGetDatasetDims(void) {
    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    const int ndimsE = 4;
    const hsize_t dimsE[ndimsE] = { 4, 7, 3, 2 };
    hsize_t* dims = NULL;
    int ndims = 0;
    h5.getDatasetDims(&dims, &ndims, "/blocks/middle");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in dataset rank.", ndimsE, ndims);
    for (int i = 0; i < ndimsE; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in dataset dim.", dimsE[i], dims[i]);
    } // for
    delete[] dims;dims = NULL;

    CPPUNIT_ASSERT_THROW(h5.getDatasetDims(&dims, &ndims, "blah"), std::runtime_error);

    h5.close();
} // testGetDatasetDims


// ------------------------------------------------------------------------------------------------
// Test getGroupDatasets().
void
geomodelgrids::serial::TestHDF5::testGetGroupDatasets(void) {
    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    const size_t numDatasets = 3;
    const char* datasetNamesE[numDatasets] = { "top", "middle", "bottom" };
    std::vector<std::string> datasetNames;
    h5.getGroupDatasets(&datasetNames, "/blocks");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of group datasets.", numDatasets, datasetNames.size());
    for (size_t i = 0; i < numDatasets; ++i) {
        bool found = false;
        for (size_t j = 0; j < numDatasets; ++j) {
            if (std::string(datasetNamesE[i]) == datasetNames[j]) {
                found = true;
                break;
            } // if
        } // for
        std::ostringstream msg;
        msg << "Did not find dataset " << datasetNamesE[i] << ".";
        CPPUNIT_ASSERT_MESSAGE(msg.str().c_str(), found);
    } // for

    CPPUNIT_ASSERT_THROW(h5.getGroupDatasets(&datasetNames, "blah"), std::runtime_error);

    h5.close();
} // testGetGroupDatasets


// ------------------------------------------------------------------------------------------------
// Test hasAttribute.
void
geomodelgrids::serial::TestHDF5::testHasAttribute(void) {
    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    bool found = h5.hasAttribute("/blocks/bottom", "x_resolution");
    CPPUNIT_ASSERT_MESSAGE("Couldn't find existing attribute.", found);

    found = h5.hasAttribute("/blocks/bottom", "abcdf");
    CPPUNIT_ASSERT_MESSAGE("Found nonexistent attribute.", !found);

    h5.close();
} // testReadAttribute


// ------------------------------------------------------------------------------------------------
// Test readAttribute.
void
geomodelgrids::serial::TestHDF5::testReadAttribute(void) {
    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    const double resolutionE = 30e+3;
    double resolution = 0.0;
    const double tolerance = 1.0e-6;
    h5.readAttribute("/blocks/bottom", "x_resolution", H5T_NATIVE_DOUBLE, (void*)&resolution);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(resolutionE, resolution, tolerance*resolutionE);

    CPPUNIT_ASSERT_THROW(h5.readAttribute("/blocks/bottom", "blah", H5T_NATIVE_DOUBLE, (void*)&resolution),
                         std::runtime_error);

    h5.close();
} // testReadAttribute


// ------------------------------------------------------------------------------------------------
// Test readAttributeString().
void
geomodelgrids::serial::TestHDF5::testReadAttributeString(void) {
    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    const std::string versionE = "1.0.0";
    const std::string version = h5.readAttribute("/", "version");
    CPPUNIT_ASSERT_EQUAL(versionE, version);

    CPPUNIT_ASSERT_THROW(h5.readAttribute("/", "blah"), std::runtime_error);

    h5.close();
} // testReadAttributeString


// ------------------------------------------------------------------------------------------------
// Test readAttributeStringArray().
void
geomodelgrids::serial::TestHDF5::testReadAttributeStringArray(void) {
    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    const size_t numKeywords = 3;
    const char* keywordsE[numKeywords] = { "key one", "key two", "key three" };
    std::vector<std::string> keywords;
    h5.readAttribute("/", "keywords", &keywords);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of keywords.", numKeywords, keywords.size());
    for (size_t i = 0; i < numKeywords; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in keyword.", std::string(keywordsE[i]), keywords[i]);
    } // for

    CPPUNIT_ASSERT_THROW(h5.readAttribute("/", "blah", &keywords), std::runtime_error);

    h5.close();
} // testReadAttributeStringArray


// ------------------------------------------------------------------------------------------------
// Test readDatasetHyperslab().
void
geomodelgrids::serial::TestHDF5::testReadDatasetHyperslab(void) {
    const char* dataset = "/blocks/top";

    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    double dx = 0.0;
    double dy = 0.0;
    double dz = 0.0;
    double z_top = 0.0;
    h5.readAttribute(dataset, "x_resolution", H5T_NATIVE_DOUBLE, &dx);
    h5.readAttribute(dataset, "y_resolution", H5T_NATIVE_DOUBLE, &dy);
    h5.readAttribute(dataset, "z_resolution", H5T_NATIVE_DOUBLE, &dz);
    h5.readAttribute(dataset, "z_top", H5T_NATIVE_DOUBLE, &z_top);

    const int ndims = 4;
    hsize_t origin[ndims] = { 3, 3, 1, 0 };
    hsize_t dims[ndims] = { 2, 3, 1, 2 };
    const int nvalues = 2*3*1*2;
    double values[nvalues];
    h5.readDatasetHyperslab((void*)values, dataset, origin, dims, ndims, H5T_NATIVE_DOUBLE);

    const double tolerance = 1.0e-6;
    for (hsize_t ix = 0, i = 0; ix < dims[0]; ++ix) {
        const double x = dx * (origin[0] + ix);
        for (hsize_t iy = 0; iy < dims[1]; ++iy) {
            const double y = dy * (origin[1] + iy);
            for (hsize_t iz = 0; iz < dims[2]; ++iz) {
                const double z = z_top - dz * (origin[2] + iz);

                { // Value 0
                    std::ostringstream msg;
                    msg << "Mismatch at hyperslab index ("<<ix<<","<<iy<<","<<iz<<",0).";
                    const double valueE = geomodelgrids::testdata::ModelPoints::computeValueOne(x, y, z);
                    const double toleranceV = std::max(tolerance, tolerance*fabs(valueE));
                    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[i++], toleranceV);
                } // Value 0
                { // Value 1
                    std::ostringstream msg;
                    msg << "Mismatch at hyperslab index ("<<ix<<","<<iy<<","<<iz<<",1).";
                    const double valueE = geomodelgrids::testdata::ModelPoints::computeValueTwo(x, y, z);
                    const double toleranceV = std::max(tolerance, tolerance*fabs(valueE));
                    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[i++], toleranceV);
                } // Value 1
            } // for
        } // for
    } // for

    // Bad number of dimensions
    CPPUNIT_ASSERT_THROW(h5.readDatasetHyperslab((void*)values, dataset, origin, dims, 1, H5T_NATIVE_DOUBLE),
                         std::runtime_error);

    // Bad dimensions
    origin[ndims-1] = 99999;
    CPPUNIT_ASSERT_THROW(h5.readDatasetHyperslab((void*)values, dataset, origin, dims, ndims, H5T_NATIVE_DOUBLE),
                         std::runtime_error);

    h5.close();
} // testReadDatasetHyperslab


// End of file
