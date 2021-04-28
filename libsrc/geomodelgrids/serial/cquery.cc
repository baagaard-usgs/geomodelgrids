#include <portinfo>

extern "C" {
#include "cquery.h"
}

#include "Query.hh" // USES Query
#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include <cassert> // USES assert()
#include <stdexcept> // USES std::exception
#include <iostream> // USES std::cerr
#include <sstream> // USES std::ostringstream, std::istringstream
#include <iomanip> // USES io manipulators

// ------------------------------------------------------------------------------------------------
// Create query object.
void*
geomodelgrids_squery_create(void) {
    return (void*) new geomodelgrids::serial::Query();

} // create


// ------------------------------------------------------------------------------------------------
// Destroy query object.
void
geomodelgrids_squery_destroy(void** handle) {
    geomodelgrids::serial::Query** query = (geomodelgrids::serial::Query**) handle;
    if (query) {
        delete *query;*query = NULL;
    } // if
} // destroy


// ------------------------------------------------------------------------------------------------
// Get error handler.
void*
geomodelgrids_squery_getErrorHandler(void* handle) {
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;
    geomodelgrids::utils::ErrorHandler* errorHandler = NULL;
    if (query) {
        errorHandler = &query->getErrorHandler();
    } // if

    return errorHandler;
} // getErrorHandler


// ------------------------------------------------------------------------------------------------
// Do setup for querying.
int
geomodelgrids_squery_initialize(void* handle,
                                const char* const modelFilenames[],
                                const int modelFilenamesSize,
                                const char* const valueNames[],
                                const int valueNamesSize,
                                const char* const inputCRSString) {
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;
    if (!handle) {
        std::cerr << "NULL handle for query object in call to geomodelgrids_squery_initialize().";
        return geomodelgrids::utils::ErrorHandler::ERROR;
    } // if

    std::vector < std::string > modelFilenamesCxx(modelFilenamesSize);
    for (int i = 0; i < modelFilenamesSize; ++i) {
        modelFilenamesCxx[i] = modelFilenames[i];
    } // for

    std::vector < std::string > valueNamesCxx(valueNamesSize);
    for (int i = 0; i < valueNamesSize; ++i) {
        valueNamesCxx[i] = valueNames[i];
    } // for

    const std::string& inputCRSStringCxx(inputCRSString);

    assert(query);
    try {
        query->initialize(modelFilenamesCxx, valueNamesCxx, inputCRSStringCxx);
    } catch (const std::exception& err) {
        geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
        errorHandler.setError(err.what());
    } // try/catch

    return query->getErrorHandler().getStatus();
} // initialize


// ------------------------------------------------------------------------------------------------
// Turn on squashing and set minimum z for squashing.
int
geomodelgrids_squery_setSquashMinElev(void* handle,
                                      const double value) {
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;
    if (!handle) {
        std::cerr << "NULL handle for query object in call to geomodelgrids_squery_setSquashMinElev().";
        return geomodelgrids::utils::ErrorHandler::ERROR;
    } // if

    assert(query);
    query->setSquashMinElev(value);

    return query->getErrorHandler().getStatus();
} // setSquashMinElev


// ------------------------------------------------------------------------------------------------
// Turn squashing on/off.
int
geomodelgrids_squery_setSquashing(void* handle,
                                  const int value) {
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;
    if (!handle) {
        std::cerr << "NULL handle for query object in call to geomodelgrids_squery_setSquashing().";
        return geomodelgrids::utils::ErrorHandler::ERROR;
    } // if

    assert(query);
    geomodelgrids::serial::Query::SquashingEnum valueEnum = geomodelgrids::serial::Query::SQUASH_NONE;
    switch (value) {
    case GEOMODELGRIDS_SQUASH_NONE:
        valueEnum = geomodelgrids::serial::Query::SQUASH_NONE;
        break;
    case GEOMODELGRIDS_SQUASH_TOP_SURFACE:
        valueEnum = geomodelgrids::serial::Query::SQUASH_TOP_SURFACE;
        break;
    case GEOMODELGRIDS_SQUASH_TOPOGRAPHY_BATHYMETRY:
        valueEnum = geomodelgrids::serial::Query::SQUASH_TOPOGRAPHY_BATHYMETRY;
        break;
    default:
        geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
        errorHandler.setError("Unknown squashing type.");

    } // switch
    query->setSquashing(valueEnum);

    return query->getErrorHandler().getStatus();
} // setSquashing


// ------------------------------------------------------------------------------------------------
// Query for elevation of top of model at point.
double
geomodelgrids_squery_queryTopElevation(void* handle,
                                       const double x,
                                       const double y) {
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;
    if (!handle) {
        std::cerr << "NULL handle for query object in call to geomodelgrids_squery_queryTopElevation().";
        return geomodelgrids::NODATA_VALUE;
    } // if

    assert(query);

    double elevation = geomodelgrids::NODATA_VALUE;
    try {
        elevation = query->queryTopElevation(x, y);
        if (elevation == geomodelgrids::NODATA_VALUE) {
            std::ostringstream warning;
            warning << "WARNING: Could not find model containing ("
                    << std::resetiosflags(std::ios::fixed)
                    << std::setiosflags(std::ios::scientific)
                    << std::setprecision(6)
                    << x << ", " << y << ") when querying for elevation of top of model.";
            geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
            errorHandler.setWarning(warning.str().c_str());
            errorHandler.logMessage(warning.str().c_str());
        } // if
    } catch (const std::exception& err) {
        std::ostringstream error;
        error << "ERROR: Fatal error when querying for elevation of top of model at point "
              << std::resetiosflags(std::ios::fixed)
              << std::setiosflags(std::ios::scientific)
              << std::setprecision(6)
              << x << ", " << y <<"\n" << err.what();
        geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
        errorHandler.setError(error.str().c_str());
        errorHandler.logMessage(error.str().c_str());
    } // try/catch

    return elevation;
} // queryTopElevation


// ------------------------------------------------------------------------------------------------
// Query for elevation of topography/bathymetry at point.
double
geomodelgrids_squery_queryTopoBathyElevation(void* handle,
                                             const double x,
                                             const double y) {
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;
    if (!handle) {
        std::cerr << "NULL handle for query object in call to geomodelgrids_squery_queryTopoBathyElevation().";
        return geomodelgrids::NODATA_VALUE;
    } // if

    assert(query);

    double elevation = geomodelgrids::NODATA_VALUE;
    try {
        elevation = query->queryTopoBathyElevation(x, y);
        if (elevation == geomodelgrids::NODATA_VALUE) {
            std::ostringstream warning;
            warning << "WARNING: Could not find model containing ("
                    << std::resetiosflags(std::ios::fixed)
                    << std::setiosflags(std::ios::scientific)
                    << std::setprecision(6)
                    << x << ", " << y << ") when querying for elevation of ground surface.";
            geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
            errorHandler.setWarning(warning.str().c_str());
            errorHandler.logMessage(warning.str().c_str());
        } // if
    } catch (const std::exception& err) {
        std::ostringstream error;
        error << "ERROR: Fatal error when querying for elevation of ground surface at point "
              << std::resetiosflags(std::ios::fixed)
              << std::setiosflags(std::ios::scientific)
              << std::setprecision(6)
              << x << ", " << y <<"\n" << err.what();
        geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
        errorHandler.setError(error.str().c_str());
        errorHandler.logMessage(error.str().c_str());
    } // try/catch

    return elevation;
} // queryTopoBathyElevation


// ------------------------------------------------------------------------------------------------
// Query at point.
int
geomodelgrids_squery_query(void* handle,
                           double* const values,
                           const double x,
                           const double y,
                           const double z) {
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;
    if (!handle) {
        std::cerr << "NULL handle for query object in call to geomodelgrids_squery_query().";
        return geomodelgrids::utils::ErrorHandler::ERROR;
    } // if

    assert(query);
    geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
    try {
        int err = query->query(values, x, y, z);
        if (err == geomodelgrids::utils::ErrorHandler::WARNING) {
            std::ostringstream warning;
            warning << "WARNING: Could not find model containing ("
                    << std::resetiosflags(std::ios::fixed)
                    << std::setiosflags(std::ios::scientific)
                    << std::setprecision(6)
                    << x << ", " << y << ", " << z << ") during query.";
            errorHandler.setWarning(warning.str().c_str());
            errorHandler.logMessage(warning.str().c_str());
        } // if
    } catch (const std::exception& err) {
        std::ostringstream error;
        error << "ERROR: Fatal error when querying for values at point "
              << std::resetiosflags(std::ios::fixed)
              << std::setiosflags(std::ios::scientific)
              << std::setprecision(6)
              << x << ", " << y << ", " << z <<"\n" << err.what();
        errorHandler.setError(error.str().c_str());
        errorHandler.logMessage(error.str().c_str());
    } // try/catch

    return errorHandler.getStatus();
} // query


// ------------------------------------------------------------------------------------------------
// Cleanup after querying.
int
geomodelgrids_squery_finalize(void* handle) {
    geomodelgrids::serial::Query* query = (geomodelgrids::serial::Query*) handle;
    if (!handle) {
        std::cerr << "NULL handle for query object in call to geomodelgrids_squery_setSquashMinElev.";
        return geomodelgrids::utils::ErrorHandler::ERROR;
    } // if

    assert(query);
    query->finalize();

    return query->getErrorHandler().getStatus();
} // finalize


// End of file
