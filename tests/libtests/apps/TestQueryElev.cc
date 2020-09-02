/**
 * C++ unit testing of geomodelgrids::apps::QueryElev.
 */

#include <portinfo>

#include "geomodelgrids/apps/QueryElev.hh" // USES QueryElev
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include "ModelPoints.hh"

#include <cppunit/extensions/HelperMacros.h>

#include <fstream> // USES std::ifstream, std::ofstream
#include <iomanip> // USES ios::setf(), ios::setprecision()
#include <getopt.h> // USES optind
#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace apps {
        class TestQueryElev;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::TestQueryElev : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestQueryElev);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testParseArgsHelp);
    CPPUNIT_TEST(testParseArgsNoPoints);
    CPPUNIT_TEST(testParseArgsNoModels);
    CPPUNIT_TEST(testParseArgsNoOutput);
    CPPUNIT_TEST(testParseArgsWrong);
    CPPUNIT_TEST(testParseArgsMinimal);
    CPPUNIT_TEST(testParseArgsAll);
    CPPUNIT_TEST(testPrintHelp);
    CPPUNIT_TEST(testRunHelp);
    CPPUNIT_TEST(testRunOneBlockFlat);
    CPPUNIT_TEST(testRunThreeBlocksTopo);
    CPPUNIT_TEST(testRunTwoModels);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Setup test.
    void setUp(void);

    /// Test constructor.
    void testConstructor(void);

    /// Test _parseArgs() with --help.
    void testParseArgsHelp(void);

    /// Test _parseArgs() missing --points.
    void testParseArgsNoPoints(void);

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

    /// Test run() wth one-block-flat and three-blocks-topo.
    void testRunTwoModels(void);

}; // class TestQueryElev
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::apps::TestQueryElev);

// ---------------------------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace apps {
        class _TestQueryElev;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::_TestQueryElev {
public:

    static
    void createPointsFile(std::ostream& sout,
                          const geomodelgrids::testdata::ModelPoints& points);

    static
    void checkQuery(std::istream& sin,
                    const geomodelgrids::testdata::ModelPoints& points,
                    const bool hasTopography);

}; // _TestQueryElev

// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::TestQueryElev::setUp(void) {
    optind = 1; // reset parsing of argc and argv
} // setUp


// ---------------------------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::apps::TestQueryElev::testConstructor(void) {
    QueryElev query;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points CRS.", std::string("EPSG:4326"), query._pointsCRS);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in help flag.", false, query._showHelp);
} // testConstructor


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() with --help.
void
geomodelgrids::apps::TestQueryElev::testParseArgsHelp(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--help" };

    QueryElev query;
    query._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", query._showHelp);
} // testParseArgsHelp


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() without --points.
void
geomodelgrids::apps::TestQueryElev::testParseArgsNoPoints(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--models=A" };

    QueryElev query;
    CPPUNIT_ASSERT_THROW(query._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoPoints


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() without --models.
void
geomodelgrids::apps::TestQueryElev::testParseArgsNoModels(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--points=A" };

    QueryElev query;
    CPPUNIT_ASSERT_THROW(query._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoModels


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() without --output.
void
geomodelgrids::apps::TestQueryElev::testParseArgsNoOutput(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--points=A", "--models=B" };

    QueryElev query;
    CPPUNIT_ASSERT_THROW(query._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoOutput


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() with wrong arguments.
void
geomodelgrids::apps::TestQueryElev::testParseArgsWrong(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--blah" };

    QueryElev query;
    CPPUNIT_ASSERT_THROW(query._parseArgs(nargs, const_cast<char**>(args)), std::logic_error);
} // testParseArgsWrong


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() with required arguments.
void
geomodelgrids::apps::TestQueryElev::testParseArgsMinimal(void) {
    const int nargs = 4;
    const char* const args[nargs] = {
        "test",
        "--models=A",
        "--points=points.in",
        "--output=points.out",
    };

    QueryElev query;
    query._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", size_t(1), query._modelFilenames.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in models.", std::string("A"), query._modelFilenames[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points.", std::string("points.in"), query._pointsFilename);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in output.", std::string("points.out"), query._outputFilename);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help for minimal args.", !query._showHelp);
} // testParseArgsMinimal


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() with all arguments.
void
geomodelgrids::apps::TestQueryElev::testParseArgsAll(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--models=A",
        "--points=points.in",
        "--output=points.out",
        "--points-coordsys=EPSG:26910",
        "--log=error.log",
    };

    QueryElev query;
    query._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", size_t(1), query._modelFilenames.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in models.", std::string("A"), query._modelFilenames[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points.", std::string("points.in"), query._pointsFilename);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in output.", std::string("points.out"), query._outputFilename);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points coordsys.", std::string("EPSG:26910"), query._pointsCRS);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in log.", std::string("error.log"), query._logFilename);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", !query._showHelp);
} // testParseArgsAll


// ---------------------------------------------------------------------------------------------------------------------
// Test _printHelp().
void
geomodelgrids::apps::TestQueryElev::testPrintHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    QueryElev query;
    query._printHelp();
    std::cout.rdbuf(coutOrig);
    CPPUNIT_ASSERT_EQUAL(size_t(611), coutHelp.str().length());
} // testPrintHelp


// ----------------------------------------------------------------------
// Test run() with help.
void
geomodelgrids::apps::TestQueryElev::testRunHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    QueryElev query;
    const int nargs = 2;
    const char* const args[nargs] = {
        "test",
        "--help",
    };
    query.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CPPUNIT_ASSERT_EQUAL(size_t(611), coutHelp.str().length());
} // testRunHelp


// ---------------------------------------------------------------------------------------------------------------------
// Test run() with one-block-flat.
void
geomodelgrids::apps::TestQueryElev::testRunOneBlockFlat(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5",
        "--points=one-block-flat.in",
        "--output=one-block-flat.out",
        "--points-coordsys=EPSG:4326",
        "--log=error.log",
    };
    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    std::ofstream sout("one-block-flat.in");CPPUNIT_ASSERT(sout.is_open() && sout.good());
    _TestQueryElev::createPointsFile(sout, pointsOne);
    sout.close();

    QueryElev query;
    query.run(nargs, const_cast<char**>(args));

    std::ifstream sin("one-block-flat.out");CPPUNIT_ASSERT(sin.is_open() && sin.good());
    bool hasTopography = false;
    _TestQueryElev::checkQuery(sin, pointsOne, hasTopography);
    sin.close();

    std::ifstream slog("error.log");CPPUNIT_ASSERT(slog.is_open() && slog.good());
} // testRunOneBlockFlat


// ---------------------------------------------------------------------------------------------------------------------
// Test run() with three-blocks-topo.
void
geomodelgrids::apps::TestQueryElev::testRunThreeBlocksTopo(void) {
    const int nargs = 5;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo.h5",
        "--points=three-blocks-topo.in",
        "--output=three-blocks-topo.out",
        "--points-coordsys=EPSG:4326",
    };
    geomodelgrids::testdata::ThreeBlocksTopoPoints pointsThree;
    std::ofstream sout("three-blocks-topo.in");CPPUNIT_ASSERT(sout.is_open() && sout.good());
    _TestQueryElev::createPointsFile(sout, pointsThree);
    sout.close();

    QueryElev query;
    query.run(nargs, const_cast<char**>(args));

    std::ifstream sin("three-blocks-topo.out");CPPUNIT_ASSERT(sin.is_open() && sin.good());
    bool hasTopography = true;
    _TestQueryElev::checkQuery(sin, pointsThree, hasTopography);
    sin.close();
} // testRunThreeBlocksTopo


// ---------------------------------------------------------------------------------------------------------------------
// Test run() with one-block-flat and three-blocks-topo.
void
geomodelgrids::apps::TestQueryElev::testRunTwoModels(void) {
    const int nargs = 5;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5,../../data/three-blocks-topo.h5",
        "--points=two-models.in",
        "--output=two-models.out",
        "--points-coordsys=EPSG:4326",
    };
    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    geomodelgrids::testdata::ThreeBlocksTopoPoints pointsThree;
    std::ofstream sout("two-models.in");CPPUNIT_ASSERT(sout.is_open() && sout.good());
    _TestQueryElev::createPointsFile(sout, pointsOne);
    _TestQueryElev::createPointsFile(sout, pointsThree);
    sout.close();

    QueryElev query;
    query.run(nargs, const_cast<char**>(args));

    std::ifstream sin("two-models.out");CPPUNIT_ASSERT(sin.is_open() && sin.good());
    bool hasTopography = false;
    _TestQueryElev::checkQuery(sin, pointsOne, hasTopography);
    hasTopography = true;
    _TestQueryElev::checkQuery(sin, pointsThree, hasTopography);
    sin.close();
} // testRunTwoModels


// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_TestQueryElev::createPointsFile(std::ostream& sout,
                                                      const geomodelgrids::testdata::ModelPoints& points) {
    const size_t spaceDim = 3;
    const size_t numPoints = points.getNumPoints();
    const double* const pointsLLE = points.getLatLonElev();

    sout << std::fixed << std::setprecision(8);
    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        for (size_t iDim = 0; iDim < 2; ++iDim) {
            sout << std::setw(15) << pointsLLE[iPt*spaceDim+iDim];
        } // for
        sout << "\n";
    } // for
} // createPointsFile


// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_TestQueryElev::checkQuery(std::istream& sin,
                                                const geomodelgrids::testdata::ModelPoints& points,
                                                const bool hasTopography) {
    const size_t spaceDim = 3;
    const size_t numPoints = points.getNumPoints();
    const double* const pointsXYZ = points.getXYZ();
    const double* const pointsLLE = points.getLatLonElev();

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double x = pointsXYZ[iPt*spaceDim+0];
        const double y = pointsXYZ[iPt*spaceDim+1];

        double xy[2];
        for (size_t iDim = 0; iDim < 2; ++iDim) {
            sin >> xy[iDim];
        } // for

        double value = NODATA_VALUE;
        sin >> value;

        const double valueE = hasTopography ? points.computeElevation(x, y) : 0.0;

        std::ostringstream msg;
        msg << "Mismatch for 'elevation' for point ("
            << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").";
        const double tolerance = 1.0e-6;
        const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, value, valueTolerance);
    } // for
} // checkQuery


// End of file