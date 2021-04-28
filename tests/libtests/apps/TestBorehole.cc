/**
 * C++ unit testing of geomodelgrids::apps::Borehole.
 */

#include <portinfo>

#include "geomodelgrids/apps/Borehole.hh" // USES Borehole
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include "ModelPoints.hh"

#include <cppunit/extensions/HelperMacros.h>

#include <fstream> // USES std::ifstream, std::ofstream
#include <iomanip> // USES ios::setf(), ios::setprecision()
#include <getopt.h> // USES optind
#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace apps {
        class TestBorehole;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::TestBorehole : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE -------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE(TestBorehole);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testParseNoArgs);
    CPPUNIT_TEST(testParseArgsHelp);
    CPPUNIT_TEST(testParseArgsNoLocation);
    CPPUNIT_TEST(testParseArgsNoValues);
    CPPUNIT_TEST(testParseArgsNoModels);
    CPPUNIT_TEST(testParseArgsNoOutput);
    CPPUNIT_TEST(testParseArgsWrong);
    CPPUNIT_TEST(testParseArgsMinimal);
    CPPUNIT_TEST(testParseArgsAll);
    CPPUNIT_TEST(testPrintHelp);
    CPPUNIT_TEST(testRunHelp);
    CPPUNIT_TEST(testRunOneBlockFlat);
    CPPUNIT_TEST(testRunThreeBlocksTopo);
    CPPUNIT_TEST(testRunBadOutput);
    CPPUNIT_TEST(testRunBadLocation);

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

    /// Test _parseArgs() missing --location.
    void testParseArgsNoLocation(void);

    /// Test _parseArgs() missing --values.
    void testParseArgsNoValues(void);

    /// Test _parseArgs() missing --models.
    void testParseArgsNoModels(void);

    /// Test _parseArgs() missing --output.
    void testParseArgsNoOutput(void);

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

    /// Test run() wth bad output location.
    void testRunBadLocation(void);

}; // class TestBorehole
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::apps::TestBorehole);

// ------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace apps {
        class _TestBorehole;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::_TestBorehole {
public:

    static
    void checkBorehole(std::istream& sin,
                       const geomodelgrids::testdata::ModelPoints& points);

}; // _TestBorehole
// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::TestBorehole::setUp(void) {
    optind = 1; // reset parsing of argc and argv
} // setUp


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::apps::TestBorehole::testConstructor(void) {
    Borehole borehole;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points CRS.", std::string("EPSG:4326"), borehole._pointsCRS);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maximum depth.", 5000.0, borehole._maxDepth);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in resolution.", 10.0, borehole._dz);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in help flag.", false, borehole._showHelp);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with no args.
void
geomodelgrids::apps::TestBorehole::testParseNoArgs(void) {
    const int nargs = 1;
    const char* const args[nargs] = { "test", };

    Borehole borehole;
    borehole._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", borehole._showHelp);
} // testParseNoArgs


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --help.
void
geomodelgrids::apps::TestBorehole::testParseArgsHelp(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--help" };

    Borehole borehole;
    borehole._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", borehole._showHelp);
} // testParseArgsHelp


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --location.
void
geomodelgrids::apps::TestBorehole::testParseArgsNoLocation(void) {
    const int nargs = 4;
    const char* const args[nargs] = { "test", "--values=A", "--models=B", "--output=C" };

    Borehole borehole;
    CPPUNIT_ASSERT_THROW(borehole._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoPoints


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --values.
void
geomodelgrids::apps::TestBorehole::testParseArgsNoValues(void) {
    const int nargs = 4;
    const char* const args[nargs] = { "test", "--location=1.0,2.0", "--models=B", "--output=C" };

    Borehole borehole;
    CPPUNIT_ASSERT_THROW(borehole._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoModels


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --models.
void
geomodelgrids::apps::TestBorehole::testParseArgsNoModels(void) {
    const int nargs = 4;
    const char* const args[nargs] = { "test", "--location=1.0,2.0", "--values=B", "--output=C" };

    Borehole borehole;
    CPPUNIT_ASSERT_THROW(borehole._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoModels


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --output.
void
geomodelgrids::apps::TestBorehole::testParseArgsNoOutput(void) {
    const int nargs = 4;
    const char* const args[nargs] = { "test", "--location=1.0,2.0", "--values=A", "--models=B" };

    Borehole borehole;
    CPPUNIT_ASSERT_THROW(borehole._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoOutput


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with wrong arguments.
void
geomodelgrids::apps::TestBorehole::testParseArgsWrong(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--blah" };

    Borehole borehole;
    CPPUNIT_ASSERT_THROW(borehole._parseArgs(nargs, const_cast<char**>(args)), std::logic_error);
} // testParseArgsWrong


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with required arguments.
void
geomodelgrids::apps::TestBorehole::testParseArgsMinimal(void) {
    const int nargs = 5;
    const char* const args[nargs] = {
        "test",
        "--values=one,two",
        "--models=A",
        "--location=1.0,2.0",
        "--output=points.out",
    };

    Borehole borehole;
    borehole._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of values.", size_t(2), borehole._valueNames.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in values.", std::string("one"), borehole._valueNames[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in values.", std::string("two"), borehole._valueNames[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", size_t(1), borehole._modelFilenames.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in models.", std::string("A"), borehole._modelFilenames[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in location x coordinate.", 1.0, borehole._location[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in location y coordinate.", 2.0, borehole._location[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in output.", std::string("points.out"), borehole._outputFilename);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help for minimal args.", !borehole._showHelp);
} // testParseArgsMinimal


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with all arguments.
void
geomodelgrids::apps::TestBorehole::testParseArgsAll(void) {
    const int nargs = 9;
    const char* const args[nargs] = {
        "test",
        "--models=A",
        "--location=1.0,2.0",
        "--output=points.out",
        "--points-coordsys=EPSG:26910",
        "--max-depth=300.0",
        "--dz=100.0",
        "--values=one,two,three",
        "--log=error.log",
    };
    const size_t numValues = 3;
    const char* const valueNamesE[numValues] = { "one", "two", "three" };

    Borehole borehole;
    borehole._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", size_t(1), borehole._modelFilenames.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of values.", numValues, borehole._valueNames.size());
    for (size_t i = 0; i < numValues; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in value name.", std::string(valueNamesE[i]), borehole._valueNames[i]);
    } // for
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in models.", std::string("A"), borehole._modelFilenames[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in location x coordinate.", 1.0, borehole._location[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in location y coordinate.", 2.0, borehole._location[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in output.", std::string("points.out"), borehole._outputFilename);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points coordsys.", std::string("EPSG:26910"), borehole._pointsCRS);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in maximum depth.", 300.0, borehole._maxDepth);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in resolution.", 100.0, borehole._dz);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in log.", std::string("error.log"), borehole._logFilename);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", !borehole._showHelp);
} // testParseArgsAll


// ------------------------------------------------------------------------------------------------
// Test _printHelp().
void
geomodelgrids::apps::TestBorehole::testPrintHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Borehole borehole;
    borehole._printHelp();
    std::cout.rdbuf(coutOrig);
    CPPUNIT_ASSERT_EQUAL(size_t(1053), coutHelp.str().length());
} // testPrintHelp


// ----------------------------------------------------------------------
// Test run() with help.
void
geomodelgrids::apps::TestBorehole::testRunHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Borehole borehole;
    const int nargs = 2;
    const char* const args[nargs] = {
        "test",
        "--help",
    };
    borehole.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CPPUNIT_ASSERT_EQUAL(size_t(1053), coutHelp.str().length());
} // testRunHelp


// ------------------------------------------------------------------------------------------------
// Test run() with one-block-flat.
void
geomodelgrids::apps::TestBorehole::testRunOneBlockFlat(void) {
    const int nargs = 8;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5",
        "--location=37.381,-121.581",
        "--dz=1000.0",
        "--output=one-block-flat-borehole.out",
        "--points-coordsys=EPSG:4326",
        "--values=two,one",
        "--log=error.log",
    };
    geomodelgrids::testdata::OneBlockFlatBorehole boreholeOne;

    Borehole borehole;
    borehole.run(nargs, const_cast<char**>(args));

    std::ifstream sin("one-block-flat-borehole.out");CPPUNIT_ASSERT(sin.is_open() && sin.good());
    _TestBorehole::checkBorehole(sin, boreholeOne);
    sin.close();

    std::ifstream slog("error.log");CPPUNIT_ASSERT(slog.is_open() && slog.good());
} // testRunOneBlockFlat


// ------------------------------------------------------------------------------------------------
// Test run() with three-blocks-topo.
void
geomodelgrids::apps::TestBorehole::testRunThreeBlocksTopo(void) {
    const int nargs = 8;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo.h5",
        "--location=35.1,-117.7",
        "--max-depth=25.0e+3",
        "--dz=5.0e+3",
        "--output=three-blocks-topo-borehole.out",
        "--points-coordsys=EPSG:4326",
        "--values=two,one",
    };
    geomodelgrids::testdata::ThreeBlocksTopoBorehole boreholeThree;

    Borehole borehole;
    borehole.run(nargs, const_cast<char**>(args));

    std::ifstream sin("three-blocks-topo-borehole.out");CPPUNIT_ASSERT(sin.is_open() && sin.good());
    _TestBorehole::checkBorehole(sin, boreholeThree);
    sin.close();
} // testRunThreeBlocksTopo


// ------------------------------------------------------------------------------------------------
// Test run() with bad output specification.
void
geomodelgrids::apps::TestBorehole::testRunBadOutput(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo.h5",
        "--location=35.1,-117.7",
        "--output=blah/three-blocks-topo-borehole.out",
        "--points-coordsys=EPSG:4326",
        "--values=two,one",
    };
    geomodelgrids::testdata::ThreeBlocksTopoBorehole boreholeThree;

    Borehole borehole;
    CPPUNIT_ASSERT_THROW(borehole.run(nargs, const_cast<char**>(args)), std::runtime_error);
} // testRunBadOutput


// ------------------------------------------------------------------------------------------------
// Test run() with bad location.
void
geomodelgrids::apps::TestBorehole::testRunBadLocation(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo.h5",
        "--location=35.1,97.7",
        "--output=badlocation-borehole.out",
        "--points-coordsys=EPSG:4326",
        "--values=two,one",
    };
    geomodelgrids::testdata::ThreeBlocksTopoBorehole boreholeThree;

    Borehole borehole;
    CPPUNIT_ASSERT_EQUAL(1, borehole.run(nargs, const_cast<char**>(args)));
} // testRunBadLocation


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_TestBorehole::checkBorehole(std::istream& sin,
                                                  const geomodelgrids::testdata::ModelPoints& points) {
    const size_t spaceDim = 3;
    const size_t numPoints = points.getNumPoints();
    const double* const pointsXYZ = points.getXYZ();
    const double* const pointsLLE = points.getLatLonElev();

    std::string comment;
    std::getline(sin, comment); // Command
    std::getline(sin, comment); // Column headers

    const double groundSurf = pointsLLE[0*spaceDim+2];
    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double x = pointsXYZ[iPt*spaceDim+0];
        const double y = pointsXYZ[iPt*spaceDim+1];
        const double z = pointsXYZ[iPt*spaceDim+2];

        double elev, depth;
        const double tolerance = 1.0e-6;
        sin >> elev;CPPUNIT_ASSERT_MESSAGE("Could not read elevation in borehole.", sin.good());
        sin >> depth;CPPUNIT_ASSERT_MESSAGE("Could not read depth in borehole.", sin.good());

        const double elevE = pointsLLE[iPt*spaceDim+2];
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in elevation.", elevE, elev, tolerance*fabs(elevE));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Mismatch in depth.", groundSurf-elevE, depth, tolerance*fabs(groundSurf-elevE));

        { // Value 'two'
            double value = NODATA_VALUE;
            sin >> value;CPPUNIT_ASSERT_MESSAGE("Could not read value 'two' in output.", sin.good());

            const double valueE = points.computeValueTwo(x, y, z);

            std::ostringstream msg;
            msg << "Mismatch for value 'two' for point ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ", " << pointsLLE[iPt*spaceDim+2] << ").";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, value, valueTolerance);
        } // Value 'two'

        { // Value 'one'
            double value = NODATA_VALUE;
            sin >> value;CPPUNIT_ASSERT_MESSAGE("Could not read value 'one' in output.", sin.good());

            const double valueE = points.computeValueOne(x, y, z);

            std::ostringstream msg;
            msg << "Mismatch for value 'one' for point ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ", " << pointsLLE[iPt*spaceDim+2] << ").";
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, value, valueTolerance);
        } // Value 'one'
    } // for
} // checkBorehole


// End of file
