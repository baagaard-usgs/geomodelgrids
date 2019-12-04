/** Topography stored as HDF5 file.
 */

#if !defined(geomodelgrids_serial_topography_hh)
#define geomodelgrids_serial_topography_hh

#include "serialfwd.hh" // forward declarations

#include <string> // HASA std::string

class geomodelgrids::serial::Topography {
    friend class TestTopography; // Unit testing

    // PUBLIC METHODS --------------------------------------------------------------------------------------------------
public:

    /// Default constructor.
    Topography(void);

    /// Destructor
    ~Topography(void);

    /** Load metadata.
     *
     * @param[in] h5 HDF5 with model.
     */
    void loadMetadata(geomodelgrids::serial::HDF5* const h5);

    /** Get horizontal resolution.
     *
     * @returns Horizontal resolution (m).
     */
    double getResolutionHoriz(void) const;

    /** Prepare for querying.
     *
     * @param[in] h5 HDF5 with model.
     */
    void openQuery(geomodelgrids::serial::HDF5* const h5);

    /** Query for elevation (m) of ground surface at a point using bilinear interpolation.
     *
     * @param[in] x X coordinate of point in model coordinate system.
     * @param[in] y Y coordinate of point in model coordinate system.
     * @returns Elevation (m) of ground surface.
     */
    double query(const double x,
                 const double y);

    // Cleanup after querying.
    void closeQuery(void);

    // PRIVATE MEMBERS -------------------------------------------------------------------------------------------------
private:

    geomodelgrids::serial::Hyperslab* _hyperslab; ///< Hyperslab of data in model.
    double _resolutionHoriz; ///< Horizontal resolution (m).
    size_t _dims[2]; ///< Number of points along grid in each x and y dimension [x, y].

}; // Topography

#endif // geomodelgrids_serial_topography_hh

// End of file
