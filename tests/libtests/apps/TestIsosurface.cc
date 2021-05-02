/**
 * C++ unit testing of geomodelgrids::apps::Isosurface.
 */

#include <portinfo>

#include "geomodelgrids/apps/Isosurface.hh" // USES Isosurface
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE
#include "geomodelgrids/utils/GeoTiff.hh" // USES GeoTiff

#include "ModelPoints.hh"

#include <cppunit/extensions/HelperMacros.h>

#include <fstream> // USES std::ifstream, std::ofstream
#include <iomanip> // USES ios::setf(), ios::setprecision()
#include <getopt.h> // USES optind
#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace apps {
        class TestIsosurface;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::TestIsosurface : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE -------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE(TestIsosurface);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testParseNoArgs);
    CPPUNIT_TEST(testParseArgsHelp);
    CPPUNIT_TEST(testParseArgsNoBBox);
    CPPUNIT_TEST(testParseArgsNoResolution);
    CPPUNIT_TEST(testParseArgsNoMaxDepth);
    CPPUNIT_TEST(testParseArgsNoIsosurface);
    CPPUNIT_TEST(testParseArgsNoModels);
    CPPUNIT_TEST(testParseArgsNoOutput);
    CPPUNIT_TEST(testParseArgsBadValues);
    CPPUNIT_TEST(testParseArgsWrong);
    CPPUNIT_TEST(testParseArgsMinimal);
    CPPUNIT_TEST(testParseArgsAll);
    CPPUNIT_TEST(testPrintHelp);
    CPPUNIT_TEST(testRunHelp);
    CPPUNIT_TEST(testRunOneBlockFlat);
    CPPUNIT_TEST(testRunThreeBlocksTopo);
    CPPUNIT_TEST(testRunBadOutput);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Setup test.
    void setUp(void);

    /// Test constructor.
    void testConstructor(void);

    /// Test _parseArgs() with no args.
    void testParseNoArgs(void);

    /// Test _parseArgs() with --help.
    void testParseArgsHelp(void);

    /// Test _parseArgs() missing --bbox.
    void testParseArgsNoBBox(void);

    /// Test _parseArgs() missing --hresolution.
    void testParseArgsNoResolution(void);

    /// Test _parseArgs() missing --max-depth.
    void testParseArgsNoMaxDepth(void);

    /// Test _parseArgs() missing --isosurface.
    void testParseArgsNoIsosurface(void);

    /// Test _parseArgs() missing --models.
    void testParseArgsNoModels(void);

    /// Test _parseArgs() missing --output.
    void testParseArgsNoOutput(void);

    /// Test _parseArgs() with bad values.
    void testParseArgsBadValues(void);

    /// Test _parseArgs() with wrong arguments.
    void testParseArgsWrong(void);

    /// Test _parseArgs() with required arguments.
    void testParseArgsMinimal(void);

    /// Test _parseArgs() with all arguments.
    void testParseArgsAll(void);

    /// Test _printHelp().
    void testPrintHelp(void);

    /// Test run() wth help.
    void testRunHelp(void);

    /// Test run() wth one-block-flat.
    void testRunOneBlockFlat(void);

    /// Test run() wth three-blocks-topo.
    void testRunThreeBlocksTopo(void);

    /// Test run() wth bad output file.
    void testRunBadOutput(void);

}; // class TestIsosurface
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::apps::TestIsosurface);

// ------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace apps {
        class _TestIsosurface;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::_TestIsosurface {
public:

    static
    void checkIsosurface(const char* filename,
                         const double isoOne,
                         const double isoTwo,
                         const geomodelgrids::testdata::ModelPoints& points);

}; // _TestIsosurface

// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::TestIsosurface::setUp(void) {
    optind = 1; // reset parsing of argc and argv
} // setUp


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::apps::TestIsosurface::testConstructor(void) {
    Isosurface isosurface;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points CRS.", std::string("EPSG:4326"), isosurface._bboxCRS);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in minX.", geomodelgrids::NODATA_VALUE, isosurface._minX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maxX.", geomodelgrids::NODATA_VALUE, isosurface._maxX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in minY.", geomodelgrids::NODATA_VALUE, isosurface._minY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maxY.", geomodelgrids::NODATA_VALUE, isosurface._maxY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in horizontal resolution.", 0.0, isosurface._horizRes);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in vertical resolution.", 10.0, isosurface._vertRes);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maximum depth.", 0.0, isosurface._maxDepth);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of search points.", 10, isosurface._numSearchPoints);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in depth surface.",
                                 geomodelgrids::serial::Query::SQUASH_TOPOGRAPHY_BATHYMETRY, isosurface._depthSurface);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in shallow preference.", true, isosurface._preferShallow);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of isosurfaces.", size_t(2), isosurface._isosurfaces.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface 1 value name.", std::string("Vs"),
                                 isosurface._isosurfaces[0].first);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface 1 value.", 1.0e+3, isosurface._isosurfaces[0].second);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface 2 value name.", std::string("Vs"),
                                 isosurface._isosurfaces[1].first);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface 2 value.", 2.5e+3, isosurface._isosurfaces[1].second);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in help flag.", false, isosurface._showHelp);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with no args.
void
geomodelgrids::apps::TestIsosurface::testParseNoArgs(void) {
    const int nargs = 1;
    const char* const args[nargs] = { "test", };

    Isosurface isosurface;
    isosurface._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", isosurface._showHelp);
} // testParseNoArgs


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --help.
void
geomodelgrids::apps::TestIsosurface::testParseArgsHelp(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--help" };

    Isosurface isosurface;
    isosurface._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", isosurface._showHelp);
} // testParseArgsHelp


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --bbox.
void
geomodelgrids::apps::TestIsosurface::testParseArgsNoBBox(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--hresolution=0.5",
        "--isosurface=one,1.0",
        "--max-depth=2.0",
        "--models=one.h5",
        "--output=one.out",
    };

    Isosurface isosurface;
    CPPUNIT_ASSERT_THROW(isosurface._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoBBox


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --hresolution.
void
geomodelgrids::apps::TestIsosurface::testParseArgsNoResolution(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--bbox=0.0,1.0,0.0,1.0",
        "--isosurface=one,1.0",
        "--max-depth=2.0",
        "--models=one.h5",
        "--output=one.out",
    };

    Isosurface isosurface;
    CPPUNIT_ASSERT_THROW(isosurface._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoResolution


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --max-depth.
void
geomodelgrids::apps::TestIsosurface::testParseArgsNoMaxDepth(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--bbox=0.0,1.0,0.0,1.0",
        "--hresolution=0.5",
        "--isosurface=one,1.0",
        "--models=one.h5",
        "--output=one.out",
    };

    Isosurface isosurface;
    CPPUNIT_ASSERT_THROW(isosurface._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoMaxDepth


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --isosurface.
void
geomodelgrids::apps::TestIsosurface::testParseArgsNoIsosurface(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--bbox=0.0,1.0,0.0,1.0",
        "--hresolution=0.5",
        "--max-depth=2.0",
        "--models=one.h5",
        "--output=one.out",
    };

    Isosurface isosurface;
    CPPUNIT_ASSERT_THROW(isosurface._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoIsosurface


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --models.
void
geomodelgrids::apps::TestIsosurface::testParseArgsNoModels(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--bbox=0.0,1.0,0.0,1.0",
        "--hresolution=0.5",
        "--isosurface=one,1.0",
        "--max-depth=2.0",
        "--output=one.out",
    };

    Isosurface isosurface;
    CPPUNIT_ASSERT_THROW(isosurface._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoModels


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --output.
void
geomodelgrids::apps::TestIsosurface::testParseArgsNoOutput(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--bbox=0.0,1.0,0.0,1.0",
        "--hresolution=0.5",
        "--isosurface=one,1.0",
        "--max-depth=2.0",
        "--models=one.h5",
    };

    Isosurface isosurface;
    CPPUNIT_ASSERT_THROW(isosurface._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoOutput


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with bad values.
void
geomodelgrids::apps::TestIsosurface::testParseArgsBadValues(void) {
    const int nargs = 9;
    const char* const args[nargs] = {
        "test",
        "--bbox=-1.0,0.0,1.0,-3.0",
        "--hresolution=-0.5",
        "--vresolution=-0.5",
        "--isosurface=one",
        "--max-depth=-2.0",
        "--num-search-points=0",
        "--depth-reference=none",
        "--models=one.h5",
    };

    Isosurface isosurface;
    CPPUNIT_ASSERT_THROW(isosurface._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoOutput


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with wrong arguments.
void
geomodelgrids::apps::TestIsosurface::testParseArgsWrong(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--blah" };

    Isosurface isosurface;
    CPPUNIT_ASSERT_THROW(isosurface._parseArgs(nargs, const_cast<char**>(args)), std::logic_error);
} // testParseArgsWrong


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with required arguments.
void
geomodelgrids::apps::TestIsosurface::testParseArgsMinimal(void) {
    const int nargs = 8;
    const char* const args[nargs] = {
        "test",
        "--bbox=0.0,1.0,2.0,3.0",
        "--hresolution=0.5",
        "--max-depth=2.0",
        "--isosurface=one,1.0",
        "--isosurface=two,2.0",
        "--models=one.h5",
        "--output=iso.tiff",
    };

    Isosurface isosurface;
    isosurface._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of values.", size_t(2), isosurface._isosurfaces.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface value name.", std::string("one"),
                                 isosurface._isosurfaces[0].first);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface value.", 1.0, isosurface._isosurfaces[0].second);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface value name.", std::string("two"),
                                 isosurface._isosurfaces[1].first);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface value.", 2.0, isosurface._isosurfaces[1].second);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points CRS.", std::string("EPSG:4326"), isosurface._bboxCRS);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in minX.", 0.0, isosurface._minX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maxX.", 1.0, isosurface._maxX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in minY.", 2.0, isosurface._minY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maxY.", 3.0, isosurface._maxY);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in horizontal resolution.", 0.5, isosurface._horizRes);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in vertical resolution.", 10.0, isosurface._vertRes);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maximum depth.", 2.0, isosurface._maxDepth);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of search points.", 10, isosurface._numSearchPoints);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in depth surface.",
                                 geomodelgrids::serial::Query::SQUASH_TOPOGRAPHY_BATHYMETRY, isosurface._depthSurface);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in shallow preference.", true, isosurface._preferShallow);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", size_t(1), isosurface._modelFilenames.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in models.", std::string("one.h5"), isosurface._modelFilenames[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in output.", std::string("iso.tiff"), isosurface._outputFilename);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help for minimal args.", !isosurface._showHelp);
} // testParseArgsMinimal


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with all arguments.
void
geomodelgrids::apps::TestIsosurface::testParseArgsAll(void) {
    const int nargs = 14;
    const char* const args[nargs] = {
        "test",
        "--log=my.log",
        "--bbox=0.0,1.0,2.0,3.0",
        "--hresolution=0.5",
        "--vresolution=0.4",
        "--isosurface=one,1.0",
        "--isosurface=two,2.0",
        "--depth-reference=top_surface",
        "--max-depth=2.0",
        "--num-search-points=5",
        "--models=one.h5,two.h5",
        "--output=iso.tiff",
        "--prefer-deep",
        "--bbox-coordsys=EPSG:3311",
    };

    Isosurface isosurface;
    isosurface._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of values.", size_t(2), isosurface._isosurfaces.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface value name.", std::string("one"),
                                 isosurface._isosurfaces[0].first);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface value.", 1.0, isosurface._isosurfaces[0].second);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface value name.", std::string("two"),
                                 isosurface._isosurfaces[1].first);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in isosurface value.", 2.0, isosurface._isosurfaces[1].second);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points CRS.", std::string("EPSG:3311"), isosurface._bboxCRS);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in minX.", 0.0, isosurface._minX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maxX.", 1.0, isosurface._maxX);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in minY.", 2.0, isosurface._minY);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maxY.", 3.0, isosurface._maxY);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in horizontal resolution.", 0.5, isosurface._horizRes);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in vertical resolution.", 0.4, isosurface._vertRes);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maximum depth.", 2.0, isosurface._maxDepth);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of search points.", 5, isosurface._numSearchPoints);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in depth surface.",
                                 geomodelgrids::serial::Query::SQUASH_TOP_SURFACE, isosurface._depthSurface);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in shallow preference.", false, isosurface._preferShallow);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", size_t(2), isosurface._modelFilenames.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in models.", std::string("one.h5"), isosurface._modelFilenames[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in models.", std::string("two.h5"), isosurface._modelFilenames[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in output.", std::string("iso.tiff"), isosurface._outputFilename);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help for minimal args.", !isosurface._showHelp);
} // testParseArgsAll


// ------------------------------------------------------------------------------------------------
// Test _printHelp().
void
geomodelgrids::apps::TestIsosurface::testPrintHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Isosurface isosurface;
    isosurface._printHelp();
    std::cout.rdbuf(coutOrig);
    CPPUNIT_ASSERT_EQUAL(size_t(1473), coutHelp.str().length());
} // testPrintHelp


// ----------------------------------------------------------------------
// Test run() with help.
void
geomodelgrids::apps::TestIsosurface::testRunHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Isosurface isosurface;
    const int nargs = 2;
    const char* const args[nargs] = {
        "test",
        "--help",
    };
    isosurface.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CPPUNIT_ASSERT_EQUAL(size_t(1473), coutHelp.str().length());
} // testRunHelp


// ------------------------------------------------------------------------------------------------
// Test run() with one-block-flat.
void
geomodelgrids::apps::TestIsosurface::testRunOneBlockFlat(void) {
    const int nargs = 11;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5",
        "--bbox=37.30,37.40,-121.80,-121.65",
        "--hresolution=0.05",
        "--vresolution=500.0",
        "--isosurface=one,25.0e+3",
        "--isosurface=two,10.0e+3",
        "--max-depth=5.0e+3",
        "--output=one-block-flat-isosurface.tiff",
        "--bbox-coordsys=EPSG:4326",
        "--log=error.log",
    };
    geomodelgrids::testdata::OneBlockFlatIsosurface isosurfaceOne;

    Isosurface isosurface;
    isosurface.run(nargs, const_cast<char**>(args));

    _TestIsosurface::checkIsosurface("one-block-flat-isosurface.tiff", 25.0e+3, 10.0e+3, isosurfaceOne);
    std::ifstream slog("error.log");CPPUNIT_ASSERT(slog.is_open() && slog.good());
} // testRunOneBlockFlat


// ------------------------------------------------------------------------------------------------
// Test run() with three-blocks-topo.
void
geomodelgrids::apps::TestIsosurface::testRunThreeBlocksTopo(void) {
    const int nargs = 13;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo.h5",
        "--bbox=34.6,34.8,-117.7,-117.3",
        "--hresolution=0.1",
        "--vresolution=500.0",
        "--isosurface=one,12.0e+4",
        "--isosurface=two,40.0e+3",
        "--max-depth=45.0e+3",
        "--depth-reference=topography_bathymetry",
        "--prefer-deep",
        "--output=three-blocks-topo-isosurface.tiff",
        "--bbox-coordsys=EPSG:4326",
        "--log=error.log",
    };
    geomodelgrids::testdata::ThreeBlocksTopoIsosurface isosurfaceThree;

    Isosurface isosurface;
    isosurface.run(nargs, const_cast<char**>(args));

    _TestIsosurface::checkIsosurface("three-blocks-topo-isosurface.tiff", 12.0e+4, 40.0e+3, isosurfaceThree);
    std::ifstream slog("error.log");CPPUNIT_ASSERT(slog.is_open() && slog.good());
} // testRunThreeBlocksTopo


// ------------------------------------------------------------------------------------------------
// Test run() with bad output specification.
void
geomodelgrids::apps::TestIsosurface::testRunBadOutput(void) {
    const int nargs = 14;
    const char* const args[nargs] = {
        "test",
        "--log=my.log",
        "--bbox=0.0,1.0,2.0,3.0",
        "--hresolution=0.5",
        "--vresolution=0.4",
        "--isosurface=one,1.0",
        "--isosurface=two,2.0",
        "--depth-reference=top_surface",
        "--max-depth=2.0",
        "--num-search-points=5",
        "--models=../../data/three-blocks-topo.h5",
        "--output=blah/three-blocks-topo-isosurface.tiff",
        "--prefer-deep",
        "--bbox-coordsys=EPSG:3311",
    };

    Isosurface isosurface;
    CPPUNIT_ASSERT_THROW(isosurface.run(nargs, const_cast<char**>(args)), std::runtime_error);
} // testRunBadOutput


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_TestIsosurface::checkIsosurface(const char* filename,
                                                      const double isoOne,
                                                      const double isoTwo,
                                                      const geomodelgrids::testdata::ModelPoints& points) {
    const size_t spaceDim = 3;
    const size_t numPoints = points.getNumPoints();
    const double* const pointsXYZ = points.getXYZ();
    const double* const pointsLLE = points.getLatLonElev();
    const geomodelgrids::testdata::ModelPoints::Domain& domain = points.getDomain();

    geomodelgrids::utils::GeoTiff reader;
    reader.read(filename);

    const size_t numX = reader.getNumCols();
    const size_t numY = reader.getNumRows();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in image size.", numPoints, numX*numY);

    double minX = 0.0, maxX = 0.0, minY = 0.0, maxY = 0.0;
    reader.getBBox(&minX, &maxX, &minY, &maxY);

    const std::vector<std::string>& bandLabels = reader.getBandLabels();
    std::ostringstream label;
    label << "one=" << isoOne;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in label for band 'one'.", label.str(), bandLabels[0]);
    label.str("");
    label << "two=" << isoTwo;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in label for band 'two'.", label.str(), bandLabels[1]);

    const float* data = reader.getBands();

    const double tolerance = 1.0e-4;
    for (size_t iY = 0, iPt = 0; iY < numY; ++iY) {
        for (size_t iX = 0; iX < numX; ++iX, iPt++) {
            const size_t row = numY - iY - 1;
            const size_t col = iX;
            const double x = pointsXYZ[iPt*spaceDim+0];
            const double y = pointsXYZ[iPt*spaceDim+1];

            const double topElev = (domain.hasTopoBathy) ? points.computeTopoBathyElevation(x, y) : 0.0;

            { // Value 'one'
                const size_t iValue = 0;
                const double valueE = topElev - points.computeIsosurfaceOne(x, y, isoOne, topElev, domain.zBottom);
                const double value = data[iValue*numY*numX + row*numX + col];

                std::ostringstream msg;
                msg << "Mismatch for isosurface 'one' for point ("
                    << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
                const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, value, valueTolerance);
            } // Value 'one'

            { // Value 'two'
                const size_t iValue = 1;
                const double valueE = topElev - points.computeIsosurfaceTwo(x, y, isoTwo, topElev, domain.zBottom);
                const double value = data[iValue*numY*numX + row*numX + col];

                std::ostringstream msg;
                msg << "Mismatch for isosurface 'two' for point ("
                    << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
                const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
                CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, value, valueTolerance);
            } // Value 'two'

        } // for
    } // for
    reader.close();

} // checkIsosurface


// End of file
