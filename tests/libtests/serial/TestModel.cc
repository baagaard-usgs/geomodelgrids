/**
 * C++ unit testing of geomodelgrids::serial::Model.
 */

#include <portinfo>

#include "tests/data/ModelPoints.hh" // USES ModelPoints

#include "geomodelgrids/serial/Model.hh" // USES Model
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Surface.hh" // USES Surface
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/utils/constants.hh" // USES TOLERANCE

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace serial {
        class TestModel;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestModel {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Test constructor.
    static
    void testConstructor(void);

    /// Test getters.
    static
    void testAccessors(void);

    /// Test open() and close().
    static
    void testOpenClose(void);

    /// Test loadMetadata().
    static
    void testLoadMetadata(void);

    /// Test initialize().
    static
    void testInitialize(void);

    /// Test _toModelXYZ() with flat ground surface.
    static
    void testToModelXYZFlat(void);

    /// Test _toModelXYZ() with topography.
    static
    void testToModelXYZTopo(void);

    /// Test contains().
    static
    void testContains(void);

    /// Test queryTopElevation().
    static
    void testQueryTopElevation(void);

    /// Test queryTopoBathyElevation().
    static
    void testQueryTopoBathyElevation(void);

    /// Test query().
    static
    void testQuery(void);

    /// Test query() with variable resolution blocks.
    static
    void testQueryVarXYZ(void);

}; // class TestModel

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestModel::testConstructor", "[TestModel]") {
    geomodelgrids::serial::TestModel::testConstructor();
}
TEST_CASE("TestModel::testAccessors", "[TestModel]") {
    geomodelgrids::serial::TestModel::testAccessors();
}
TEST_CASE("TestModel::testOpenClose", "[TestModel]") {
    geomodelgrids::serial::TestModel::testOpenClose();
}
TEST_CASE("TestModel::testLoadMetadata", "[TestModel]") {
    geomodelgrids::serial::TestModel::testLoadMetadata();
}
TEST_CASE("TestModel::testInitialize", "[TestModel]") {
    geomodelgrids::serial::TestModel::testInitialize();
}
TEST_CASE("TestModel::testToModelXYZFlat", "[TestModel]") {
    geomodelgrids::serial::TestModel::testToModelXYZFlat();
}
TEST_CASE("TestModel::testToModelXYZTopo", "[TestModel]") {
    geomodelgrids::serial::TestModel::testToModelXYZTopo();
}
TEST_CASE("TestModel::testContains", "[TestModel]") {
    geomodelgrids::serial::TestModel::testContains();
}
TEST_CASE("TestModel::testQueryTopElevation", "[TestModel]") {
    geomodelgrids::serial::TestModel::testQueryTopElevation();
}
TEST_CASE("TestModel::testQueryTopoBathyElevation", "[TestModel]") {
    geomodelgrids::serial::TestModel::testQueryTopoBathyElevation();
}
TEST_CASE("TestModel::testQuery", "[TestModel]") {
    geomodelgrids::serial::TestModel::testQuery();
}
TEST_CASE("TestModel::testQueryVarXYZ", "[TestModel]") {
    geomodelgrids::serial::TestModel::testQueryVarXYZ();
}

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestModel::testConstructor(void) {
    Model model;

    CHECK(model._valueNames.empty());
    CHECK(model._valueUnits.empty());
    CHECK(std::string("") == model._modelCRSString);
    CHECK(0.0 == model._origin[0]);
    CHECK(0.0 == model._origin[1]);
    CHECK(0.0 == model._yazimuth);
    CHECK(0.0 == model._dims[0]);
    CHECK(0.0 == model._dims[1]);
    CHECK(0.0 == model._dims[2]);
    CHECK((HDF5*)NULL == model._h5);
    CHECK((ModelInfo*)NULL == model._info);
    CHECK((Surface*)NULL == model._surfaceTop);
    CHECK((Surface*)NULL == model._surfaceTopoBathy);
    // CHECK("Checking CRS", (HDF5*)NULL, model._h5);
    CHECK(model._blocks.empty());
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
    REQUIRE(valueNames.size() == valueNamesT.size());
    for (size_t i = 0; i < valueNames.size(); ++i) {
        CHECK(valueNames[i] == valueNamesT[i]);
    } // for

    const std::vector<std::string>& valueUnitsT = model.getValueUnits();
    REQUIRE(valueUnits.size() == valueUnitsT.size());
    for (size_t i = 0; i < valueUnits.size(); ++i) {
        CHECK(valueUnits[i] == valueUnitsT[i]);
    } // for

    CHECK(modelCRSString == model.getCRSString());

    const double* originT = model.getOrigin();
    CHECK(originT);
    for (size_t i = 0; i < 2; ++i) {
        CHECK(origin[i] == originT[i]);
    } // for

    CHECK_THAT(model.getYAzimuth(), Catch::Matchers::WithinAbs(yazimuth, tolerance));

    const double* dimsT = model.getDims();
    REQUIRE(dimsT);
    for (size_t i = 0; i < 2; ++i) {
        CHECK_THAT(dimsT[i], Catch::Matchers::WithinAbs(dims[i], tolerance));
    } // for

    CHECK(const_cast<const ModelInfo*>(info) == model.getInfo());
    CHECK(const_cast<const Surface*>(surfaceTop) == model.getTopSurface());
    CHECK(const_cast<const Surface*>(surfaceTopoBathy) == model.getTopoBathy());

    const std::vector<Block*>& blocksT = model.getBlocks();
    REQUIRE(blocks.size() == blocksT.size());
    for (size_t i = 0; i < blocks.size(); ++i) {
        CHECK(blocks[i] == blocksT[i]);
    } // for

    Block* block = model._findBlock(0, 0, 0);
    CHECK(block);

    block = model._findBlock(0, 0, 1.0e+20);
    CHECK(!block);
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test open() and close().
void
geomodelgrids::serial::TestModel::testOpenClose(void) {
    Model model;

    model.open("../../data/three-blocks-topo.h5", Model::READ);
    CHECK(model._h5);
    model.close();

    model.open("../../data/three-blocks-topo.h5", Model::READ_WRITE);
    CHECK(model._h5);
    model.close();

    model.open("../../data/tmp.h5", Model::READ_WRITE_TRUNCATE);
    CHECK(model._h5);
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
    CHECK_THROWS_AS(model.loadMetadata(), std::logic_error); // Model not open

    model.open("../../data/three-blocks-topo.h5", Model::READ);
    model.loadMetadata();

    const std::vector<std::string>& valueNamesT = model.getValueNames();
    REQUIRE(valueNames.size() == valueNamesT.size());
    for (size_t i = 0; i < valueNames.size(); ++i) {
        CHECK(valueNames[i] == valueNamesT[i]);
    } // for

    const std::vector<std::string>& valueUnitsT = model.getValueUnits();
    REQUIRE(valueUnits.size() == valueUnitsT.size());
    for (size_t i = 0; i < valueUnits.size(); ++i) {
        CHECK(valueUnits[i] == valueUnitsT[i]);
    } // for

    CHECK(modelCRSString == model.getCRSString());

    const double* originT = model.getOrigin();
    CHECK(originT);
    for (size_t i = 0; i < 2; ++i) {
        CHECK(origin[i] == originT[i]);
    } // for

    CHECK_THAT(model.getYAzimuth(), Catch::Matchers::WithinAbs(yazimuth, tolerance));

    const double* dimsT = model.getDims();
    REQUIRE(dimsT);
    for (size_t i = 0; i < 2; ++i) {
        CHECK_THAT(dimsT[i], Catch::Matchers::WithinAbs(dims[i], tolerance));
    } // for

    const ModelInfo* info = model.getInfo();
    CHECK(info);
    CHECK(title == info->getTitle());
    CHECK(id == info->getId());
    CHECK(doi == info->getRepositoryDOI());

    const Surface* surfaceTop = model.getTopSurface();
    REQUIRE(surfaceTop);
    CHECK_THAT(surfaceTop->getResolutionX(), Catch::Matchers::WithinAbs(topoResX, tolerance));
    CHECK_THAT(surfaceTop->getResolutionY(), Catch::Matchers::WithinAbs(topoResY, tolerance));

    const Surface* surfaceTopoBathy = model.getTopoBathy();
    REQUIRE(surfaceTopoBathy);
    CHECK_THAT(surfaceTopoBathy->getResolutionX(), Catch::Matchers::WithinAbs(topoResX, tolerance));
    CHECK_THAT(surfaceTopoBathy->getResolutionY(), Catch::Matchers::WithinAbs(topoResY, tolerance));

    const std::vector<Block*>& blocksT = model.getBlocks();
    REQUIRE(numBlocks == blocksT.size());
    for (size_t i = 0; i < numBlocks; ++i) {
        REQUIRE(blocksT[i]);
        CHECK(blockNames[i] == blocksT[i]->getName());
        CHECK(blockZTop[i] == blocksT[i]->getZTop());

    } // for
    CHECK_NOTHROW(model.loadMetadata());

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

    CHECK(model._crsTransformer);

    Surface* const surfaceTop = model._surfaceTop;
    if (surfaceTop) {
        CHECK_NOTHROW(surfaceTop->query(0.0, 0.0));
    } // if

    Surface* const surfaceTopoBathy = model._surfaceTopoBathy;
    if (surfaceTopoBathy) {
        CHECK_NOTHROW(surfaceTopoBathy->query(0.0, 0.0));
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
            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ") for component " << iDim << ".");
            const double valueE = pointsXYZ[iPt*spaceDim+iDim];
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CHECK_THAT(xyz[iDim], Catch::Matchers::WithinAbs(valueE, valueTolerance));
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
            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ") for component " << iDim << ".");
            const double valueE = pointsXYZ[iPt*spaceDim+iDim];
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CHECK_THAT(xyz[iDim], Catch::Matchers::WithinAbs(valueE, valueTolerance));
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
            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ").");
            CHECK(flag);
        } // for

        // Verify points slightly above topography are considered in the domain.
        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const double elev = model.queryTopElevation(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1]) + 0.5*TOLERANCE;
            const bool flag = model.contains(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], elev);
            INFO("Mismatch for surface point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                                << ", " << elev << ").");
            CHECK(flag);
        } // for

    } // inside domain

    { // outside domain
        geomodelgrids::testdata::OutsideDomainPoints points;
        const size_t numPoints = points.getNumPoints();
        const size_t spaceDim = 3;
        const double* pointsLLE = points.getLatLonElev();

        for (size_t iPt = 0; iPt < numPoints; ++iPt) {
            const bool flag = model.contains(pointsLLE[iPt*spaceDim+0], pointsLLE[iPt*spaceDim+1], pointsLLE[iPt*spaceDim+2]);
            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ").");
            CHECK(false == flag);
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

        INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
        const double tolerance = 1.0e-6;
        const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
        CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, valueTolerance));
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

        INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
        const double tolerance = 1.0e-6;
        const double valueTolerance = std::max(tolerance, tolerance*fabs(elevationE));
        CHECK_THAT(elevation, Catch::Matchers::WithinAbs(elevationE, valueTolerance));
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

            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 0.");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CHECK_THAT(values[0], Catch::Matchers::WithinAbs(valueE, valueTolerance));
        } // Value 0

        { // Value 1
            const double valueE = points.computeValueTwo(x, y, z);

            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 1.");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CHECK_THAT(values[1], Catch::Matchers::WithinAbs(valueE, valueTolerance));
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

            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 0.");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CHECK_THAT(values[0], Catch::Matchers::WithinAbs(valueE, valueTolerance));
        } // Value 0

        { // Value 1
            const double valueE = points.computeValueTwo(x, y, z);

            INFO("Mismatch for point (" << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1]
                                        << ", " << pointsLLE[iPt*spaceDim+2] << ") for value 1.");
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CHECK_THAT(values[1], Catch::Matchers::WithinAbs(valueE, valueTolerance));
        } // Value 1
    } // for
} // testQueryVarXYZ


// End of file
