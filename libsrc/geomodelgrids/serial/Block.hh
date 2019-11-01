/** Block stored as HDF5 file.
 */

#if !defined(geomodelgrids_serial_block_hh)
#define geomodelgrids_serial_block_hh

#include "serialfwd.hh" // forward declarations

#include <vector> // HASA std::std::vector
#include <string> // HASA std::string

class geomodelgrids::serial::Block {
    friend class TestBlock; // Unit testing

    // PUBLIC METHODS --------------------------------------------------------------------------------------------------
public:

    /** Default constructor.
     *
     * @param[in] name Name of block.
     */
    Block(const char* name);

    /// Destructor
    ~Block(void);

    /** Load metadata.
     *
     * @param[in] h5 HDF5 with model.
     */
    void loadMetadata(geomodelgrids::serial::HDF5* const h5);

    /** Get name of block.
     *
     * @returns Name of block.
     */
    const std::string& getName(void) const;

    /** Get horizontal resolution.
     *
     * @returns Horizontal resolution (m).
     */
    double getResolutionHoriz(void) const;

    /** Get vertical resolution.
     *
     * @returns Vertical resolution (m).
     */
    double getResolutionVert(void) const;

    /** Get elevation of top of block in topological space.
     *
     * @returns Elevation (m) of top of block.
     */
    double getZTop(void) const;

    /** Get number of values along each grid dimension.
     *
     * @returns Number of points along grid in each dimension [x, y, z].
     */
    const size_t* getDims(void) const;

    /** Get number of values stored at each grid point.
     *
     * @returns Number of values stored at each grid point.
     */
    size_t getNumValues(void) const;

    /** Query for values at a point using bilinear interpolation.
     *
     * @param[in] x X coordinate of point in model coordinate system.
     * @param[in] y Y coordinate of point in model coordinate system.
     * @param[in] z Z coordinate of point in model coordinate system.
     * @returns Value of model at specified point.
     */
    const double* query(const double x,
                        const double y,
                        const double z);

    // PRIVATE MEMBERS -------------------------------------------------------------------------------------------------
private:

    std::string _name; ///< Name of block.
    double _resolutionHoriz; ///< Horizontal resolution (m).
    double _resolutionVert; ///< Vertical resolution (m).
    double _zTop; ///< Elevation (m) of top of block.
    double* _values;
    size_t _numValues; ///< Number of values stored at each grid point.
    size_t _dims[3]; ///< Number of points along grid in each coordinate dimension [x, y, z].

}; // Block

#endif // geomodelgrids_serial_block_hh

// End of file
