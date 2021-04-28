/** Error handler providing a flexible interface for logging errors and warnings.
 */

#if !defined(geomodelgrids_utils_errorhandler_hh)
#define geomodelgrids_utils_errorhandler_hh

#include "utilsfwd.hh" // forward declarations

#include <string> // HASA std::string
#include <iosfwd> // HOLDSA std::ostream

class geomodelgrids::utils::ErrorHandler {
    friend class TestErrorHandler;
    friend class TestCErrorHandler;

public:

    // PUBLIC ENUMS -------------------------------------------------------------------------------

    /// Enumerated type for error status.
    enum StatusEnum {
        OK=0, ///< No errors
        WARNING=1, ///< Non-fatal error
        ERROR=2 ///< Fatal error
    };

public:

    // PUBLIC METHODS -----------------------------------------------------------------------------

    /// Constructor
    ErrorHandler(void);

    /// Destructor
    ~ErrorHandler(void);

    /** Set filename for logging and enable logging.
     *
     * @param[in] filename Name of file
     */
    void setLogFilename(const char* filename);

    /** Get filename used in logging.
     *
     * @returns Name of file.
     */
    const char* const getLogFilename(void) const;

    /** Turn logging on/off.
     *
     * @pre The log filename must have been set for logging to work if
     * it is turned on.
     *
     * @note Turning logging on after it has been turned off will cause
     * subsequent messages to be appended to the log file.
     *
     * @param[in] value True to turn on logging, false to turn logging off
     */
    void setLoggingOn(const bool value);

    /// Reset error status and clear any error message.
    void resetStatus(void);

    /** Get status.
     *
     * @returns Status of errors
     */
    StatusEnum getStatus(void) const;

    /** Get warning/error message.
     *
     * @returns Warning/error message
     */
    const char* getMessage(void) const;

    /** Set status to error and store error message.
     *
     * @param[in] msg Error message
     */
    void setError(const char* msg);

    /** Set status to warning and store warning message.
     *
     * @param[in] msg Warning message
     */
    void setWarning(const char* msg);

    /** Write message to log file.
     *
     * @param[in] msg Message to write to log file
     */
    void logMessage(const char* msg);

private:

    // PRIVATE METHODS ----------------------------------------------------------------------------

    ErrorHandler(const ErrorHandler& h); ///< Not implemented
    const ErrorHandler& operator=(const ErrorHandler& h); ///< Not implemented

private:

    // PRIVATE MEMBERS ----------------------------------------------------------------------------

    std::string _message; ///< Message associated with error/warning
    std::string _logFilename; ///< Name of log file
    std::ofstream* _log; ///< Pointer to log file
    StatusEnum _status; ///< Error status

    static const char* _NULLFILE; ///< Name of null device

}; // ErrorHandler

#endif // geomodelgrids_utils_errorhandler_hh

// End of file
