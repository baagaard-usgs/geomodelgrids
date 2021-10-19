/**
 * C++ unit testing of geomodelgrids::serial::Model.
 */

#include <portinfo>

#include "ModelPoints.hh" // USES ModelPoints

#include "geomodelgrids/serial/Model.hh" // USES Model
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Surface.hh" // USES Surface
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/utils/constants.hh" // USES TOLERANCE

#include <cppunit/extensions/HelperMacros.h>

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace serial {
        class TestModel;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestModel : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE -------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE(TestModel);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testOpenClose);
    CPPUNIT_TEST(testLoadMetadata);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testToModelXYZFlat);
    CPPUNIT_TEST(testToModelXYZTopo);
    CPPUNIT_TEST(testContains);
    CPPUNIT_TEST(testQueryTopElevation);
    CPPUNIT_TEST(testQueryTopoBathyElevation);
    CPPUNIT_TEST(testQuery);
    CPPUNIT_TEST(testQueryVarXYZ);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test getters.
    void testAccessors(void);

    /// Test open() and close().
    void testOpenClose(void);

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

    /// Test queryTopElevation().
    void testQueryTopElevation(void);

    /// Test queryTopoBathyElevation().
    void testQueryTopoBathyElevation(void);

    /// Test query().
    void testQuery(void);

    /// Test query() with variable resolution blocks.
    void testQueryVarXYZ(void);

}; // class TestModel
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestModel);

// ------------------------------------------------------------------------------------------------
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
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking top surface", (Surface*)NULL, model._surfaceTop);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking topography/bathymetry", (Surface*)NULL, model._surfaceTopoBathy);
    // CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking CRS", (HDF5*)NULL, model._h5);
    CPPUNIT_ASSERT_MESSAGE("Checking blocks", model._blocks.empty());
} // testConstructor


// ------------------------------------------------------------------------------------------------
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
    Surface* surfaceTop = new Surface("top_surface");model._surfaceTop = surfaceTop;
    Surface* surfaceTopoBathy = new Surface("topography_bathymetry");model._surfaceTopoBathy = surfaceTopoBathy;

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
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking top surface", const_cast<const Surface*>(surfaceTop), model.getTopSurface());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking topography/bathymetry", const_cast<const Surface*>(surfaceTopoBathy), model.getTopoBathy());

    const std::vector<Block*>& blocksT = model.getBlocks();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking blocks size", blocks.size(), blocksT.size());
    for (size_t i = 0; i < blocks.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking blocks", blocks[i], blocksT[i]);
    } // for

    Block* block = model._findBlock(0, 0, 0);
    CPPUNIT_ASSERT_MESSAGE("Failed to find block.", block);

    block = model._findBlock(0, 0, 1.0e+20);
    CPPUNIT_ASSERT_MESSAGE("Found block for invalid point.", !block);
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test open() and close().
void
geomodelgrids::serial::TestModel::testOpenClose(void) {
    Model model;

    model.open("../../data/three-blocks-topo.h5", Model::READ);
    CPPUNIT_ASSERT_MESSAGE("Open with read access failed.", model._h5);
    model.close();

    model.open("../../data/three-blocks-topo.h5", Model::READ_WRITE);
    CPPUNIT_ASSERT_MESSAGE("Open with read/write access failed.", model._h5);
    model.close();

    model.open("../../data/tmp.h5", Model::READ_WRITE_TRUNCATE);
    CPPUNIT_ASSERT_MESSAGE("Open with read/write access and truncate failed.", model._h5);
    model.close();
} // testOpenClose


// ------------------------------------------------------------------------------------------------
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
    const double topoResX = 5.0e+3;
    const double topoResY = 5.0e+3;

    const size_t numBlocks = 3;
    const char* blockNamesPtr[numBlocks] = {"top", "middle", "bottom"};
    const double blockZTop[numBlocks] = {0.0e+3, -5.0e+3, -25.0e+3 };
    const std::vector<std::string> blockNames(blockNamesPtr, blockNamesPtr+numBlocks);

    const double tolerance = 1.0e-6;

    Model model;
    CPPUNIT_ASSERT_THROW(model.loadMetadata(), std::logic_error); // Model not open

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
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking model repositoryDOI", doi, info->getRepositoryDOI());

    const Surface* surfaceTop = model.getTopSurface();
    CPPUNIT_ASSERT_MESSAGE("Checking top surface pointer", surfaceTop);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Checking x resolution of top surface",
                                         topoResX, surfaceTop->getResolutionX(), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Checking y resolution of top surface",
                                         topoResY, surfaceTop->getResolutionY(), tolerance);

    const Surface* surfaceTopoBathy = model.getTopoBathy();
    CPPUNIT_ASSERT_MESSAGE("Checking topography/bathymetry pointer", surfaceTopoBathy);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Checking x resolution of topography/bathymetry",
                                         topoResX, surfaceTopoBathy->getResolutionX(), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Checking y resolution of topography/bathymetry",
                                         topoResY, surfaceTopoBathy->getResolutionY(), tolerance);

    const std::vector<Block*>& blocksT = model.getBlocks();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking blocks size", numBlocks, blocksT.size());
    for (size_t i = 0; i < numBlocks; ++i) {
        CPPUNIT_ASSERT_MESSAGE("Checking block pointer", blocksT[i]);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking block names", blockNames[i], blocksT[i]->getName());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking block z_top", blockZTop[i], blocksT[i]->getZTop());

    } // for
    CPPUNIT_ASSERT_NO_THROW(model.loadMetadata());

    model.close();
} // testLoadMetadata


// ------------------------------------------------------------------------------------------------
// Test initialize().
void
geomodelgrids::serial::TestModel::testInitialize(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    CPPUNIT_ASSERT_MESSAGE("CRS transformer not created.", model._crsTransformer);

    Surface* const surfaceTop = model._surfaceTop;
    if (surfaceTop) {
        CPPUNIT_ASSERT_NO_THROW(surfaceTop->query(0.0, 0.0));
    } // if

    Surface* const surfaceTopoBathy = model._surfaceTopoBathy;
    if (surfaceTopoBathy) {
        CPPUNIT_ASSERT_NO_THROW(surfaceTopoBathy->query(0.0, 0.0));
    } // if

    model.close();
} // testInitialize


// ------------------------------------------------------------------------------------------------
// Test _toModelXYZ() with flat ground surface.
void
geomodelgrids::serial::TestModel::testToModelXYZFlat(void) {
    Model model;
    model.open("../../data/three-blocks-flat.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    geomodelgrids::testdata::ThreeBlocksFlatPoints points;
    const size_t numPoints = points.getNumPoints();
    const size_t spaceDim = 3;
    const double* pointsLLE = points.getLatLonElev();
    const double* pointsXYZ = points.getXYZ();

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        double xyz[spaceDim] = { 0.0, 0.0, 0.0 };
        model._toModelXYZ(&xyz[0], &xyz[1], &xyz[2],
                          pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
        for (size_t iDim = 0; iDim < spaceDim; ++iDim) {
            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ") for component " << iDim << ".";
            const double valueE = pointsXYZ[iPt*spaceDim+iDim];
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, xyz[iDim], valueTolerance);
        } // for
    } // for
} // testToModelXYZFlat


// ------------------------------------------------------------------------------------------------
// Test _toModelXYZ() with topography.
void
geomodelgrids::serial::TestModel::testToModelXYZTopo(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    geomodelgrids::testdata::ThreeBlocksTopoPoints points;
    const size_t numPoints = points.getNumPoints();
    const size_t spaceDim = 3;
    const double* pointsLLE = points.getLatLonElev();
    const double* pointsXYZ = points.getXYZ();

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        double xyz[spaceDim] = { 0.0, 0.0, 0.0 };
        model._toModelXYZ(&xyz[0], &xyz[1], &xyz[2],
                          pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);

        for (size_t iDim = 0; iDim < spaceDim; ++iDim) {
            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ") for component " << iDim << ".";
            const double valueE = pointsXYZ[iPt*spaceDim+iDim];
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, xyz[iDim], valueTolerance);
        } // for
    } // for
} // testToModelXYZTopo


// ------------------------------------------------------------------------------------------------
// Test contains().
void
geomodelgrids::serial::TestModel::testContains(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    { // inside domain
        geomodelgrids::testdata::ThreeBlocksTopoPoints points;
        const size_t numPoints = points.getNumPoints();
        const size_t spaceDim = 3;
        const double* pointsLLE = points.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const bool flag = model.contains(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ").";
            CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str().c_str(), true, flag);
        } // for

        // Verify points slightly above topography are considered in the domain.
        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const double elev = model.queryTopElevation(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1]) + 0.5*TOLERANCE;
            const bool flag = model.contains(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], elev);
            std::ostringstream msg;
            msg << "Mismatch for surface point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << elev << ").";
            CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str().c_str(), true, flag);
        } // for

    } // inside domain

    { // outside domain
        geomodelgrids::testdata::OutsideDomainPoints points;
        const size_t numPoints = points.getNumPoints();
        const size_t spaceDim = 3;
        const double* pointsLLE = points.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const bool flag = model.contains(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ").";
            CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str().c_str(), false, flag);
        } // for
    } // outside domain
} // testContains


// ------------------------------------------------------------------------------------------------
// Test queryTopElevation().
void
geomodelgrids::serial::TestModel::testQueryTopElevation(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    geomodelgrids::testdata::ThreeBlocksTopoPoints points;
    const size_t numPoints = points.getNumPoints();
    const size_t spaceDim = 3;
    const double* pointsLLE = points.getLatLonElev();
    const double* pointsXYZ = points.getXYZ();

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double elevation = model.queryTopElevation(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1]);
        const double elevationE = points.computeTopElevation(pointsXYZ[iPt*spaceDim+0], pointsXYZ[iPt*spaceDim+1]);

        std::ostringstream msg;
        msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
        const double tolerance = 1.0e-6;
        const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, valueTolerance);
    } // for
} // testQueryElevation


// ------------------------------------------------------------------------------------------------
// Test queryTopoBathyElevation().
void
geomodelgrids::serial::TestModel::testQueryTopoBathyElevation(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    geomodelgrids::testdata::ThreeBlocksTopoPoints points;
    const size_t numPoints = points.getNumPoints();
    const size_t spaceDim = 3;
    const double* pointsLLE = points.getLatLonElev();
    const double* pointsXYZ = points.getXYZ();

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double elevation = model.queryTopoBathyElevation(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1]);
        const double elevationE = points.computeTopoBathyElevation(pointsXYZ[iPt*spaceDim+0], pointsXYZ[iPt*spaceDim+1]);

        std::ostringstream msg;
        msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
        const double tolerance = 1.0e-6;
        const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), elevationE, elevation, valueTolerance);
    } // for
} // testQueryTopoBathyElevation


// ------------------------------------------------------------------------------------------------
// Test query().
void
geomodelgrids::serial::TestModel::testQuery(void) {
    Model model;
    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    geomodelgrids::testdata::ThreeBlocksTopoPoints points;
    const size_t numPoints = points.getNumPoints();
    const size_t spaceDim = 3;
    const double* pointsLLE = points.getLatLonElev();
    const double* pointsXYZ = points.getXYZ();

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double* values = model.query(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);

        const double x = pointsXYZ[iPt*spaceDim+0];
        const double y = pointsXYZ[iPt*spaceDim+1];
        const double z = pointsXYZ[iPt*spaceDim+2];

        const double tolerance = 1.0e-5;
        { // Value 0
            const double valueE = points.computeValueOne(x, y, z);

            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 0.";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[0], valueTolerance);
        } // Value 0

        { // Value 1
            const double valueE = points.computeValueTwo(x, y, z);

            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 1.";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[1], valueTolerance);
        } // Value 1
    } // for
} // testQuery


void testQueryVarXYZ(void);

// ------------------------------------------------------------------------------------------------
// Test query() with variable resolution blocks.
void
geomodelgrids::serial::TestModel::testQueryVarXYZ(void) {
    Model model;
    model.open("../../data/three-blocks-topo-varxyz.h5", Model::READ);
    model.loadMetadata();
    model.initialize();

    geomodelgrids::testdata::ThreeBlocksTopoPoints points;
    const size_t numPoints = points.getNumPoints();
    const size_t spaceDim = 3;
    const double* pointsLLE = points.getLatLonElev();
    const double* pointsXYZ = points.getXYZ();

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double* values = model.query(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);

        const double x = pointsXYZ[iPt*spaceDim+0];
        const double y = pointsXYZ[iPt*spaceDim+1];
        const double z = pointsXYZ[iPt*spaceDim+2];

        const double tolerance = 1.0e-5;
        { // Value 0
            const double valueE = points.computeValueOne(x, y, z);

            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 0.";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[0], valueTolerance);
        } // Value 0

        { // Value 1
            const double valueE = points.computeValueTwo(x, y, z);

            std::ostringstream msg;
            msg << "Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 1.";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, values[1], valueTolerance);
        } // Value 1
    } // for
} // testQueryVarXYZ


// End of file
