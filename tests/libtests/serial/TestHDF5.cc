/**
 * C++ unit testing of geomodelgrids::serial::HDF5.
 */

#include <portinfo>

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace serial {
        class TestHDF5;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestHDF5 : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestHDF5);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testOpenClose);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testGetDatasetDims);
    CPPUNIT_TEST(testGetGroupDatasets);
    CPPUNIT_TEST(testReadAttribute);
    CPPUNIT_TEST(testReadAttributeString);
    CPPUNIT_TEST(testReadAttributeStringArray);
    CPPUNIT_TEST(testReadDatasetChunk);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

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

    /// Test readAttribute.
    void testReadAttribute(void);

    /// Test readAttributeString().
    void testReadAttributeString(void);

    /// Test readAttributeStringArray().
    void testReadAttributeStringArray(void);

    /// Test readDatasetChunk().
    void testReadDatasetChunk(void);

}; // class TestHDF5
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestHDF5);

// ---------------------------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestHDF5::testConstructor(void) {
    HDF5 h5;

    CPPUNIT_ASSERT_EQUAL(hid_t(-1), h5._file);
} // testConstructor


// ---------------------------------------------------------------------------------------------------------------------
// Test open(), isOpen(), close().
void
geomodelgrids::serial::TestHDF5::testOpenClose(void) {
    HDF5 h5;

    CPPUNIT_ASSERT(!h5.isOpen());

    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDONLY);CPPUNIT_ASSERT(h5.isOpen());
    h5.close();CPPUNIT_ASSERT(!h5.isOpen());

    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDWR);CPPUNIT_ASSERT(h5.isOpen());
    h5.close();CPPUNIT_ASSERT(!h5.isOpen());

    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDWR);CPPUNIT_ASSERT(h5.isOpen());
    CPPUNIT_ASSERT_THROW(h5.open("abc", H5F_ACC_RDONLY), std::runtime_error);
    h5.close();CPPUNIT_ASSERT(!h5.isOpen());
} // testOpenClose


// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
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

    h5.close();
} // testGetDatasetDims


// ---------------------------------------------------------------------------------------------------------------------
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

    h5.close();
} // testGetGroupDatasets


// ---------------------------------------------------------------------------------------------------------------------
// Test readAttribute.
void
geomodelgrids::serial::TestHDF5::testReadAttribute(void) {
    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    const double resolutionE = 30e+3;
    double resolution = 0.0;
    const double tolerance = 1.0e-6;
    h5.readAttribute("/blocks/bottom", "resolution_horiz", H5T_NATIVE_DOUBLE, (void*)&resolution);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(resolutionE, resolution, tolerance*resolutionE);

    h5.close();
} // testReadAttribute


// ---------------------------------------------------------------------------------------------------------------------
// Test readAttributeString().
void
geomodelgrids::serial::TestHDF5::testReadAttributeString(void) {
    HDF5 h5;
    h5.open("../../data/three-blocks-flat.h5", H5F_ACC_RDONLY);

    const std::string versionE = "1.0.0";
    const std::string version = h5.readAttribute("/", "version");
    CPPUNIT_ASSERT_EQUAL(versionE, version);

    h5.close();
} // testReadAttributeString


// ---------------------------------------------------------------------------------------------------------------------
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

    h5.close();
} // testReadAttributeStringArray


// ---------------------------------------------------------------------------------------------------------------------
// Test readDatasetChunk().
void
geomodelgrids::serial::TestHDF5::testReadDatasetChunk(void) {
    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement test.", false);
} // testReadDatasetChunk


#if 0
// ---------------------------------------------------------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestHDF5::testLoadMetadata(void) {
    const std::string title("Three Blocks Topo");
    const std::string id("three-blocks-topo");
    const std::string doi("this.is.a.doi");
    const size_t numValues = 2;
    const char* namesPtr[numValues] = {"one", "two" };
    const char* unitsPtr[numValues] = {"m", "m/s"};
    const std::vector<std::string> valueNames(namesPtr, namesPtr+numValues);
    const std::vector<std::string> valueUnits(unitsPtr, unitsPtr+numValues);
    const std::string modelCRSString("EPSG:3311");
    const double origin[2] = { 200000.0, -400000.0 };
    const double yazimuth(330.0);
    const double dims[3] = { 60.0e+3, 120.0e+3, 45.0e+3 };
    const double topoHorizRes = 5.0e+3;

    const size_t numBlocks = 3;
    const char* blockNamesPtr[numBlocks] = {"top", "middle", "bottom"};
    const double blockZTop[numBlocks] = {0.0e+3, -5.0e+3, -25.0e+3 };
    const std::vector<std::string> blockNames(blockNamesPtr, blockNamesPtr+numBlocks);

    const double tolerance = 1.0e-6;

    HDF5 model;
    model.open("../../data/three-blocks-topo.h5", HDF5::READ);
    model.loadMetadata();

    const std::vector<std::string>& valueNamesT = model.getValueNames();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking names of values size", valueNames.size(), valueNamesT.size());
    for (size_t i = 0; i < valueNames.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking names of values", valueNames[i], valueNamesT[i]);
    } // for

    const std::vector<std::string>& valueUnitsT = model.getValueUnits();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking names of values size", valueUnits.size(), valueUnitsT.size());
    for (size_t i = 0; i < valueUnits.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking names of values", valueUnits[i], valueUnitsT[i]);
    } // for

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking CRS string", modelCRSString, model.getCRSString());

    const double* originT = model.getOrigin();
    CPPUNIT_ASSERT_MESSAGE("Checking origin pointer", originT);
    for (size_t i = 0; i < 2; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking origin", origin[i], originT[i]);
    } // for

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Checking yazimuth", yazimuth, model.getYAzimuth(), tolerance);

    const double* dimsT = model.getDims();
    CPPUNIT_ASSERT_MESSAGE("Checking dims pointer", dimsT);
    for (size_t i = 0; i < 2; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Checking dims", dims[i], dimsT[i], tolerance);
    } // for

    const ModelInfo* info = model.getInfo();
    CPPUNIT_ASSERT_MESSAGE("Checking model info pointer", info);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking model title", title, info->getTitle());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking model id", id, info->getId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking model doi", doi, info->getDOI());

    const Topography* topography = model.getTopography();
    CPPUNIT_ASSERT_MESSAGE("Checking topography pointer", topography);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Checking horizontal resolution of topography",
                                         topoHorizRes, topography->getResolutionHoriz(), tolerance);

    const std::vector<Block*>& blocksT = model.getBlocks();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking blocks size", numBlocks, blocksT.size());
    for (size_t i = 0; i < numBlocks; ++i) {
        CPPUNIT_ASSERT_MESSAGE("Checking block pointer", blocksT[i]);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking block names", blockNames[i], blocksT[i]->getName());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking block z_top", blockZTop[i], blocksT[i]->getZTop());

    } // for

    model.close();
} // testLoadMetadata


#endif

// End of file
