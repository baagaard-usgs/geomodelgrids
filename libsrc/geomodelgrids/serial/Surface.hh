/** Surface stored as HDF5 file.
 */

#if !defined(geomodelgrids_serial_surface_hh)
#define geomodelgrids_serial_surface_hh

#include "serialfwd.hh" // forward declarations

#include <string> // HASA std::string

class geomodelgrids::serial::Surface {
    friend class TestSurface; // Unit testing

    // PUBLIC METHODS --------------------------------------------------------------------------------------------------
public:

    /** Default constructor.
     *
     * @param[in] name Name of surface.
     */
    Surface(const char* const name);

    /// Destructor
    ~Surface(void);

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

    /** Get number of values along each grid dimension.
     *
     * @returns Number of points along grid in each dimension [x, y].
     */
    const size_t* getDims(void) const;

    /** Set hyperslab size.
     *
     * @param[in] dims Dimensions of hyperslab.
     * @param[in] ndims Number of dimensions.
     */
    void setHyperslabDims(const size_t dims[],
                          const size_t ndims);

    /** Prepare for querying.
     *
     * @param[in] h5 HDF5 with model.
     */
    void openQuery(geomodelgrids::serial::HDF5* const h5);

    // Cleanup after querying.
    void closeQuery(void);

    /** Query for elevation (m) of ground surface at a point using bilinear interpolation.
     *
     * @param[in] x X coordinate of point in model coordinate system.
     * @param[in] y Y coordinate of point in model coordinate system.
     * @returns Elevation (m) of ground surface.
     */
    double query(const double x,
                 const double y);

    // PRIVATE MEMBERS -------------------------------------------------------------------------------------------------
private:

    geomodelgrids::serial::Hyperslab* _hyperslab; ///< Hyperslab of data in model.
    std::string _name; ///< Name of surface (matches dataset in HDF5 file).
    double _resolutionHoriz; ///< Horizontal resolution (m).
    size_t _dims[2]; ///< Number of points along grid in each x and y dimension [x, y].
    size_t _hyperslabDims[3]; ///< Dimensions of hyperslab.

}; // Surface

#endif // geomodelgrids_serial_surface_hh

// End of file
