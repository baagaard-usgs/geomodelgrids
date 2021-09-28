# Utilities CRSTransformer Object 

**Full name**: geomodelgrids::utils::CRSTransformer

## Methods

+ [CRSTransformer()](cxx-api-utils-crs-CRSTransformer)
+ [setSrc(const char* value)](cxx-api-utils-crs-setSrc)
+ [setDest(const char* value)](cxx-api-utils-crs-setDest)
+ [initialize()](cxx-api-utils-crs-initialize)
+ [transform(double* destX, double* destY, const double* destZ, const double srcX, const double srcY, const double srcZ)](cxx-api-utils-crs-transform)
+ [inverse_transform(double* srcX, double* srcY, const double* srcZ, const double destX, const double destY, const double destZ)](cxx-api-utils-crs-inverse-transform)
+ [createGeoToXYAxisOrder(const char*)](cxx-api-utils-crs-createGeoToXYAxisOrder)

(cxx-api-utils-crs-CRSTransformer)=
### CRSTransformer()

Constructor.

(cxx-api-utils-crs-setSrc)=
### setSrc(const char* value)

Set source coordinate system. String can be EPSG:XXXX, WKT, or Proj parameters.


* **value**[in] String specifying source coordinate system.

(cxx-api-utils-crs-setDest)=
### setDest(const char* value)

Set destination coordinate system.  String can be EPSG:XXXX, WKT, or Proj parameters.

* **value**[in] String specifying destination coordinate system.

(cxx-api-utils-crs-initialize)=
### initialize()

Initialize transformer.

(cxx-api-utils-crs-transform)=
### transform(double* destX, double* destY, const double* destZ, const double srcX, const double srcY, const double srcZ)

Transform coordinates from source to destination coordinate system. If `destZ` is `NULL`, then the z coordinate in the destination coordinate system is not computed.

* **destX**[out] X coordinate in destination coordinate system.
* **destY**[out] Y coordinate in destination coordinate system.
* **destZ**[out] Z coordinate in destination coordinate system.
* **srcX**[in] X coordinate in source coordinate system.
* **srcY**[in] Y coordinate in source coordinate system.
* **srcZ**[in] Z coordinate in source coordinate system.

(cxx-api-utils-crs-inverse-transform)=
### inverse_transform(ouble* srcX, double* srcY, double* srcZ, const double destX, const double destY, const double destZ)

Transform coordinates from destination to source coordinate system.

* **srcX[out]** X coordinate in source coordinate system.
* **srcY[out]** Y coordinate in source coordinate system.
* **srcZ[out]** Z coordinate in source coordinate system (can be NULL).
* **destX[in]** X coordinate in destination coordinate system.
* **destY[in]** Y coordinate in destination coordinate system.
* **destZ[in]** Z coordinate in destination coordinate system.

(cxx-api-utils-crs-createGeoToXYAxisOrder)=
### CRSTransformer* createGeoToXYAxisOrder(const char* crsString)

Create CRSTransformer that transforms axis order from geo to xy order.

* **crsString[in]** CRS for coordinate system.
* **returns** CRSTransformer.
