/**
 * C++ unit testing of geomodelgrids::serial::Model.
 */

#include <portinfo>

#include "geomodelgrids/serial/Model.hh" // USES Model
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Topography.hh" // USES Topography
#include "geomodelgrids/serial/Block.hh" // USES Block

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace serial {
        class TestModel;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestModel : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestModel);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testLoadMetadata);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testToModelXYZFlat);
    CPPUNIT_TEST(testToModelXYZTopo);
    CPPUNIT_TEST(testContains);
    CPPUNIT_TEST(testQueryElevation);
    CPPUNIT_TEST(testQuery);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test getters.
    void testAccessors(void);

    /// Test loadMetadata().
    void testLoadMetadata(void);

    /// Test initialize().
    void testInitialize(void);

    /// Test _toModelXYZ() with flat ground surface.
    void testToModelXYZFlat(void);

    /// Test _toModelXYZ() with topography.
    void testToModelXYZTopo(void);

    /// Test contains().
    void testContains(void);

    /// Test queryElevation().
    void testQueryElevation(void);

    /// Test query().
    void testQuery(void);

}; // class TestModel
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestModel);

// ----------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestModel::testConstructor(void) {
    Model model;

    CPPUNIT_ASSERT_MESSAGE("Checking value names", model._valueNames.empty());
    CPPUNIT_ASSERT_MESSAGE("Checking value units", model._valueUnits.empty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking CRS string", std::string(""), model._modelCRSString);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking origin x", 0.0, model._origin[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking origin y", 0.0, model._origin[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking y azimuth", 0.0, model._yazimuth);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[0]", 0.0, model._dims[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[1]", 0.0, model._dims[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking dims[2]", 0.0, model._dims[2]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking h5", (HDF5*)NULL, model._h5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking info", (ModelInfo*)NULL, model._info);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking topography", (Topography*)NULL, model._topography);
    // CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking CRS", (HDF5*)NULL, model._h5);
    CPPUNIT_ASSERT_MESSAGE("Checking blocks", model._blocks.empty());
} // testConstructor


// ----------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestModel::testAccessors(void) {
    Model model;

    const size_t numValues = 3;
    const char* namesPtr[numValues] = {"one", "two", "three"};
    const char* unitsPtr[numValues] = {"m", "m/s", "MPa"};
    const std::vector<std::string> valueNames(namesPtr, namesPtr+numValues);model._valueNames = valueNames;
    const std::vector<std::string> valueUnits(unitsPtr, unitsPtr+numValues);model._valueUnits = valueUnits;
    const std::string modelCRSString("CRSTransformer String");model._modelCRSString = modelCRSString;
    const double origin[2] = { 10.0, 20.0 };
    model._origin[0] = origin[0];
    model._origin[1] = origin[1];
    const double yazimuth(34.0);model._yazimuth = yazimuth;
    const double dims[3] = { 100.0, 200.0, 50.0 };
    model._dims[0] = dims[0];
    model._dims[1] = dims[1];
    model._dims[2] = dims[2];

    ModelInfo* info = new ModelInfo();model._info = info;
    Topography* topography = new Topography();model._topography = topography;

    const size_t numBlocks(3);
    Block* blocksPtr[numBlocks] = {
        new Block("one"),
        new Block("three"),
        new Block("five"),
    };
    const std::vector<Block*> blocks(blocksPtr, blocksPtr+numBlocks);model._blocks = blocks;

    const double tolerance = 1.0e-6;

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

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking info", const_cast<const ModelInfo*>(info), model.getInfo());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking topography", const_cast<const Topography*>(topography), model.getTopography());

    const std::vector<Block*>& blocksT = model.getBlocks();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking blocks size", blocks.size(), blocksT.size());
    for (size_t i = 0; i < blocks.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking blocks", blocks[i], blocksT[i]);
    } // for
} // testAccessors


// ---------------------------------------------------------------------------------------------------------------------
// Test loadMetadata().
void
geomodelgrids::serial::TestModel::testLoadMetadata(void) {
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

    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
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


// ---------------------------------------------------------------------------------------------------------------------
// Test initialize().
void
geomodelgrids::serial::TestModel::testInitialize(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    CPPUNIT_ASSERT_MESSAGE("CRS transformer not created.", model._crsTransformer);

    Topography* const topography = model._topography;
    if (topography) {
        CPPUNIT_ASSERT_NO_THROW(topography->query(0.0, 0.0));
    } // if

    model.close();
} // testInitialize


// ----------------------------------------------------------------------
// Test _toModelXYZ() with flat ground surface.
void
geomodelgrids::serial::TestModel::testToModelXYZFlat(void) {
    Model model;
    model.open("../../data/three-blocks-flat.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    const std::string inputCRS("EPSG:4326"); // WGS84
    const size_t numPoints = 5;
    const size_t spaceDim = 3;
    const double lle[numPoints*spaceDim] = {
        34.0, -116.5, 10.0,
        34.5, -117.8, 12.0,
        35.5, -118.0, -3.0e+3,
        37.0, -122.0, -45.0e+3,
        38.0, -123.0, -12.5e+3,
    };
    const double xyzE[numPoints*spaceDim] = {
        86658.20218683, -96450.21492572, 45010.0,
        7507.08122684, 9120.43657295, 45012.0,
        44881.53106401, 115249.73721748, 42000.0,
        -182675.75205696, 439051.94016524, 0.0,
        -199841.70552931, 579964.1840423, 32500.0,
    };

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        double xyz[spaceDim];
        model._toModelXYZ(&xyz[0], &xyz[1], &xyz[2], lle[iPt*spaceDim+0], lle[iPt*spaceDim+1], lle[iPt*spaceDim+2]);
        for (size_t iDim = 0; iDim < spaceDim; ++iDim) {
            std::ostringstream msg;
            msg << "Mismatch for point (" << lle[iPt*spaceDim+0] << ", " << lle[iPt*spaceDim+1]
                << ", " << lle[iPt*spaceDim+2] << ") for component " << iDim << ".";
            const double valueE = xyzE[iPt*spaceDim+iDim];
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*valueE);
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, xyz[iDim], valueTolerance);
        } // for
    } // for
} // testToModelXYZFlat


// ----------------------------------------------------------------------
// Test _toModelXYZ() with topography.
void
geomodelgrids::serial::TestModel::testToModelXYZTopo(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    const std::string inputCRS("EPSG:4326"); // WGS84
    const size_t numPoints = 5;
    const size_t spaceDim = 3;
    const double lle[numPoints*spaceDim] = {
        34.7, -117.8, 10.0,
        34.5, -117.8, 12.0,
        34.6, -117.5, -3.0e+3,
        35.0, -117.5, -45.0e+3,
        34.7, -118.0, -12.5e+3,
    };
    double xyzE[numPoints*spaceDim] = {
        18157.12318227833, 28596.959586967772, 28176.78110685589,
        7507.081226836308, 9120.43657294684, 41200.69283177594,
        36985.200422215414, 5700.435626422162, 30231.010879699552,
        58165.78933216298, 44727.815689240764, 0.0,
        2098.6043330640623, 37400.00293473315, 31964.34087669759,
    };

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        double xyz[spaceDim];
        model._toModelXYZ(&xyz[0], &xyz[1], &xyz[2], lle[iPt*spaceDim+0], lle[iPt*spaceDim+1], lle[iPt*spaceDim+2]);

        for (size_t iDim = 0; iDim < spaceDim; ++iDim) {
            std::ostringstream msg;
            msg << "Mismatch for point (" << lle[iPt*spaceDim+0] << ", " << lle[iPt*spaceDim+1]
                << ", " << lle[iPt*spaceDim+2] << ") for component " << iDim << ".";
            const double valueE = xyzE[iPt*spaceDim+iDim];
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*valueE);
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, xyz[iDim], valueTolerance);
        } // for
    } // for
} // testToModelXYZTopo


// ----------------------------------------------------------------------
// Test contains().
void
geomodelgrids::serial::TestModel::testContains(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    const std::string inputCRS("EPSG:4326"); // WGS84
    const size_t numPoints = 10;
    const size_t spaceDim = 3;
    const double lle[numPoints*spaceDim] = {
        34.7, -117.8, 10.0,
        34.7, -117.8, 9.9e+5,

        35.0, -117.6, -45.0e+3,
        35.0, -117.6, -45.1e+3,

        35.1, -117.8, -3.0e+3,
        34.3, -117.8, -3.0e+3,

        35.0, -117.5, -3.0e+3,
        35.0, -113.0, -3.0e+3,

        35.0, -118.2, -3.0e+3,
        42.0, -117.8, -3.0e+3,
    };
    const bool containsE[numPoints] = {
        true, false,
        true, false,
        true, false,
        true, false,
        true, false,
    };

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const bool flag = model.contains(lle[iPt*spaceDim+0], lle[iPt*spaceDim+1], lle[iPt*spaceDim+2]);
        std::ostringstream msg;
        msg << "Mismatch for point (" << lle[iPt*spaceDim+0] << ", " << lle[iPt*spaceDim+1]
            << ", " << lle[iPt*spaceDim+2] << ").";
        CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str().c_str(), containsE[iPt], flag);
    } // for
} // testContains


// ----------------------------------------------------------------------
// Test queryElevation().
void
geomodelgrids::serial::TestModel::testQueryElevation(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    const std::string inputCRS("EPSG:4326"); // WGS84
    const size_t numPoints = 5;
    const size_t spaceDim = 2;
    const double latlon[numPoints*spaceDim] = {
        34.7, -117.8,
        34.5, -117.8,
        34.6, -117.5,
        35.0, -117.5,
        34.7, -118.0,
    };
    double elevationE[numPoints] = {
        26883.65457072,
        4162.76549694,
        17518.58422866,
        137391.81153092,
        754.11098391,
    };

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double elevation = model.queryElevation(latlon[iPt*spaceDim+0], latlon[iPt*spaceDim+1]);

        std::ostringstream msg;
        msg << "Mismatch for point (" << latlon[iPt*spaceDim+0] << ", " << latlon[iPt*spaceDim+1] << ").";
        const double tolerance = 1.0e-6;
        const double valueTolerance = std::max(tolerance, tolerance*elevationE[iPt]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE[iPt], elevation, valueTolerance);
    } // for
} // testQueryElevation


// ----------------------------------------------------------------------
// Test query().
void
geomodelgrids::serial::TestModel::testQuery(void) {
    CPPUNIT_ASSERT_MESSAGE(":TODO: @brad Implement testQuery().", false);
} // testQuery


// End of file
