/* C interface for querying a model.
 */
#if !defined(geomodelgrids_serial_cquery_h)
#define geomodelgrids_serial_cquery_h

#define GEOMODELGRIDS_NODATA_VALUE -1.0e+20
#define GEOMODELGRIDS_SQUASH_NONE 0
#define GEOMODELGRIDS_SQUASH_TOP_SURFACE 1
#define GEOMODELGRIDS_SQUASH_TOPOGRAPHY_BATHYMETRY 2

/** Create query object.
 *
 * @returns Pointer to Query object (NULL on failure).
 */
void* geomodelgrids_squery_create(void);

/** Destroy query object.
 *
 * @param handle Query object.
 */
void geomodelgrids_squery_destroy(void** handle);

/** Get error handler.
 *
 * @param[in] handle Query object.
 */
void* geomodelgrids_squery_getErrorHandler(void* handle);

/** Do setup for querying.
 *
 * @param[inout] handle Handle to query object.
 * @param[in] modelFilenames Array of model filenames (in query order).
 * @param[in] modelFilenamesSize Size of modelFilenames array.
 * @param[in] valueNames Array of names of values to return in query.
 * @param[in] valueNamesSize Size of valueNames array.
 * @param[in] inputCRSString CRS as string (PROJ, EPSG, WKT) for input points.
 *
 * @returns Status of error handler.
 */
int geomodelgrids_squery_initialize(void* handle,
                                    const char* const modelFilenames[],
                                    const int modelFilenamesSize,
                                    const char* const valueNames[],
                                    const int valueNamesSize,
                                    const char* const inputCRSString);

/** Turn on squashing and set minimum elevation for squashing.
 *
 * Geometry below minimum elevation is not perturbed.
 *
 * @param[inout] handle Handle to query object.
 * @param[in] value Elevation (m) above which topography is squashed.
 *
 * @returns Status of error handler.
 */
int geomodelgrids_squery_setSquashMinElev(void* handle,
                                          const double value);

/** Set squashing type.
 *
 * @param[inout] handle Handle to query object.
 * @param[in] value True if squashing is on, false otherwise.
 */
int geomodelgrids_squery_setSquashing(void* handle,
                                      const int value);

/** Query for elevation of top of model at point.
 *
 * @param[inout] handle Handle to query object.
 * @param[in] x X coordinate of point (in input CRS).
 * @param[in] y Y coordinate of point (in input CRS).
 * @returns Elevation (m) of top of model at point.
 */
double geomodelgrids_squery_queryTopElevation(void* handle,
                                              const double x,
                                              const double y);

/** Query for elevation of topography/bathymetry at point.
 *
 * @param[inout] handle Handle to query object.
 * @param[in] x X coordinate of point (in input CRS).
 * @param[in] y Y coordinate of point (in input CRS).
 * @returns Elevation (m) of ground surface at point.
 */
double geomodelgrids_squery_queryTopoBathyElevation(void* handle,
                                                    const double x,
                                                    const double y);

/** Query model for values at a point.
 *
 * Values array must be preallocated.
 *
 * @param[inout] handle Handle to query object.
 * @param[out] values Array of values returned in query.
 * @param[in] x X coordinate of point (in input CRS).
 * @param[in] y Y coordinate of point (in input CRS).
 * @param[in] z Z coordinate of point (in input CRS).
 * @returns 0 on success, 1 on error.
 */
int geomodelgrids_squery_query(void* handle,
                               double* const values,
                               const double x,
                               const double y,
                               const double z);

/* Cleanup after querying.
 *
 * @param[inout] handle Handle to query object.
 */
int geomodelgrids_squery_finalize(void* handle);

#endif // geomodelgrids_serial_cquery_h

// End of file
