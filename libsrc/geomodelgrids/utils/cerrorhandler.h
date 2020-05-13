/** C interface to ErrorHandler.
 */

#if !defined(geomodelgrids_utils_cerrorhandler_h)
#define geomodelgrids_utils_cerrorhandler_h

enum GeomodelgridsStatusEnum {
    GEOMODELGRIDS_OK=0, ///< No errors
    GEOMODELGRIDS_WARNING=1, ///< Non-fatal error
    GEOMODELGRIDS_ERROR=2 ///< Fatal error
};

/** Set filename for logging and enable logging.
 *
 * @param[in] Handle to C++ ErrorHandler.
 * @param[in] filename Name of file
 */
void geomodelgrids_cerrorhandler_setLogFilename(void* handle,
                                                const char* filename);

/** Get filename used in logging.
 *
 * @param[in] Handle to C++ ErrorHandler.
 * @returns Name of file.
 */
const char* const geomodelgrids_cerrorhandler_getLogFilename(void* handle);

/** Turn logging on/off.
 *
 * @pre The log filename must have been set for logging to work if
 * it is turned on.
 *
 * @note Turning logging on after it has been turned off will cause
 * subsequent messages to be appended to the log file.
 *
 * @param[in] Handle to C++ ErrorHandler.
 * @param value True to turn on logging, false to turn logging off
 */
void geomodelgrids_cerrorhandler_setLoggingOn(void* handle,
                                              const int value);

/** Reset error status and clear any error message.
 *
 * @param[in] Handle to C++ ErrorHandler.
 */
void geomodelgrids_cerrorhandler_resetStatus(void* handle);

/** Get status.
 *
 * @param[in] Handle to C++ ErrorHandler.
 *
 * @returns Status of errors
 */
enum GeomodelgridsStatusEnum geomodelgrids_cerrorhandler_getStatus(void* handle);

/** Get warning/error message.
 *
 * @param[in] Handle to C++ ErrorHandler.
 *
 * @returns Warning/error message
 */
const char* geomodelgrids_cerrorhandler_getMessage(void* handle);

#endif // geomodelgrids_utils_cerrorhandler_h

// End of file
