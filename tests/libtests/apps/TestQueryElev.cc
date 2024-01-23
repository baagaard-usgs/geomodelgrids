/**
 * C++ unit testing of geomodelgrids::apps::QueryElev.
 */

#include <portinfo>

#include "geomodelgrids/apps/QueryElev.hh" // USES QueryElev
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include "tests/data/ModelPoints.hh"

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <fstream> // USES std::ifstream, std::ofstream
#include <iostream> // USES std::cout
#include <sstream> // USES std::ostringstream
#include <iomanip> // USES ios::setf(), ios::setprecision()
#include <getopt.h> // USES optind
#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace apps {
        class TestQueryElev;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::TestQueryElev {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Constructor.
    TestQueryElev(void);

    /// Test constructor.
    void testConstructor(void);

    /// Test _parseArgs() with no args.
    void testParseNoArgs(void);

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

    /// Test run() wth three-blocks-topo and top surface.
    void testRunThreeBlocksTop(void);

    /// Test run() wth one-block-flat and three-blocks-topo.
    void testRunTwoModels(void);

    /// Test run() wth bad input.
    void testRunBadInput(void);

    /// Test run() wth bad output.
    void testRunBadOutput(void);

}; // class TestQueryElev

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestQueryElev::testConstructor", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testConstructor();
}
TEST_CASE("TestQueryElev::testParseNoArgs", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testParseNoArgs();
}
TEST_CASE("TestQueryElev::testParseArgsHelp", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testParseArgsHelp();
}
TEST_CASE("TestQueryElev::testParseArgsNoPoints", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testParseArgsNoPoints();
}
TEST_CASE("TestQueryElev::testParseArgsNoModels", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testParseArgsNoModels();
}
TEST_CASE("TestQueryElev::testParseArgsNoOutput", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testParseArgsNoOutput();
}
TEST_CASE("TestQueryElev::testParseArgsWrong", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testParseArgsWrong();
}
TEST_CASE("TestQueryElev::testParseArgsMinimal", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testParseArgsMinimal();
}
TEST_CASE("TestQueryElev::testParseArgsAll", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testParseArgsAll();
}
TEST_CASE("TestQueryElev::testPrintHelp", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testPrintHelp();
}
TEST_CASE("TestQueryElev::testRunHelp", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testRunHelp();
}
TEST_CASE("TestQueryElev::testRunOneBlockFlat", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testRunOneBlockFlat();
}
TEST_CASE("TestQueryElev::testRunThreeBlocksTop", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testRunThreeBlocksTop();
}
TEST_CASE("TestQueryElev::testRunTwoModels", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testRunTwoModels();
}
TEST_CASE("TestQueryElev::testRunBadInput", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testRunBadInput();
}
TEST_CASE("TestQueryElev::testRunBadOutput", "[TestQueryElev]") {
    geomodelgrids::apps::TestQueryElev().testRunBadOutput();
}

// ------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace apps {
        class _TestQueryElev;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::_TestQueryElev {
public:

    static
    void readHeader(std::istream& sin);

    static
    void createPointsFile(std::ostream& sout,
                          const geomodelgrids::testdata::ModelPoints& points);

    static
    void checkQuery(std::istream& sin,
                    const geomodelgrids::testdata::ModelPoints& points,
                    const double elevScale=1.0);

}; // _TestQueryElev

// ------------------------------------------------------------------------------------------------
geomodelgrids::apps::TestQueryElev::TestQueryElev(void) {
    optind = 1; // reset parsing of argc and argv
} // setUp


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::apps::TestQueryElev::testConstructor(void) {
    QueryElev query;

    CHECK(std::string("EPSG:4326") == query._pointsCRS);
    CHECK(false == query._showHelp);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with no args.
void
geomodelgrids::apps::TestQueryElev::testParseNoArgs(void) {
    const int nargs = 1;
    const char* const args[nargs] = { "test" };

    QueryElev query;
    query._parseArgs(nargs, const_cast<char**>(args));
    CHECK(query._showHelp);
} // testParseNoArgs


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --help.
void
geomodelgrids::apps::TestQueryElev::testParseArgsHelp(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--help" };

    QueryElev query;
    query._parseArgs(nargs, const_cast<char**>(args));
    CHECK(query._showHelp);
} // testParseArgsHelp


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --points.
void
geomodelgrids::apps::TestQueryElev::testParseArgsNoPoints(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--output=C" };

    QueryElev query;
    CHECK_THROWS_AS(query._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoPoints


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --models.
void
geomodelgrids::apps::TestQueryElev::testParseArgsNoModels(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--points=A", "--output=C" };

    QueryElev query;
    CHECK_THROWS_AS(query._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoModels


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() without --output.
void
geomodelgrids::apps::TestQueryElev::testParseArgsNoOutput(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--points=A", "--models=B" };

    QueryElev query;
    CHECK_THROWS_AS(query._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoOutput


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with wrong arguments.
void
geomodelgrids::apps::TestQueryElev::testParseArgsWrong(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--blah" };

    QueryElev query;
    CHECK_THROWS_AS(query._parseArgs(nargs, const_cast<char**>(args)), std::logic_error);
} // testParseArgsWrong


// ------------------------------------------------------------------------------------------------
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
    CHECK(size_t(1) == query._modelFilenames.size());
    CHECK(std::string("A") == query._modelFilenames[0]);
    CHECK(std::string("points.in") == query._pointsFilename);
    CHECK(std::string("points.out") == query._outputFilename);
    CHECK(!query._showHelp);
} // testParseArgsMinimal


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with all arguments.
void
geomodelgrids::apps::TestQueryElev::testParseArgsAll(void) {
    const int nargs = 7;
    const char* const args[nargs] = {
        "test",
        "--models=A",
        "--points=points.in",
        "--output=points.out",
        "--points-coordsys=EPSG:26910",
        "--surface=topography_bathymetry",
        "--log=error.log",
    };

    QueryElev query;
    query._parseArgs(nargs, const_cast<char**>(args));
    CHECK(size_t(1) == query._modelFilenames.size());
    CHECK(std::string("A") == query._modelFilenames[0]);
    CHECK(std::string("points.in") == query._pointsFilename);
    CHECK(std::string("points.out") == query._outputFilename);
    CHECK(std::string("EPSG:26910") == query._pointsCRS);
    CHECK(true == query._useTopoBathy);
    CHECK(std::string("error.log") == query._logFilename);
    CHECK(!query._showHelp);
} // testParseArgsAll


// ------------------------------------------------------------------------------------------------
// Test _printHelp().
void
geomodelgrids::apps::TestQueryElev::testPrintHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    QueryElev query;
    query._printHelp();
    std::cout.rdbuf(coutOrig);
    CHECK(size_t(758) == coutHelp.str().length());
} // testPrintHelp


// ------------------------------------------------------------------------------------------------
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
    CHECK(size_t(758) == coutHelp.str().length());
} // testRunHelp


// ------------------------------------------------------------------------------------------------
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
    std::ofstream sout("one-block-flat.in");assert(sout.is_open() && sout.good());
    _TestQueryElev::createPointsFile(sout, pointsOne);
    sout.close();

    QueryElev query;
    query.run(nargs, const_cast<char**>(args));

    std::ifstream sin("one-block-flat.out");assert(sin.is_open() && sin.good());
    _TestQueryElev::readHeader(sin);
    _TestQueryElev::checkQuery(sin, pointsOne);
    sin.close();

    std::ifstream slog("error.log");assert(slog.is_open() && slog.good());
} // testRunOneBlockFlat


// ------------------------------------------------------------------------------------------------
// Test run() with three-blocks-topo with top surface.
void
geomodelgrids::apps::TestQueryElev::testRunThreeBlocksTop(void) {
    const int nargs = 5;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo.h5",
        "--points=three-blocks-top.in",
        "--output=three-blocks-top.out",
        "--points-coordsys=+proj=lonlat +axis=neu +datum=WGS84 +vunits=km",
    };
    geomodelgrids::testdata::ThreeBlocksTopoPoints pointsThree;
    std::ofstream sout("three-blocks-top.in");assert(sout.is_open() && sout.good());
    _TestQueryElev::createPointsFile(sout, pointsThree);
    sout.close();

    QueryElev query;
    query.run(nargs, const_cast<char**>(args));

    const double elevScale = 1.0e-3;
    std::ifstream sin("three-blocks-top.out");assert(sin.is_open() && sin.good());
    _TestQueryElev::readHeader(sin);
    _TestQueryElev::checkQuery(sin, pointsThree, elevScale);
    sin.close();
} // testRunThreeBlocksTop


// ------------------------------------------------------------------------------------------------
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
    std::ofstream sout("two-models.in");assert(sout.is_open() && sout.good());
    _TestQueryElev::createPointsFile(sout, pointsOne);
    _TestQueryElev::createPointsFile(sout, pointsThree);
    sout.close();

    QueryElev query;
    query.run(nargs, const_cast<char**>(args));

    std::ifstream sin("two-models.out");assert(sin.is_open() && sin.good());
    _TestQueryElev::readHeader(sin);
    _TestQueryElev::checkQuery(sin, pointsOne);
    _TestQueryElev::checkQuery(sin, pointsThree);

    sin.close();
} // testRunTwoModels


// ------------------------------------------------------------------------------------------------
// Test run() with bad input.
void
geomodelgrids::apps::TestQueryElev::testRunBadInput(void) {
    const int nargs = 5;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5",
        "--points=blah/two-models.in",
        "--output=two-models.out",
        "--points-coordsys=EPSG:4326",
    };
    QueryElev query;
    CHECK_THROWS_AS(query.run(nargs, const_cast<char**>(args)), std::runtime_error);
} // testRunBadInput


// ------------------------------------------------------------------------------------------------
// Test run() with bad output.
void
geomodelgrids::apps::TestQueryElev::testRunBadOutput(void) {
    const int nargs = 5;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5",
        "--points=two-models.in",
        "--output=blah/two-models.out",
        "--points-coordsys=EPSG:4326",
    };
    QueryElev query;
    CHECK_THROWS_AS(query.run(nargs, const_cast<char**>(args)), std::runtime_error);
} // testRunBadOutput


// ------------------------------------------------------------------------------------------------
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


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_TestQueryElev::readHeader(std::istream& sin) {
    std::string comment;
    std::getline(sin, comment);
    std::getline(sin, comment);
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_TestQueryElev::checkQuery(std::istream& sin,
                                                const geomodelgrids::testdata::ModelPoints& points,
                                                const double elevScale) {
    const size_t spaceDim = 3;
    const size_t numPoints = points.getNumPoints();
    const double* const pointsXYZ = points.getXYZ();
    const double* const pointsLLE = points.getLatLonElev();
    const geomodelgrids::testdata::ModelPoints::Domain& domain = points.getDomain();
    const bool hasTopSurface = domain.hasTopSurface;

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double x = pointsXYZ[iPt*spaceDim+0];
        const double y = pointsXYZ[iPt*spaceDim+1];

        double xy[2];
        for (size_t iDim = 0; iDim < 2; ++iDim) {
            sin >> xy[iDim];
        } // for
        CHECK(sin.good());

        double value = NODATA_VALUE;
        sin >> value;CHECK(sin.good());

        const double valueE = hasTopSurface ? points.computeTopElevation(x, y) * elevScale : 0.0;

        INFO("Mismatch for 'elevation' for point ("
             << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ").");
        const double tolerance = 1.0e-6;
        const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
        CHECK_THAT(value, Catch::Matchers::WithinAbs(valueE, valueTolerance));
    } // for
} // checkQuery


// End of file
