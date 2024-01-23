/**
 * C++ unit testing of geomodelgrids::utils::ErrorHandler.
 */

#include <portinfo>

#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler

#include "catch2/catch_test_macros.hpp"

#include <cassert>
#include <fstream> // USES std::ifstream
#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace utils {
        class TestErrorHandler;
    } // utils
} // geomodelgrids

class geomodelgrids::utils::TestErrorHandler {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Test constructor.
    static
    void testConstructor(void);

    /// Test get/setLogFilename().
    static
    void testLogFilename(void);

    /// Test resetStatus(), and getStatus().
    static
    void testStatus(void);

    /// Test getMessage().
    static
    void testGetMessage(void);

    /// Test setError().
    static
    void testSetError(void);

    /// Test setWarning().
    static
    void testSetWarning(void);

    /// Test logMessage().
    static
    void testLogMessage(void);

}; // class TestErrorHandler

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestErrorHandler::testConstructor", "[TestErrorHandler]") {
    geomodelgrids::utils::TestErrorHandler::testConstructor();
}
TEST_CASE("TestErrorHandler::testLogFilename", "[TestErrorHandler]") {
    geomodelgrids::utils::TestErrorHandler::testLogFilename();
}
TEST_CASE("TestErrorHandler::testStatus", "[TestErrorHandler]") {
    geomodelgrids::utils::TestErrorHandler::testStatus();
}
TEST_CASE("TestErrorHandler::testGetMessage", "[TestErrorHandler]") {
    geomodelgrids::utils::TestErrorHandler::testGetMessage();
}
TEST_CASE("TestErrorHandler::testSetError", "[TestErrorHandler]") {
    geomodelgrids::utils::TestErrorHandler::testSetError();
}
TEST_CASE("TestErrorHandler::testSetWarning", "[TestErrorHandler]") {
    geomodelgrids::utils::TestErrorHandler::testSetWarning();
}
TEST_CASE("TestErrorHandler::testLogMessage", "[TestErrorHandler]") {
    geomodelgrids::utils::TestErrorHandler::testLogMessage();
}

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::utils::TestErrorHandler::testConstructor(void) {
    ErrorHandler handler;

    CHECK(handler._message.empty());
    CHECK(std::string("/dev/null") == handler._logFilename);
    CHECK(ErrorHandler::OK == handler._status);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test set/getLogFilename().
void
geomodelgrids::utils::TestErrorHandler::testLogFilename(void) {
    ErrorHandler handler;

    CHECK(std::string(ErrorHandler::_NULLFILE) == std::string(handler.getLogFilename()));

    const std::string filename = "error.log";
    handler.setLogFilename(filename.c_str());
    CHECK(filename == std::string(handler.getLogFilename()));
} // testLogFilename


// ------------------------------------------------------------------------------------------------
// Test resetStatus(), and getStatus().
void
geomodelgrids::utils::TestErrorHandler::testStatus(void) {
    ErrorHandler handler;

    ErrorHandler::StatusEnum status = ErrorHandler::OK;
    CHECK(status == handler._status);

    status = ErrorHandler::WARNING;
    handler._status = status;
    CHECK(status == handler._status);
} // testStatus


// ------------------------------------------------------------------------------------------------
// Test getMessage().
void
geomodelgrids::utils::TestErrorHandler::testGetMessage(void) {
    ErrorHandler handler;

    std::string message = "";
    CHECK(message == std::string(handler.getMessage()));

    message = "Hello";
    handler._message = message;
    CHECK(message == std::string(handler.getMessage()));
} // testGetMessage


// ------------------------------------------------------------------------------------------------
// Test setError().
void
geomodelgrids::utils::TestErrorHandler::testSetError(void) {
    ErrorHandler handler;

    const std::string message = "Error message";
    handler.setError(message.c_str());

    CHECK(message == std::string(handler.getMessage()));
    CHECK(ErrorHandler::ERROR == handler.getStatus());
} // testSetError


// ------------------------------------------------------------------------------------------------
// Test setWarning().
void
geomodelgrids::utils::TestErrorHandler::testSetWarning(void) {
    ErrorHandler handler;

    const std::string message = "Warning message";
    handler.setWarning(message.c_str());

    CHECK(message == std::string(handler.getMessage()));
    CHECK(ErrorHandler::WARNING == handler.getStatus());
} // testSetWarning


// ------------------------------------------------------------------------------------------------
// Test logMessage().
void
geomodelgrids::utils::TestErrorHandler::testLogMessage(void) {
    ErrorHandler handler;

    const std::string message = "Log this";
    const std::string filename = "error.log";

    handler.setLogFilename(filename.c_str());

    handler.setLoggingOn(true);
    handler.logMessage(message.c_str());

    handler.setLoggingOn(false);
    handler.logMessage("Skip this");

    handler.setLoggingOn(true);
    handler.logMessage("\n");
    handler._log->close();

    const size_t maxLength = 32;
    char buffer[maxLength];
    std::ifstream sin(filename.c_str(), std::ios::in);
    sin.getline(buffer, maxLength);
    CHECK(message == std::string(buffer));
} // testLogMessage


// End of file
