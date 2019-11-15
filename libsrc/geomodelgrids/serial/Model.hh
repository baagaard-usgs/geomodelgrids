/** Model stored as HDF5 file.
 */

#if !defined(geomodelgrids_serial_model_hh)
#define geomodelgrids_serial_model_hh

// Include directives ---------------------------------------------------
#include "serialfwd.hh" // forward declarations

#include <vector> // HASA std::std::vector
#include <string> // HASA std::string

class geomodelgrids::serial::Model {
    friend class TestModel; // Unit testing

    // PUBLIC ENUMS ----------------------------------------------------------------------------------------------------
public:

    enum ModelMode {
        READ=0,
        READ_WRITE=1,
        READ_WRITE_TRUNCATE=2,
    }; // ModelEnum

    // PUBLIC METHODS --------------------------------------------------------------------------------------------------
public:

    /// Default constructor.
    Model(void);

    /// Destructor
    ~Model(void);

    /** Open Model.
     *
     * @param[in] filename Name of Model file
     * @param[in] mode Mode for Model file
     */
    void open(const char* filename,
              ModelMode mode);

    /// Close Model file.
    void close(void);

    /** Load metadata.
     */
    void loadMetadata(void);

    /** Get names of values in model.
     *
     * @returns Array of names of values in model.
     */
    const std::vector<std::string>& getValueNames(void) const;

    /** Get names of units in model.
     *
     * @returns Array of names of values in model.
     */
    const std::vector<std::string>& getValueUnits(void) const;

    /** Get model dimensions.
     *
     * @returns Model dimension (m) [x, y, z].
     */
    const double* getDims(void) const;

    /** Get coordinates of model origin in geographic projection.
     *
     * @returns Coordinates of model origin [x, y].
     */
    const double* getOrigin(void) const;

    /** Get azimuth of y coordinate axies.
     *
     * @returns Azimuth (degrees) of y coordinate axis.
     */
    const double getYAzimuth(void) const;

    /** Get geographic projection for model.
     *
     * @returns Geographic projection in WKT.
     */
    const std::string& getProjectionString(void) const;

    /** Get model description information.
     *
     * @returns Model description information.
     */
    const geomodelgrids::serial::ModelInfo* getInfo(void) const;

    /** Get model topography.
     *
     * @returns Model topography.
     */
    const geomodelgrids::serial::Topography* getTopography(void) const;

    /** Get model blocks.
     *
     * @returns Array of blocks in model.
     */
    const std::vector<geomodelgrids::serial::Block*>& getBlocks(void) const;

    /** Does model contain given point?
     *
     * @param[in] longitude Longitude (degrees, WGS84) of point.
     * @param[in] latitude (degrees, WGS84) of point.
     * @param[in] Elevation (m) of point.
     * @returns True if model contains given point, false otherwise.
     */
    bool contains(const double longitude,
                  const double latitude,
                  const double elevation) const;

    /** Query for elevation of ground surface at point using bilinear interpolation.
     *
     * @param[in] longitude Longitude (degrees, WGS84) of point.
     * @param[in] latitude (degrees, WGS84) of point.
     * @returns Elevation (m) of ground surface at point.
     */
    double queryElevation(const double longitude,
                          const double latitude) const;

    /** Query for model values at point using bilinear interpolation.
     *
     * @param[in] longitude Longitude (degrees, WGS84) of point.
     * @param[in] latitude (degrees, WGS84) of point.
     * @param[in] Elevation (m) of point.
     * @returns Array of model values at point.
     */
    const double* query(const double longitude,
                        const double latitude,
                        const double elevation) const;

    // PRIVATE MEMBERS ------------------------------------------------------
private:

    std::vector<std::string> _valueNames; ///< Names of values in model.
    std::vector<std::string> _valueUnits; ///< Units of values in model.
    std::string _projectionString; ///< Projection as string.
    double _origin[2]; ///< x and y coordinates of model origin.
    double _yazimuth; ///< Azimuth of y coordinate axis.
    double _dims[3]; ///< Dimensions of model along coordinate axes.

    geomodelgrids::serial::HDF5* _h5; ///< Model file.
    geomodelgrids::serial::ModelInfo* _info; ///< Model description information.
    geomodelgrids::serial::Topography* _topography; ///< Model topography.
    // geomodelgrids::utils::Projection* _projection; ///< Geographic projection of model.
    std::vector<geomodelgrids::serial::Block*> _blocks; ///< Model blocks.

}; // Model

#endif // geomodelgrids_serial_model_hh

// End of file
