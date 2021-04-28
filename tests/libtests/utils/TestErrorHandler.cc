/**
 * C++ unit testing of geomodelgrids::utils::ErrorHandler.
 */

#include <portinfo>

#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler

#include <cppunit/extensions/HelperMacros.h>

#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace utils {
        class TestErrorHandler;
    } // utils
} // geomodelgrids

class geomodelgrids::utils::TestErrorHandler : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestErrorHandler);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testLogFilename);
    CPPUNIT_TEST(testStatus);
    CPPUNIT_TEST(testGetMessage);
    CPPUNIT_TEST(testSetError);
    CPPUNIT_TEST(testSetWarning);
    CPPUNIT_TEST(testLogMessage);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test get/setLogFilename().
    void testLogFilename(void);

    /// Test resetStatus(), and getStatus().
    void testStatus(void);

    /// Test getMessage().
    void testGetMessage(void);

    /// Test setError().
    void testSetError(void);

    /// Test setWarning().
    void testSetWarning(void);

    /// Test logMessage().
    void testLogMessage(void);

}; // class TestErrorHandler
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::utils::TestErrorHandler);

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::utils::TestErrorHandler::testConstructor(void) {
    ErrorHandler handler;

    CPPUNIT_ASSERT_MESSAGE("Expected empty message.", handler._message.empty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in log filename.", std::string("/dev/null"), handler._logFilename);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in status.", ErrorHandler::OK, handler._status);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test set/getLogFilename().
void
geomodelgrids::utils::TestErrorHandler::testLogFilename(void) {
    ErrorHandler handler;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default.",
                                 std::string(ErrorHandler::_NULLFILE), std::string(handler.getLogFilename()));

    const std::string filename = "error.log";
    handler.setLogFilename(filename.c_str());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user value.", filename, std::string(handler.getLogFilename()));
} // testLogFilename


// ------------------------------------------------------------------------------------------------
// Test resetStatus(), and getStatus().
void
geomodelgrids::utils::TestErrorHandler::testStatus(void) {
    ErrorHandler handler;

    ErrorHandler::StatusEnum status = ErrorHandler::OK;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default status.", status, handler._status);

    status = ErrorHandler::WARNING;
    handler._status = status;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user status.", status, handler._status);
} // testStatus


// ------------------------------------------------------------------------------------------------
// Test getMessage().
void
geomodelgrids::utils::TestErrorHandler::testGetMessage(void) {
    ErrorHandler handler;

    std::string message = "";
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in default message.", message, std::string(handler.getMessage()));

    message = "Hello";
    handler._message = message;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in user message.", message, std::string(handler.getMessage()));
} // testGetMessage


// ------------------------------------------------------------------------------------------------
// Test setError().
void
geomodelgrids::utils::TestErrorHandler::testSetError(void) {
    ErrorHandler handler;

    const std::string message = "Error message";
    handler.setError(message.c_str());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in message.", message, std::string(handler.getMessage()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in status.", ErrorHandler::ERROR, handler.getStatus());
} // testSetError


// ------------------------------------------------------------------------------------------------
// Test setWarning().
void
geomodelgrids::utils::TestErrorHandler::testSetWarning(void) {
    ErrorHandler handler;

    const std::string message = "Warning message";
    handler.setWarning(message.c_str());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in message.", message, std::string(handler.getMessage()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in status.", ErrorHandler::WARNING, handler.getStatus());
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
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in log.", message, std::string(buffer));
} // testLogMessage


// End of file
