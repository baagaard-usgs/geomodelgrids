/** Geographic projection.
 */

#if !defined(geomodelgrids_utils_projection_hh)
#define geomodelgrids_utils_projection_hh

#include "utilsfwd.hh" // forward declarations

#include "proj.h" // HOLDSA PJ

#include <string> // HASA std::string

class geomodelgrids::utils::Projection {
    friend class TestProjection; // Unit testing

    // PUBLIC METHODS --------------------------------------------------------------------------------------------------
public:

    /// Default constructor.
    Projection(void);

    /// Destructor
    ~Projection(void);

    /** Set source coordinate system.
     *
     * String can be EPSG:XXXX, WKT, or Proj.
     *
     * @param[in] value String specifying source coordinate system.
     */
    void setSrc(const char* value);

    /** Set destination coordinate system.
     *
     * String can be EPSG:XXXX, WKT, or Proj.
     *
     * @param[in] value String specifying destination coordinate system.
     */
    void setDest(const char* value);

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

    std::string _srcString;
    std::string _destString;
    PJ* _proj;

}; // Projection

#endif // geomodelgrids_utils_projection_hh

// End of file
