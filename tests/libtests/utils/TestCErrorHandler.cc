/**
 * C++ unit testing of error handler C API.
 */

#include <portinfo>

extern "C" {
#include "geomodelgrids/utils/cerrorhandler.h"
}

#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler

#include "catch2/catch_test_macros.hpp"

#include <cassert>

namespace geomodelgrids {
    namespace utils {
        class TestCErrorHandler;
    } // utils
} // geomodelgrids

class geomodelgrids::utils::TestCErrorHandler {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    // Create test subject.
    TestCErrorHandler(void);

    // Destroy test subject.
    ~TestCErrorHandler(void);

    /// Test get/setLogFilename().
    void testLogFilename(void);

    /// Test resetStatus(), and getStatus().
    void testStatus(void);

    /// Test getMessage().
    void testGetMessage(void);

    // PRIVATE METHODS ////////////////////////////////////////////////////////////////////////////
private:

    ErrorHandler* _errorHandler;

}; // class TestCErrorHandler

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestCErrorHandler::testLogFilename", "[TestCErrorHandler]") {
    geomodelgrids::utils::TestCErrorHandler().testLogFilename();
}
TEST_CASE("TestCErrorHandler::testStatus", "[TestCErrorHandler]") {
    geomodelgrids::utils::TestCErrorHandler().testStatus();
}
TEST_CASE("TestCErrorHandler::testGetMessage", "[TestCErrorHandler]") {
    geomodelgrids::utils::TestCErrorHandler().testGetMessage();
}

// ------------------------------------------------------------------------------------------------
// Create test subject.
geomodelgrids::utils::TestCErrorHandler::TestCErrorHandler(void) {
    _errorHandler = new geomodelgrids::utils::ErrorHandler();
} // constructor


// ------------------------------------------------------------------------------------------------
// Destroy test subject.
geomodelgrids::utils::TestCErrorHandler::~TestCErrorHandler(void) {
    delete _errorHandler;_errorHandler = nullptr;
} // destructor


// ------------------------------------------------------------------------------------------------
// Test set/getLogFilename().
void
geomodelgrids::utils::TestCErrorHandler::testLogFilename(void) {
    assert(_errorHandler);

    const std::string& filenameE = "error.log";
    geomodelgrids_cerrorhandler_setLogFilename((void*)_errorHandler, filenameE.c_str());
    const std::string& filename = geomodelgrids_cerrorhandler_getLogFilename((void*)_errorHandler);
    CHECK(filenameE == filename);

    geomodelgrids_cerrorhandler_setLoggingOn((void*)_errorHandler, true);
    geomodelgrids_cerrorhandler_setLoggingOn((void*)_errorHandler, false);
} // testLogFilename


// ------------------------------------------------------------------------------------------------
// Test resetStatus(), and getStatus().
void
geomodelgrids::utils::TestCErrorHandler::testStatus(void) {
    assert(_errorHandler);

    GeomodelgridsStatusEnum status = GEOMODELGRIDS_OK;
    CHECK(status == geomodelgrids_cerrorhandler_getStatus((void*)_errorHandler));

    status = GEOMODELGRIDS_WARNING;
    _errorHandler->_status = geomodelgrids::utils::ErrorHandler::WARNING;
    CHECK(status == geomodelgrids_cerrorhandler_getStatus((void*)_errorHandler));

    status = GEOMODELGRIDS_ERROR;
    _errorHandler->_status = geomodelgrids::utils::ErrorHandler::ERROR;
    CHECK(status == geomodelgrids_cerrorhandler_getStatus((void*)_errorHandler));

    status = GEOMODELGRIDS_OK;
    geomodelgrids_cerrorhandler_resetStatus((void*)_errorHandler);
    CHECK(status == geomodelgrids_cerrorhandler_getStatus((void*)_errorHandler));
} // testStatus


// ------------------------------------------------------------------------------------------------
// Test getMessage().
void
geomodelgrids::utils::TestCErrorHandler::testGetMessage(void) {
    assert(_errorHandler);

    std::string messageE = "";
    std::string message = geomodelgrids_cerrorhandler_getMessage((void*)_errorHandler);
    CHECK(messageE == message);

    messageE = "Hello";
    _errorHandler->_message = messageE;
    message = geomodelgrids_cerrorhandler_getMessage((void*)_errorHandler);
    CHECK(messageE == message);
} // testGetMessage


// End of file
