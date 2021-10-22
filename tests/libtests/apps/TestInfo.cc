/**
 * C++ unit testing of geomodelgrids::apps::TestInfo.
 */

#include <portinfo>

#include "geomodelgrids/apps/Info.hh" // USES Info

#include <cppunit/extensions/HelperMacros.h>

#include <getopt.h> // USES optind

namespace geomodelgrids {
    namespace apps {
        class TestInfo;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::TestInfo : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE -------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE(TestInfo);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testParseArgsNone);
    CPPUNIT_TEST(testParseArgsHelp);
    CPPUNIT_TEST(testParseArgsMissing);
    CPPUNIT_TEST(testParseArgsWrong);
    CPPUNIT_TEST(testParseArgsDescription);
    CPPUNIT_TEST(testParseArgsCoordSys);
    CPPUNIT_TEST(testParseArgsValues);
    CPPUNIT_TEST(testParseArgsBlocks);
    CPPUNIT_TEST(testParseArgsVerify);
    CPPUNIT_TEST(testParseArgsAll);
    CPPUNIT_TEST(testParseArgsMany);
    CPPUNIT_TEST(testPrintHelp);
    CPPUNIT_TEST(testRunHelp);
    CPPUNIT_TEST(testRunOneBlockFlat);
    CPPUNIT_TEST(testRunThreeBlocksTopo);
    CPPUNIT_TEST(testRunTwoModels);
    CPPUNIT_TEST(testRunBadMetadata);
    CPPUNIT_TEST(testRunBadTopSurface);
    CPPUNIT_TEST(testRunBadBlock);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Setup test.
    void setUp(void);

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
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::apps::TestInfo);

// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::TestInfo::setUp(void) {
    optind = 1; // reset parsing of argc and argv
} // setUp


// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::apps::TestInfo::testConstructor(void) {
    Info info;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in help flag.", false, info._showHelp);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in all flag.", false, info._showAll);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in description flag.", false, info._showDescription);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in blocks flag.", false, info._showBlocks);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in coordinate system flag.", false, info._showCoordSys);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in values flag.", false, info._showValues);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with no arguments.
void
geomodelgrids::apps::TestInfo::testParseArgsNone(void) {
    const int nargs = 1;
    const char* const args[nargs] = { "test" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", info._showHelp);
} // testParseArgsNone


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --help.
void
geomodelgrids::apps::TestInfo::testParseArgsHelp(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--help" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", info._showHelp);
} // testParseArgsHelp


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with missing required arguments.
void
geomodelgrids::apps::TestInfo::testParseArgsMissing(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--blocks" };

    Info info;
    CPPUNIT_ASSERT_THROW(info._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsMissing


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with wrong arguments.
void
geomodelgrids::apps::TestInfo::testParseArgsWrong(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--blah" };

    Info info;
    CPPUNIT_ASSERT_THROW(info._parseArgs(nargs, const_cast<char**>(args)), std::logic_error);
} // testParseArgsWrong


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --description.
void
geomodelgrids::apps::TestInfo::testParseArgsDescription(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--description" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in description.", info._showDescription);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in coordsys.", !info._showCoordSys);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in values.", !info._showValues);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in blocks.", !info._showBlocks);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in verify.", !info._doVerification);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in all.", !info._showAll);
} // testParseArgsDescription


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --coordsys.
void
geomodelgrids::apps::TestInfo::testParseArgsCoordSys(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--coordsys" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in description.", !info._showDescription);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in coordsys.", info._showCoordSys);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in values.", !info._showValues);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in blocks.", !info._showBlocks);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in verify.", !info._doVerification);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in all.", !info._showAll);
} // testParseArgsCoordSys


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --values.
void
geomodelgrids::apps::TestInfo::testParseArgsValues(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--values" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in description.", !info._showDescription);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in coordsys.", !info._showCoordSys);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in values.", info._showValues);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in blocks.", !info._showBlocks);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in verify.", !info._doVerification);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in all.", !info._showAll);
} // testParseArgsValues


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --blocks.
void
geomodelgrids::apps::TestInfo::testParseArgsBlocks(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--blocks" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in description.", !info._showDescription);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in coordsys.", !info._showCoordSys);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in values.", !info._showValues);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in blocks.", info._showBlocks);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in verify.", !info._doVerification);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in all.", !info._showAll);
} // testParseArgsBlocks


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --verify.
void
geomodelgrids::apps::TestInfo::testParseArgsVerify(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--verify" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in description.", !info._showDescription);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in coordsys.", !info._showCoordSys);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in values.", !info._showValues);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in blocks.", !info._showBlocks);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in verify.", info._doVerification);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in all.", !info._showAll);
} // testParseArgsBlocks


// ------------------------------------------------------------------------------------------------
// Test _parseArgs() with --all.
void
geomodelgrids::apps::TestInfo::testParseArgsAll(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--models=A", "--all" };

    Info info;
    info._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in description.", !info._showDescription);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in coordsys.", !info._showCoordSys);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in values.", !info._showValues);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in blocks.", !info._showBlocks);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in verify.", !info._doVerification);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in all.", info._showAll);
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
    CPPUNIT_ASSERT_MESSAGE("Mismatch in description.", info._showDescription);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in coordsys.", info._showCoordSys);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in values.", info._showValues);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in blocks.", info._showBlocks);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in all.", !info._showAll);
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
    CPPUNIT_ASSERT_EQUAL(size_t(728), coutHelp.str().length());
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
    CPPUNIT_ASSERT_EQUAL(size_t(728), coutHelp.str().length());
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
    CPPUNIT_ASSERT_EQUAL(size_t(1596), coutHelp.str().length());
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
    CPPUNIT_ASSERT_EQUAL(size_t(2407), coutHelp.str().length());
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
    CPPUNIT_ASSERT_EQUAL(size_t(1894), coutHelp.str().length());
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
    CPPUNIT_ASSERT_EQUAL(size_t(2621), coutHelp.str().length());
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
    CPPUNIT_ASSERT_EQUAL(size_t(492), coutHelp.str().length());
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
    CPPUNIT_ASSERT_EQUAL(size_t(993), coutHelp.str().length());

}


// End of file
