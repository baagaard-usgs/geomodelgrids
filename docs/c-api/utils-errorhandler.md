# Utilities Error Handler functions

All of these functions are prefixed by `geomodelgrids_cerrorhandler`.

## Enums

### GeomodelgridsStatusEnum

* **GEOMODELGRIDS_OK** No errors.
* **GEOMODELGRIDS_WARNING** Non-fatal error.
* **GEOMODELGRIDS_ERROR** Fatal error.

## Functions

+ [geomodelgrids_cerrorhandler_setLogFilename(void* handle, const char* filename)](#geomodelgrids_cerrorhandler_setlogfilenamevoid-handle-const-char-filename)
+ [geomodelgrids_cerrorhandler_getLogFilename(void* handle)](#const-char-const-geomodelgrids_cerrorhandler_getlogfilenamevoid-handle-const)
+ [geomodelgrids_cerrorhandler_setLoggingOn(void* handle, const int value)](#geomodelgrids_cerrorhandler_setloggingonvoid-handle-const-int-value)
+ [geomodelgrids_cerrorhandler_resetStatus(void* handle)](#geomodelgrids_cerrorhandler_resetstatusvoid-handle)
+ [geomodelgrids_cerrorhandler_getStatus(void* handle)](#geomodelgridsstatusenum-geomodelgrids_cerrorhandler_getstatusvoid-handle-const)
+ [geomodelgrids_cerrorhandler_getMessage(void* handle)](#const-char-geomodelgrids_cerrorhandler_getmessagevoid-handle-const)


### geomodelgrids_cerrorhandler_setLogFilename(void* handle, const char* filename)

Set filename for logging and enable logging.

* **handle**[in] Pointer to C++ error handler object.
* **filename**[in] Name of file.


### const char* const geomodelgrids_cerrorhandler_getLogFilename(void* handle) const

Get filename used in logging.

* **handle**[in] Pointer to C++ error handler object.
* **returns** Name of file.


### geomodelgrids_cerrorhandler_setLoggingOn(void* handle, const int value)

Turn logging on/off.

The log filename must have been set for logging to work if it is
turned on.

Turning logging on after it has been turned off will cause subsequent
messages to be appended to the log file.

* **handle**[in] Pointer to C++ error handler object.
* **value**[in] 1 to turn on logging, 0 to turn logging off


### geomodelgrids_cerrorhandler_resetStatus(void* handle)

Reset error status and clear any error message.

* **handle**[in] Pointer to C++ error handler object.


### GeomodelgridsStatusEnum geomodelgrids_cerrorhandler_getStatus(void* handle) const

Get status.

* **handle**[in] Pointer to C++ error handler object.
* **returns** Status of errors.


### const char* geomodelgrids_cerrorhandler_getMessage(void* handle) const 

Get warning/error message.

* **handle**[in] Pointer to C++ error handler object.
* **returns** Warning/error message.
