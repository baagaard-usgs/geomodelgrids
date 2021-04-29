/// C++ interface for querying a model.
#if !defined(geomodelgrids_serial_query_hh)
#define geomodelgrids_serial_query_hh

#include "serialfwd.hh" // forward declarations

#include "geomodelgrids/utils/utilsfwd.hh" // HOLDSA ErrorHandler

#include <vector> // USES std::vector
#include <map> // USES std::map
#include <string> // USES std::string

class geomodelgrids::serial::Query {
    friend class TestQuery; // unit testing
    friend class TestCQuery; // unit testing
    friend class _Query; // Helper class

    // PUBLIC ENUMS -------------------------------------------------------------------------------
public:

    enum SquashingEnum {
        SQUASH_NONE=0,
        SQUASH_TOP_SURFACE=1,
        SQUASH_TOPOGRAPHY_BATHYMETRY=2,
    };

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Constructor
    Query(void);

    /// Destructor
    ~Query(void);

    /** Get error handler.
     *
     * @returns Error handler.
     */
    geomodelgrids::utils::ErrorHandler& getErrorHandler(void);

    /** Do setup for querying.
     *
     * @param[in] modelFilenames Array of model filenames (in query order).
     * @param[in] valueNames Array of names of values to return in query.
     * @param[in] inputCRSString CRS as string (PROJ, EPSG, WKT) for input points.
     */
    void initialize(const std::vector<std::string>& modelFilenames,
                    const std::vector<std::string>& valueNames,
                    const std::string& inputCRSString);

    /** Turn on squashing and set minimum elevation for squashing.
     *
     * Geometry below minimum elevation is not perturbed.
     *
     * @param[in] value Elevation (m) above which topography is squashed.
     */
    void setSquashMinElev(const double value);

    /** Set type of squashing.
     *
     * @param[in] value Type of squashing.
     */
    void setSquashing(const SquashingEnum value);

    /** Query for elevation of top of model at point.
     *
     * @param[in] x X coordinate of point (in input CRS).
     * @param[in] y Y coordinate of point (in input CRS).
     * @returns Elevation (m) of top of model at point.
     */
    double queryTopElevation(const double x,
                             const double y);

    /** Query for elevation of topography/bathymetry at point.
     *
     * @param[in] x X coordinate of point (in input CRS).
     * @param[in] y Y coordinate of point (in input CRS).
     * @returns Elevation (m) of ground surface at point.
     */
    double queryTopoBathyElevation(const double x,
                                   const double y);

    /** Query model for values at a point.
     *
     * Values array must be preallocated.
     *
     * @param[out] values Array of values returned in query.
     * @param[in] x X coordinate of point (in input CRS).
     * @param[in] y Y coordinate of point (in input CRS).
     * @param[in] z Z coordinate of point (in input CRS).
     * @returns 0 on success, 1 on error.
     */
    int query(double* const values,
              const double x,
              const double y,
              const double z);

    /// Cleanup after querying.
    void finalize(void);

    // PRIVATE TYPEDEFS ---------------------------------------------------------------------------
private:

    typedef std::map<size_t, size_t> values_map_type;

    // PRIVATE METHODS ----------------------------------------------------------------------------
private:

    // PRIVATE MEMBERS ----------------------------------------------------------------------------
private:

    std::vector<geomodelgrids::serial::Model*> _models;
    std::vector<std::string> _valuesLowercase;
    std::vector<values_map_type> _valuesIndex;
    double _squashMinElev;
    geomodelgrids::utils::ErrorHandler* _errorHandler;
    SquashingEnum _squash;

    // NOT IMPLEMENTED ----------------------------------------------------------------------------
private:

    Query(const Query&); ///< Not implemented
    const Query& operator=(const Query&); ///< Not implemented

}; // Query

#endif // geomodelgrids_serial_query_hh

// End of file
