/**
 * C++ unit testing of geomodelgrids::utils::Indexing.
 */

#include <portinfo>

#include "geomodelgrids/utils/Indexing.hh" // USES Indexing

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace utils {
        class TestIndexing;
    } // utils
} // geomodelgrids

class geomodelgrids::utils::TestIndexing {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Test indexing for uniform resolution.
    static
    void testUniform(void);

    /// Test indexing for variable resolution with coordinates increasing.
    static
    void testVariableAscending(void);

    /// Test indexing for variable resolution with coordinates decreasing.
    static
    void testVariableDescending(void);

}; // class TestIndexing

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestIndexing::testUniform", "[TestIndexing]") {
    geomodelgrids::utils::TestIndexing::testUniform();
}
TEST_CASE("TestIndexing::testVariableAscending", "[TestIndexing]") {
    geomodelgrids::utils::TestIndexing::testVariableAscending();
}
TEST_CASE("TestIndexing::testVariableDescending", "[TestIndexing]") {
    geomodelgrids::utils::TestIndexing::testVariableDescending();
}

// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::TestIndexing::testUniform(void) {
    const double tolerance = 1.0e-6;
    const double dx = 5.0;

    IndexingUniform indexing(dx);

    CHECK_THAT(indexing.getIndex(0.0), Catch::Matchers::WithinAbs(0.0, tolerance));
    CHECK_THAT(indexing.getIndex(dx), Catch::Matchers::WithinAbs(1.0, tolerance));
    CHECK_THAT(indexing.getIndex(0.4*dx), Catch::Matchers::WithinAbs(0.4, tolerance));
    CHECK_THAT(indexing.getIndex(7.3*dx), Catch::Matchers::WithinAbs(7.3, tolerance));
    CHECK_THAT(indexing.getIndex(103.2*dx), Catch::Matchers::WithinAbs(103.2, tolerance));

    CHECK_THROWS_AS(IndexingUniform(-dx), std::invalid_argument);
} // testUniform


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::TestIndexing::testVariableAscending(void) {
    const double tolerance = 1.0e-6;
    const size_t numX = 6;
    const double x[numX] = { -2.0, -4.0, 1.0, 3.0, 5.0, 10.0 };

    IndexingVariable indexing(x, numX);

    CHECK_THAT(indexing.getIndex(+4.0-4.0), Catch::Matchers::WithinAbs(0.0, tolerance));
    CHECK_THAT(indexing.getIndex(+4.0-2.0), Catch::Matchers::WithinAbs(1.0, tolerance));
    CHECK_THAT(indexing.getIndex(+4.0+10.0), Catch::Matchers::WithinAbs(5.0, tolerance));

    CHECK_THAT(indexing.getIndex(+4.0-2.5), Catch::Matchers::WithinAbs(0.75, tolerance));
    CHECK_THAT(indexing.getIndex(+4.0+9.0), Catch::Matchers::WithinAbs(4.8, tolerance));
    CHECK_THAT(indexing.getIndex(+4.0+4.0), Catch::Matchers::WithinAbs(3.5, tolerance));

    CHECK_THROWS_AS(IndexingVariable(NULL, 1), std::invalid_argument);
    CHECK_THROWS_AS(IndexingVariable(x, 0), std::invalid_argument);
} // testVariableAscending


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::TestIndexing::testVariableDescending(void) {
    const double tolerance = 1.0e-6;
    const size_t numX = 6;
    const double x[numX] = { -2.0, -4.0, 1.0, 3.0, 5.0, 10.0 };

    IndexingVariable indexing(x, numX, IndexingVariable::DESCENDING);

    CHECK_THAT(indexing.getIndex(10.0+4.0), Catch::Matchers::WithinAbs(5.0, tolerance));
    CHECK_THAT(indexing.getIndex(10.0+2.0), Catch::Matchers::WithinAbs(4.0, tolerance));
    CHECK_THAT(indexing.getIndex(10.0-10.0), Catch::Matchers::WithinAbs(0.0, tolerance));

    CHECK_THAT(indexing.getIndex(10.0+2.5), Catch::Matchers::WithinAbs(4.25, tolerance));
    CHECK_THAT(indexing.getIndex(10.0-9.0), Catch::Matchers::WithinAbs(0.2, tolerance));
    CHECK_THAT(indexing.getIndex(10.0-4.0), Catch::Matchers::WithinAbs(1.5, tolerance));
} // testVariableDescending


// End of file
