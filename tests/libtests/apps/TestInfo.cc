/**
 * C++ unit testing of geomodelgrids::apps::TestInfo.
 */

#include <portinfo>

#include "geomodelgrids/apps/Info.hh" // USES Info

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <getopt.h> // USES optind
#include <iostream> // USES std::cout
#include <sstream> // USES std::ostringstream

namespace geomodelgrids {
    namespace apps {
        class TestInfo;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::TestInfo {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Constructor.
    TestInfo(void);

    /// Test constructor.
    void testConstructor(void);

    /// Test _parseArgs() with no arguments.
    void testParseArgsNone(void);

    /// Test _parseArgs() with --help.
    void testParseArgsHelp(void);

    /// Test _parseArgs() with missing required arguments.
    void testParseArgsMissing(void);

    /// Test _parseArgs() with wrong arguments.
    void testParseArgsWrong(void);

    /// Test _parseArgs() with --description.
    void testParseArgsDescription(void);

    /// Test _parseArgs() with --coordsys.
    void testParseArgsCoordSys(void);

    /// Test _parseArgs() with --values.
    void testParseArgsValues(void);

    /// Test _parseArgs() with --blocks.
    void testParseArgsBlocks(void);

    /// Test _parseArgs() with --verify.
    void testParseArgsVerify(void);

    /// Test _parseArgs() with --all.
    void testParseArgsAll(void);

    /// Test _parseArgs() with --description --coordsys --values --blocks.
    void testParseArgsMany(void);

    /// Test _printHelp().
    void testPrintHelp(void);

    /// Test run() wth help flag.
    void testRunHelp(void);

    /// Test run() wth one-block-flat.
    void testRunOneBlockFlat(void);

    /// Test run() wth three-blocks-topo.
    void testRunThreeBlocksTopo(void);

    /// Test run() wth one-block-flat and three-blocks-topo.
    void testRunTwoModels(void);

    /// Test run() wth bad model metadata.
    void testRunBadMetadata(void);

    /// Test run() wth bad top surface size.
    void testRunBadTopSurface(void);

    /// Test run() wth bad block size.
    void testRunBadBlock(void);

}; // class TestInfo

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestInfo::testConstructor", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testConstructor();
}
TEST_CASE("TestInfo::testParseArgsNone", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsNone();
}
TEST_CASE("TestInfo::testParseArgsHelp", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsHelp();
}
TEST_CASE("TestInfo::testParseArgsMissing", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsMissing();
}
TEST_CASE("TestInfo::testParseArgsWrong", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsWrong();
}
TEST_CASE("TestInfo::testParseArgsDescription", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsDescription();
}
TEST_CASE("TestInfo::testParseArgsCoordSys", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsCoordSys();
}
TEST_CASE("TestInfo::testParseArgsValues", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsValues();
}
TEST_CASE("TestInfo::testParseArgsBlocks", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsBlocks();
}
TEST_CASE("TestInfo::testParseArgsVerify", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsVerify();
}
TEST_CASE("TestInfo::testParseArgsAll", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsAll();
}
TEST_CASE("TestInfo::testParseArgsMany", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testParseArgsMany();
}
TEST_CASE("TestInfo::testPrintHelp", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testPrintHelp();
}
TEST_CASE("TestInfo::testRunHelp", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testRunHelp();
}
TEST_CASE("TestInfo::testRunOneBlockFlat", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testRunOneBlockFlat();
}
TEST_CASE("TestInfo::testRunThreeBlocksTopo", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testRunThreeBlocksTopo();
}
TEST_CASE("TestInfo::testRunTwoModels", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testRunTwoModels();
}
TEST_CASE("TestInfo::testRunBadMetadata", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testRunBadMetadata();
}
TEST_CASE("TestInfo::testRunBadTopSurface", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testRunBadTopSurface();
}
TEST_CASE("TestInfo::testRunBadBlock", "[TestInfo]") {
    geomodelgrids::apps::TestInfo().testRunBadBlock();
}

// ------------------------------------------------------------------------------------------------
geomodelgrids::apps::TestInfo::TestInfo(void) {
    optind = 1; // reset parsing of argc and argv
} // constructor


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::apps::TestInfo::testConstructor(void) {
    Info info;

    CHECK(false == info._showHelp);
    CHECK(false == info._showAll);
    CHECK(false == info._showDescription);
    CHECK(false == info._showBlocks);
    CHECK(false == info._showCoordSys);
    CHECK(false == info._showValues);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with no arguments.
void
geomodelgrids::apps::TestInfo::testParseArgsNone(void) {
    const int nargs = 1;
    const char* const args[nargs] = { "test" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CHECK(info._showHelp);
} // testParseArgsNone


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --help.
void
geomodelgrids::apps::TestInfo::testParseArgsHelp(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--help" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CHECK(info._showHelp);
} // testParseArgsHelp


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with missing required arguments.
void
geomodelgrids::apps::TestInfo::testParseArgsMissing(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--blocks" };

    Info info;
    CHECK_THROWS_AS(info._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsMissing


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with wrong arguments.
void
geomodelgrids::apps::TestInfo::testParseArgsWrong(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--blah" };

    Info info;
    CHECK_THROWS_AS(info._parseArgs(nargs, const_cast<char**>(args)), std::logic_error);
} // testParseArgsWrong


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --description.
void
geomodelgrids::apps::TestInfo::testParseArgsDescription(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--description" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CHECK(info._showDescription);
    CHECK(!info._showCoordSys);
    CHECK(!info._showValues);
    CHECK(!info._showBlocks);
    CHECK(!info._doVerification);
    CHECK(!info._showAll);
} // testParseArgsDescription


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --coordsys.
void
geomodelgrids::apps::TestInfo::testParseArgsCoordSys(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--coordsys" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CHECK(!info._showDescription);
    CHECK(info._showCoordSys);
    CHECK(!info._showValues);
    CHECK(!info._showBlocks);
    CHECK(!info._doVerification);
    CHECK(!info._showAll);
} // testParseArgsCoordSys


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --values.
void
geomodelgrids::apps::TestInfo::testParseArgsValues(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--values" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CHECK(!info._showDescription);
    CHECK(!info._showCoordSys);
    CHECK(info._showValues);
    CHECK(!info._showBlocks);
    CHECK(!info._doVerification);
    CHECK(!info._showAll);
} // testParseArgsValues


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --blocks.
void
geomodelgrids::apps::TestInfo::testParseArgsBlocks(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--blocks" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CHECK(!info._showDescription);
    CHECK(!info._showCoordSys);
    CHECK(!info._showValues);
    CHECK(info._showBlocks);
    CHECK(!info._doVerification);
    CHECK(!info._showAll);
} // testParseArgsBlocks


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --verify.
void
geomodelgrids::apps::TestInfo::testParseArgsVerify(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--verify" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CHECK(!info._showDescription);
    CHECK(!info._showCoordSys);
    CHECK(!info._showValues);
    CHECK(!info._showBlocks);
    CHECK(info._doVerification);
    CHECK(!info._showAll);
} // testParseArgsBlocks


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --all.
void
geomodelgrids::apps::TestInfo::testParseArgsAll(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--all" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CHECK(!info._showDescription);
    CHECK(!info._showCoordSys);
    CHECK(!info._showValues);
    CHECK(!info._showBlocks);
    CHECK(!info._doVerification);
    CHECK(info._showAll);
} // testParseArgsAll


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --description --coordsys --values --blocks.
void
geomodelgrids::apps::TestInfo::testParseArgsMany(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--models=A",
        "--description",
        "--coordsys",
        "--values",
        "--blocks",
    };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CHECK(info._showDescription);
    CHECK(info._showCoordSys);
    CHECK(info._showValues);
    CHECK(info._showBlocks);
    CHECK(!info._showAll);
} // testParseArgsMany


// ------------------------------------------------------------------------------------------------
// Test _printHelp().
void
geomodelgrids::apps::TestInfo::testPrintHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Info info;
    info._printHelp();
    std::cout.rdbuf(coutOrig);
    CHECK(size_t(728) == coutHelp.str().length());
} // testPrintHelp


// ------------------------------------------------------------------------------------------------
// Test run() with help.
void
geomodelgrids::apps::TestInfo::testRunHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Info info;
    const int nargs = 2;
    const char* const args[nargs] = {
        "test",
        "--help",
    };
    info.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CHECK(size_t(728) == coutHelp.str().length());
} // testRunHelp


// ------------------------------------------------------------------------------------------------
// Test run() with one-block-flat.
void
geomodelgrids::apps::TestInfo::testRunOneBlockFlat(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Info info;
    const int nargs = 7;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5",
        "--description",
        "--coordsys",
        "--values",
        "--blocks",
        "--verify",
    };
    info.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CHECK(size_t(1596) == coutHelp.str().length());
} // testRunOneBlockFlat


// ------------------------------------------------------------------------------------------------
// Test run() with three-blocks-topo.
void
geomodelgrids::apps::TestInfo::testRunThreeBlocksTopo(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Info info;
    const int nargs = 7;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo.h5",
        "--description",
        "--coordsys",
        "--values",
        "--blocks",
        "--verify",
    };
    info.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CHECK(size_t(2407) == coutHelp.str().length());
} // testRunThreeBlocksTopo


// ------------------------------------------------------------------------------------------------
// Test run() with one-block-flat and three-blocks-topo.
void
geomodelgrids::apps::TestInfo::testRunTwoModels(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Info info;
    const int nargs = 4;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5,../../data/three-blocks-topo.h5",
        "--description",
        "--coordsys",
    };
    info.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CHECK(size_t(1894) == coutHelp.str().length());
} // testRunTwoModels


// ------------------------------------------------------------------------------------------------
// Test run() wth bad model metadata.
void
geomodelgrids::apps::TestInfo::testRunBadMetadata(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Info info;
    const int nargs = 3;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo-missing-metadata.h5",
        "--verify",
    };
    info.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CHECK(size_t(2621) == coutHelp.str().length());
}


// ------------------------------------------------------------------------------------------------
// Test run() wth bad top surface size.
void
geomodelgrids::apps::TestInfo::testRunBadTopSurface(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Info info;
    const int nargs = 3;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-topo-bad-topo.h5",
        "--verify",
    };
    info.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CHECK(size_t(492) == coutHelp.str().length());
}


// ------------------------------------------------------------------------------------------------
// Test run() wth bad block size.
void
geomodelgrids::apps::TestInfo::testRunBadBlock(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Info info;
    const int nargs = 3;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo-bad-blocks.h5",
        "--verify",
    };
    info.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CHECK(size_t(993) == coutHelp.str().length());

}


// End of file
