#include <portinfo>

#include "ErrorHandler.hh" // implementation of class methods

#include <fstream> // USES std::ofstream
#include <cassert> // USES assert()

// ------------------------------------------------------------------------------------------------
const char* geomodelgrids::utils::ErrorHandler::_NULLFILE = "/dev/null";

// ------------------------------------------------------------------------------------------------
geomodelgrids::utils::ErrorHandler::ErrorHandler(void) :
    _message(""),
    _logFilename(_NULLFILE),
    _log(new std::ofstream(_NULLFILE)),
    _status(OK)
{}


// ------------------------------------------------------------------------------------------------
geomodelgrids::utils::ErrorHandler::~ErrorHandler(void) {
    if (_log) {
        _log->close();
    } // if
    delete _log;_log = NULL;
} // destructor


// ------------------------------------------------------------------------------------------------
// Set filename for logging.
void
geomodelgrids::utils::ErrorHandler::setLogFilename(const char* filename) {
    assert(_log);

    _logFilename = filename;

    if (_log->is_open()) {
        _log->close();
    } // if
    _log->clear();
    _log->open(_logFilename.c_str(), std::ios::out|std::ios::trunc);
} // setLogFilename


// ------------------------------------------------------------------------------------------------
// Get filename for logging.
const char* const
geomodelgrids::utils::ErrorHandler::getLogFilename(void) const {
    return _logFilename.c_str();
} // getLogFilename


// ------------------------------------------------------------------------------------------------
// Turn logging on/off.
void
geomodelgrids::utils::ErrorHandler::setLoggingOn(const bool turnOn) {
    assert(_log);

    _log->close();
    _log->clear();
    if (turnOn && ( _logFilename.length() > 0) ) {
        _log->open(_logFilename.c_str(), std::ios::out|std::ios::app);
    } else {
        _log->open(_NULLFILE, std::ios::out);
    } // if/else
} // setLoggingOn


// ------------------------------------------------------------------------------------------------
// Reset error status and message.
void
geomodelgrids::utils::ErrorHandler::resetStatus(void) {
    _status = OK;
    _message = "";
} // resetStatus


// ------------------------------------------------------------------------------------------------
// Get status.
geomodelgrids::utils::ErrorHandler::StatusEnum
geomodelgrids::utils::ErrorHandler::getStatus(void) const {
    return _status;
} // getStatus


// ------------------------------------------------------------------------------------------------
// Get warning/error message.
const char*
geomodelgrids::utils::ErrorHandler::getMessage(void) const {
    return _message.c_str();
} // getMessage


// ------------------------------------------------------------------------------------------------
// Set status to error and store error message.
void
geomodelgrids::utils::ErrorHandler::setError(const char* msg) {
    _status = ERROR;
    _message = msg;
} // setError


// ------------------------------------------------------------------------------------------------
// Set status to warning and store warning message.
void
geomodelgrids::utils::ErrorHandler::setWarning(const char* msg) {
    _status = WARNING;
    _message = msg;
} // setWarning


// ------------------------------------------------------------------------------------------------
// Write message to log file.
void
geomodelgrids::utils::ErrorHandler::logMessage(const char* msg) {
    assert(_log);
    (*_log) << msg;
} // logMessage


// End of file
