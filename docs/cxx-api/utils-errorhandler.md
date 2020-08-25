# Utilities ErrorHandler Object 

**Full name**: geomodelgrids::utils::ErrorHandler

## Enums

### StatusEnum

* **OK** No errors.
* **WARNING** Non-fatal error.
* **ERROR** Fatal error.

## Methods

* [ErrorHandler()](#errorhandler)
* [setLogFilename(const char* filename)](#setlogfilenameconst-char-filename)
* [getLogFilename()](#const-char-const-getlogfilenamevoid-const)
* [setLoggingOn(const bool value)](#setloggingonconst-bool-value)
* [resetStatus()](#resetstatus)
* [getStatus()](#statusenum-getstatus-const)
* [getMessage()](#const-char-getmessagevoid-const)
* [setError(const char* msg)](#seterrorconst-char-msg)
* [setWarning(const char* msg)](#setwarningconst-char-msg)
* [logMessage(const char* msg)](#logmessageconst-char-msg)


### ErrorHandler()

Constructor.



### setLogFilename(const char* filename)

Set filename for logging and enable logging.

* **filename**[in] Name of file.


### const char* const getLogFilename(void) const

Get filename used in logging.

* **returns** Name of file.


### setLoggingOn(const bool value)

Turn logging on/off.

The log filename must have been set for logging to work if it is
turned on.

Turning logging on after it has been turned off will cause subsequent
messages to be appended to the log file.

* **value**[in] True to turn on logging, false to turn logging off


### resetStatus()

Reset error status and clear any error message.


### StatusEnum getStatus() const

Get status.

* **returns** Status of errors.


### const char* getMessage(void) const 

Get warning/error message.

* **returns** Warning/error message.


### setError(const char* msg)

Set status to error and store error message.

* **msg**[in] Error message.


### setWarning(const char* msg)

Set status to warning and store warning message.

* **msg** Warning message.


### logMessage(const char* msg)

Write message to log file.

* **msg**[in] Message to write to log file.
