# Utilities ErrorHandler Object 

**Full name**: geomodelgrids::utils::ErrorHandler

## Enums

### StatusEnum

* **OK** No errors.
* **WARNING** Non-fatal error.
* **ERROR** Fatal error.

## Methods

+ [ErrorHandler()](cxx-api-utils-errorhandler-ErrorHandler)
+ [setLogFilename(const char* filename)](cxx-api-utils-errorhandler-setLogFilename)
+ [getLogFilename()](cxx-api-utils-errorhandler-getLogFilename)
+ [setLoggingOn(const bool value)](cxx-api-utils-errorhandler-setLoggingOn)
+ [resetStatus()](cxx-api-utils-errorhandler-resetStatus)
+ [getStatus()](cxx-api-utils-errorhandler-getStatus)
+ [getMessage()](cxx-api-utils-errorhandler-getMessage)
+ [setError(const char* msg)](cxx-api-utils-errorhandler-setError)
+ [setWarning(const char* msg)](cxx-api-utils-errorhandler-setWarning)
+ [logMessage(const char* msg)](cxx-api-utils-errorhandler-logMessage)

(cxx-api-utils-errorhandler-ErrorHandler)=
### ErrorHandler()

Constructor.

(cxx-api-utils-errorhandler-setLogFilename)=
### setLogFilename(const char* filename)

Set filename for logging and enable logging.

* **filename**[in] Name of file.

(cxx-api-utils-errorhandler-getLogFilename)=
### const char* const getLogFilename(void) const

Get filename used in logging.

* **returns** Name of file.

(cxx-api-utils-errorhandler-setLoggingOn)=
### setLoggingOn(const bool value)

Turn logging on/off.

The log filename must have been set for logging to work if it is turned on.

Turning logging on after it has been turned off will cause subsequent messages to be appended to the log file.

* **value**[in] True to turn on logging, false to turn logging off

(cxx-api-utils-errorhandler-resetStatus)=
### resetStatus()

Reset error status and clear any error message.

(cxx-api-utils-errorhandler-getStatus)=
### StatusEnum getStatus() const

Get status.

* **returns** Status of errors.

(cxx-api-utils-errorhandler-getMessage)=
### const char* getMessage(void) const 

Get warning/error message.

* **returns** Warning/error message.

(cxx-api-utils-errorhandler-setError)=
### setError(const char* msg)

Set status to error and store error message.

* **msg**[in] Error message.

(cxx-api-utils-errorhandler-setWarning)=
### setWarning(const char* msg)

Set status to warning and store warning message.

* **msg** Warning message.

(cxx-api-utils-errorhandler-logMessage)=
### logMessage(const char* msg)

Write message to log file.

* **msg**[in] Message to write to log file.
