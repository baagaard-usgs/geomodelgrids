/** Block stored as HDF5 file.
 */

#if !defined(geomodelgrids_serial_block_hh)
#define geomodelgrids_serial_block_hh

#include "serialfwd.hh" // forward declarations

#include "geomodelgrids/utils/utilsfwd.hh" // forward declarations

#include <vector> // HASA std::std::vector
#include <string> // HASA std::string

class geomodelgrids::serial::Block {
    friend class TestBlock; // Unit testing

    // PUBLIC METHODS -----------------------------------------------------------------------------
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

    /** Get resolution along x axis.
     *
     * @returns Resolution along x axis.
     */
    double getResolutionX(void) const;

    /** Get resolution along y axis.
     *
     * @returns Resolution along y axis.
     */
    double getResolutionY(void) const;

    /** Get resolution along z axis.
     *
     * @returns Resolution along z axis.
     */
    double getResolutionZ(void) const;

    /** Get elevation of top of block in topological space.
     *
     * @returns Elevation (m) of top of block.
     */
    double getZTop(void) const;

    /** Get elevation of bottom of block in topological space.
     *
     * @returns Elevation (m) of bottom of block.
     */
    double getZBottom(void) const;

    /** Get coordinates along x axis.
     *
     * @returns Array of coordinates along x axis.
     */
    double* getCoordinatesX(void) const;

    /** Get coordinates along y axis.
     *
     * @returns Array of coordinates along y axis.
     */
    double* getCoordinatesY(void) const;

    /** Get coordinates along z axis.
     *
     * @returns Array of coordinates along z axis.
     */
    double* getCoordinatesZ(void) const;

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

    // Cleanup after querying.
    void closeQuery(void);

    /** Compare order of blocks by z_top (descending order).
     *
     * @param[in]  a Block to compare
     * @param[in]  b Block to compare
     * @returns True if a.z_top > b.z_top, else false.
     */
    static
    bool compare(const Block* a,
                 const Block* b);

    // PRIVATE MEMBERS ----------------------------------------------------------------------------
private:

    std::string _name; ///< Name of block.
    geomodelgrids::serial::Hyperslab* _hyperslab; ///< Hyperslab of data in model.
    double _resolutionX; ///< Resolution along x axis.
    double _resolutionY; ///< Resolution along y axis.
    double _resolutionZ; ///< Resolution along z axis.
    double _zTop; ///< Elevation (m) of top of block.

    double* _coordinatesX; ///< Coordinates along x axis.
    double* _coordinatesY; ///< Coordinates along y axis.
    double* _coordinatesZ; ///< Coordinates along z axis.

    geomodelgrids::utils::Indexing* _indexingX; ///< Procedure for finding index along x axis.
    geomodelgrids::utils::Indexing* _indexingY; ///< Procedure for finding index along y axis.
    geomodelgrids::utils::Indexing* _indexingZ; ///< Procedure for finding index along z axis.

    double* _values;
    size_t _numValues; ///< Number of values stored at each grid point.
    size_t _dims[3]; ///< Number of points along grid in each coordinate dimension [x, y, z].
    size_t _hyperslabDims[4]; ///< Dimensions of hyperslab.

    // NOT IMPLEMENTED ----------------------------------------------------------------------------
private:

    Block(const Block&); ///< Not implemented
    const Block& operator=(const Block&); ///< Not implemented

}; // Block

#endif // geomodelgrids_serial_block_hh

// End of file
