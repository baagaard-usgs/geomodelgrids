#include <portinfo>

extern "C" {
#include "cerrorhandler.h"
}

#include "ErrorHandler.hh" // USES ErrorHandler

#include <cassert> // USES assert

// ------------------------------------------------------------------------------------------------
// Set filename for logging and enable logging.
void
geomodelgrids_cerrorhandler_setLogFilename(void* handle,
                                           const char* filename) {
    if (!handle) { return; }

    geomodelgrids::utils::ErrorHandler* errorHandler = (geomodelgrids::utils::ErrorHandler*) handle;
    errorHandler->setLogFilename(filename);
} // setLogFilename


// ------------------------------------------------------------------------------------------------
// Get filename used in logging.
const char* const
geomodelgrids_cerrorhandler_getLogFilename(void* handle) {
    if (!handle) { return NULL; }

    geomodelgrids::utils::ErrorHandler* errorHandler = (geomodelgrids::utils::ErrorHandler*) handle;
    return errorHandler->getLogFilename();
} // getLogFilename


// ------------------------------------------------------------------------------------------------
// Turn logging on/off.
void
geomodelgrids_cerrorhandler_setLoggingOn(void* handle,
                                         const int value) {
    if (!handle) { return; }

    geomodelgrids::utils::ErrorHandler* errorHandler = (geomodelgrids::utils::ErrorHandler*) handle;
    errorHandler->setLoggingOn(bool(value));
} // setLoggingOn


// ------------------------------------------------------------------------------------------------
// Reset error status and clear any error message.
void
geomodelgrids_cerrorhandler_resetStatus(void* handle) {
    if (!handle) { return; }

    geomodelgrids::utils::ErrorHandler* errorHandler = (geomodelgrids::utils::ErrorHandler*) handle;
    errorHandler->resetStatus();
} // resetStatus


// ------------------------------------------------------------------------------------------------
// Get status.
enum GeomodelgridsStatusEnum
geomodelgrids_cerrorhandler_getStatus(void* handle) {
    if (!handle) { return GEOMODELGRIDS_OK; }

    geomodelgrids::utils::ErrorHandler* errorHandler = (geomodelgrids::utils::ErrorHandler*) handle;
    enum GeomodelgridsStatusEnum status = GEOMODELGRIDS_OK;
    switch (errorHandler->getStatus()) {
    case geomodelgrids::utils::ErrorHandler::OK:
        status = GEOMODELGRIDS_OK;
        break;
    case geomodelgrids::utils::ErrorHandler::WARNING:
        status = GEOMODELGRIDS_WARNING;
        break;
    case geomodelgrids::utils::ErrorHandler::ERROR:
        status = GEOMODELGRIDS_ERROR;
        break;
    default:
        assert(0);
    } // switch
    return status;
} // getStatus


// ------------------------------------------------------------------------------------------------
// Get warning/error message.
const char*
geomodelgrids_cerrorhandler_getMessage(void* handle) {
    if (!handle) { return NULL; }

    geomodelgrids::utils::ErrorHandler* errorHandler = (geomodelgrids::utils::ErrorHandler*) handle;
    return errorHandler->getMessage();
} // getMessage


// End of file
