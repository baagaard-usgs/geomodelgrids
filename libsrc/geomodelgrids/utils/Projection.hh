/** Geographic projection.
 */

#if !defined(geomodelgrids_utils_projection_hh)
#define geomodelgrids_utils_projection_hh

#include "utilsfwd.hh" // forward declarations

#include <string> // HASA std::string

class geomodelgrids::utils::Projection {
    friend class TestProjection; // Unit testing

    // PUBLIC METHODS --------------------------------------------------------------------------------------------------
public:

    /// Default constructor.
    Projection(void);

    /// Destructor
    ~Projection(void);

    /** Create projection from WKT string.
     *
     * @param[in] wkt Projection information as Well-Known Text string.
     */
    void fromWKT(const char* wkt);

    /// Initialize projection.
    void initialize(void);

    /** Compute xy coordinates in geographic projection.
     *
     * @param[out] x X coordinate in geographic projection.
     * @param[out] y Y coordinate in geographic projection.
     * @param[in] longitude Longitude (degrees, WGS84).
     * @param[in] latitude Latitude (degrees, WGS84).
     */
    void project(double* x,
                 double* y,
                 const double longitude,
                 const double latitude);

    // PRIVATE MEMBERS -------------------------------------------------------------------------------------------------
private:

}; // Projection

#endif // geomodelgrids_utils_projection_hh

// End of file
