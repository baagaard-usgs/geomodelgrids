/**
 * C++ unit testing of error handler C API.
 */

#include <portinfo>

extern "C" {
#include "geomodelgrids/utils/cerrorhandler.h"
}

#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace utils {
        class TestCErrorHandler;
    } // utils
} // geomodelgrids

class geomodelgrids::utils::TestCErrorHandler : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestCErrorHandler);

    CPPUNIT_TEST(testLogFilename);
    CPPUNIT_TEST(testStatus);
    CPPUNIT_TEST(testGetMessage);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    // Create test subject.
    void setUp(void);

    // Destroy test subject.
    void tearDown(void);

    /// Test get/setLogFilename().
    void testLogFilename(void);

    /// Test resetStatus(), and getStatus().
    void testStatus(void);

    /// Test getMessage().
    void testGetMessage(void);

    // PRIVATE METHODS ///////////////////////////////////////////////////////
private:

    ErrorHandler* _errorHandler;

}; // class TestCErrorHandler
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::utils::TestCErrorHandler);

// ------------------------------------------------------------------------------------------------
// Create test subject.
void
geomodelgrids::utils::TestCErrorHandler::setUp(void) {
    _errorHandler = new geomodelgrids::utils::ErrorHandler();
} // setUp


// ------------------------------------------------------------------------------------------------
// Destroy test subject.
void
geomodelgrids::utils::TestCErrorHandler::tearDown(void) {
    delete _errorHandler;_errorHandler = NULL;
} // tearDown


// ------------------------------------------------------------------------------------------------
// Test set/getLogFilename().
void
geomodelgrids::utils::TestCErrorHandler::testLogFilename(void) {
    CPPUNIT_ASSERT(_errorHandler);

    const std::string& filenameE = "error.log";
    geomodelgrids_cerrorhandler_setLogFilename((void*)_errorHandler, filenameE.c_str());
    const std::string& filename = geomodelgrids_cerrorhandler_getLogFilename((void*)_errorHandler);
    CPPUNIT_ASSERT_EQUAL(filenameE, filename);

    geomodelgrids_cerrorhandler_setLoggingOn((void*)_errorHandler, true);
    geomodelgrids_cerrorhandler_setLoggingOn((void*)_errorHandler, false);
} // testLogFilename


// ------------------------------------------------------------------------------------------------
// Test resetStatus(), and getStatus().
void
geomodelgrids::utils::TestCErrorHandler::testStatus(void) {
    CPPUNIT_ASSERT(_errorHandler);

    GeomodelgridsStatusEnum status = GEOMODELGRIDS_OK;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default status.", status,
                                 geomodelgrids_cerrorhandler_getStatus((void*)_errorHandler));

    status = GEOMODELGRIDS_WARNING;
    _errorHandler->_status = geomodelgrids::utils::ErrorHandler::WARNING;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user status.", status,
                                 geomodelgrids_cerrorhandler_getStatus((void*)_errorHandler));

    status = GEOMODELGRIDS_ERROR;
    _errorHandler->_status = geomodelgrids::utils::ErrorHandler::ERROR;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user status.", status,
                                 geomodelgrids_cerrorhandler_getStatus((void*)_errorHandler));

    status = GEOMODELGRIDS_OK;
    geomodelgrids_cerrorhandler_resetStatus((void*)_errorHandler);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user status.", status,
                                 geomodelgrids_cerrorhandler_getStatus((void*)_errorHandler));
} // testStatus


// ------------------------------------------------------------------------------------------------
// Test getMessage().
void
geomodelgrids::utils::TestCErrorHandler::testGetMessage(void) {
    CPPUNIT_ASSERT(_errorHandler);

    std::string messageE = "";
    std::string message = geomodelgrids_cerrorhandler_getMessage((void*)_errorHandler);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default message.", messageE, message);

    messageE = "Hello";
    _errorHandler->_message = messageE;
    message = geomodelgrids_cerrorhandler_getMessage((void*)_errorHandler);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user message.", messageE, message);
} // testGetMessage


// End of file
