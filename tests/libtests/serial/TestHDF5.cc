/**
 * C++ unit testing of geomodelgrids::serial::HDF5.
 */

#include <portinfo>

#include "tests/data/ModelPoints.hh" // USES ModelPoints

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace serial {
        class TestHDF5;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestHDF5 {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Constructor.
    TestHDF5(void);

    /// Destructor.
    ~TestHDF5(void);

    /// Test constructor.
    static
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

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestHDF5::testConstructor", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5::testConstructor();
}

TEST_CASE("TestHDF5::testOpenClose", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5().testOpenClose();
}
TEST_CASE("TestHDF5::testAccessors", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5().testAccessors();
}
TEST_CASE("TestHDF5::testGetDatasetDims", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5().testGetDatasetDims();
}
TEST_CASE("TestHDF5::testGetGroupDatasets", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5().testGetGroupDatasets();
}
TEST_CASE("TestHDF5::testHasAttribute", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5().testHasAttribute();
}
TEST_CASE("TestHDF5::testReadAttribute", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5().testReadAttribute();
}
TEST_CASE("TestHDF5::testReadAttributeString", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5().testReadAttributeString();
}
TEST_CASE("TestHDF5::testReadAttributeStringArray", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5().testReadAttributeStringArray();
}
TEST_CASE("TestHDF5::testReadDatasetHyperslab", "[TestHDF5]") {
    geomodelgrids::serial::TestHDF5().testReadDatasetHyperslab();
}

// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::TestHDF5::TestHDF5(void) {
    // Temporarily turn off HDF5 error handler.
    H5Eget_auto(H5E_DEFAULT, &_errFunc, &_errData);
    H5Eset_auto(H5E_DEFAULT, NULL, NULL);
} // constructor


// ------------------------------------------------------------------------------------------------
// Destructor.
geomodelgrids::serial::TestHDF5::~TestHDF5(void) {
    // Restore default HDF5 error handler.
    H5Eset_auto(H5E_DEFAULT, _errFunc, _errData);
} // destructor


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestHDF5::testConstructor(void) {
    HDF5 h5;

    CHECK(hid_t(-1) == h5._file);

    // Check default cache parameters.
    CHECK(size_t(128*1048576) == h5._cacheSize);
    CHECK(size_t(63997) == h5._cacheNumSlots);
    CHECK(0.75 == h5._cachePreemption);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test open(), isOpen(), close().
void
geomodelgrids::serial::TestHDF5::testOpenClose(void) {
    HDF5 h5;

    REQUIRE(!h5.isOpen());

    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDONLY);REQUIRE(h5.isOpen());
    h5.close();REQUIRE(!h5.isOpen());

    const size_t cacheSize = 1048576;
    const size_t cacheNumSlots = 37;
    const double cachePreemption = 0.6;
    h5.setCache(cacheSize, cacheNumSlots, cachePreemption);

    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDWR);REQUIRE(h5.isOpen());
    h5.close();REQUIRE(!h5.isOpen());
    CHECK(cacheSize == h5._cacheSize);
    CHECK(cacheNumSlots == h5._cacheNumSlots);
    CHECK(cachePreemption == h5._cachePreemption);

    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDWR);REQUIRE(h5.isOpen());
    CHECK_THROWS_AS(h5.open("../../data/one-block-flat.h5", H5F_ACC_RDONLY), std::runtime_error); // already open
    h5.close();REQUIRE(!h5.isOpen());

    CHECK_THROWS_AS(h5.open("abc", H5F_ACC_RDONLY), std::runtime_error); // file doesn't exist
} // testOpenClose


// ------------------------------------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestHDF5::testAccessors(void) {
    HDF5 h5;

    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    CHECK(h5.hasGroup("blocks"));
    CHECK(!h5.hasGroup("abc"));

    CHECK(h5.hasDataset("/blocks/bottom"));
    CHECK(!h5.hasDataset("/topography"));

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
    REQUIRE(ndimsE == ndims);
    for (int i = 0; i < ndimsE; ++i) {
        CHECK(dimsE[i] == dims[i]);
    } // for
    delete[] dims;dims = NULL;

    CHECK_THROWS_AS(h5.getDatasetDims(&dims, &ndims, "blah"), std::runtime_error);

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
    REQUIRE(numDatasets == datasetNames.size());
    for (size_t i = 0; i < numDatasets; ++i) {
        bool found = false;
        for (size_t j = 0; j < numDatasets; ++j) {
            if (std::string(datasetNamesE[i]) == datasetNames[j]) {
                found = true;
                break;
            } // if
        } // for
        INFO("Searching for dataset " << datasetNamesE[i] << ".");
        CHECK(found);
    } // for

    CHECK_THROWS_AS(h5.getGroupDatasets(&datasetNames, "blah"), std::runtime_error);

    h5.close();
} // testGetGroupDatasets


// ------------------------------------------------------------------------------------------------
// Test hasAttribute.
void
geomodelgrids::serial::TestHDF5::testHasAttribute(void) {
    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    bool found = h5.hasAttribute("/blocks/bottom", "x_resolution");
    CHECK(found);

    found = h5.hasAttribute("/blocks/bottom", "abcdf");
    CHECK(!found);

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
    CHECK_THAT(resolution, Catch::Matchers::WithinAbs(resolutionE, tolerance*resolutionE));

    CHECK_THROWS_AS(h5.readAttribute("/blocks/bottom", "blah", H5T_NATIVE_DOUBLE, (void*)&resolution),
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
    CHECK(versionE == version);

    CHECK_THROWS_AS(h5.readAttribute("/", "blah"), std::runtime_error);

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
    REQUIRE(numKeywords == keywords.size());
    for (size_t i = 0; i < numKeywords; ++i) {
        CHECK(std::string(keywordsE[i]) == keywords[i]);
    } // for

    CHECK_THROWS_AS(h5.readAttribute("/", "blah", &keywords), std::runtime_error);

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
                    INFO("Checking hyperslab index ("<<ix<<","<<iy<<","<<iz<<",0).");
                    const double valueE = geomodelgrids::testdata::ModelPoints::computeValueOne(x, y, z);
                    const double toleranceV = std::max(tolerance, tolerance*fabs(valueE));
                    CHECK_THAT(values[i++], Catch::Matchers::WithinAbs(valueE, toleranceV));
                } // Value 0
                { // Value 1
                    INFO("Checking hyperslab index ("<<ix<<","<<iy<<","<<iz<<",1).");
                    const double valueE = geomodelgrids::testdata::ModelPoints::computeValueTwo(x, y, z);
                    const double toleranceV = std::max(tolerance, tolerance*fabs(valueE));
                    CHECK_THAT(values[i++], Catch::Matchers::WithinAbs(valueE, toleranceV));
                } // Value 1
            } // for
        } // for
    } // for

    // Bad number of dimensions
    CHECK_THROWS_AS(h5.readDatasetHyperslab((void*)values, dataset, origin, dims, 1, H5T_NATIVE_DOUBLE),
                    std::runtime_error);

    // Bad dimensions
    origin[ndims-1] = 99999;
    CHECK_THROWS_AS(h5.readDatasetHyperslab((void*)values, dataset, origin, dims, ndims, H5T_NATIVE_DOUBLE),
                    std::runtime_error);

    h5.close();
} // testReadDatasetHyperslab


// End of file
