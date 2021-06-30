# Utilities CRSTransformer Object 

**Full name**: geomodelgrids::utils::CRSTransformer

## Methods

+ [CRSTransformer()](#crstransformer)
+ [setSrc(const char* value)](#setsrcconst-char-value)
+ [setDest(const char* value)](#setdestconst-char-value)
+ [initialize()](#initialize)
+ [transform(double* destX, double* destY, const double* destZ, const double srcX, const double srcY, const double srcZ)](#transformdouble-destx-double-desty-const-double-destz-const-double-srcx-const-double-srcy-const-double-srcz)


### CRSTransformer()

Constructor.


### setSrc(const char* value)

Set source coordinate system. String can be EPSG:XXXX, WKT, or Proj parameters.


* **value**[in] String specifying source coordinate system.


### setDest(const char* value)

Set destination coordinate system.  String can be EPSG:XXXX, WKT, or Proj parameters.

* **value**[in] String specifying destination coordinate system.


### initialize()

Initialize transformer.


### transform(double* destX, double* destY, const double* destZ, const double srcX, const double srcY, const double srcZ)

Transform coordinates from source to destination coordinate system. If `destZ` is `NULL`, then the z coordinate in the destination coordinate system is not computed.

* **destX**[out] X coordinate in destination coordinate system.
* **destY**[out] Y coordinate in destination coordinate system.
* **destZ**[out] Z coordinate in destination coordinate system.
* **srcX**[in] X coordinate in source coordinate system.
* **srcY**[in] Y coordinate in source coordinate system.
* **srcZ**[in] Z coordinate in source coordinate system.
