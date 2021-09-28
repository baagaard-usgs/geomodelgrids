/**
 * C++ unit testing of geomodelgrids::utils::Indexing.
 */

#include <portinfo>

#include "geomodelgrids/utils/Indexing.hh" // USES Indexing

#include <cppunit/extensions/HelperMacros.h>

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace utils {
        class TestIndexing;
    } // utils
} // geomodelgrids

class geomodelgrids::utils::TestIndexing : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestIndexing);

    CPPUNIT_TEST(testUniform);
    CPPUNIT_TEST(testVariableAscending);
    CPPUNIT_TEST(testVariableDescending);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test indexing for uniform resolution.
    void testUniform(void);

    /// Test indexing for variable resolution with coordinates increasing.
    void testVariableAscending(void);

    /// Test indexing for variable resolution with coordinates decreasing.
    void testVariableDescending(void);

}; // class TestIndexing
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::utils::TestIndexing);

// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::TestIndexing::testUniform(void) {
    const double tolerance = 1.0e-6;
    const double dx = 5.0;

    IndexingUniform indexing(dx);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, indexing.getIndex(0.0), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, indexing.getIndex(dx), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, indexing.getIndex(0.4*dx), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.3, indexing.getIndex(7.3*dx), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(103.2, indexing.getIndex(103.2*dx), tolerance);

    CPPUNIT_ASSERT_THROW_MESSAGE("Bad resolution", IndexingUniform bad(-dx), std::invalid_argument);
} // testUniform


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::TestIndexing::testVariableAscending(void) {
    const double tolerance = 1.0e-6;
    const size_t numX = 6;
    const double x[numX] = { -2.0, -4.0, 1.0, 3.0, 5.0, 10.0 };

    IndexingVariable indexing(x, numX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, indexing.getIndex(+4.0-4.0), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, indexing.getIndex(+4.0-2.0), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, indexing.getIndex(+4.0+10.0), tolerance);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.75, indexing.getIndex(+4.0-2.5), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.8, indexing.getIndex(+4.0+9.0), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.5, indexing.getIndex(+4.0+4.0), tolerance);

    CPPUNIT_ASSERT_THROW_MESSAGE("Bad coordinates", IndexingVariable bad(NULL, 1), std::invalid_argument);
    CPPUNIT_ASSERT_THROW_MESSAGE("Bad coordinates size", IndexingVariable bad(x, 0), std::invalid_argument);
} // testVariableAscending


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::TestIndexing::testVariableDescending(void) {
    const double tolerance = 1.0e-6;
    const size_t numX = 6;
    const double x[numX] = { -2.0, -4.0, 1.0, 3.0, 5.0, 10.0 };

    IndexingVariable indexing(x, numX, IndexingVariable::DESCENDING);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, indexing.getIndex(10.0+4.0), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, indexing.getIndex(10.0+2.0), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, indexing.getIndex(10.0-10.0), tolerance);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.25, indexing.getIndex(10.0+2.5), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, indexing.getIndex(10.0-9.0), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, indexing.getIndex(10.0-4.0), tolerance);
} // testVariableDescending


// End of file
