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
     * @returns Model description.
     */
    const double* getDims(void) const;

    /** Get coordinates of model origin.
     *
     * @returns x and y coordinates of model origin.
     */
    const double* getOrigin(void) const;

    /** Get azimuth of y coordinate axies.
     *
     * @returns Model description.
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

    /** Get model description.
     *
     * @returns Model description.
     */
    const geomodelgrids::serial::Topography* getTopography(void) const;

    /** Get model description.
     *
     * @returns Model description.
     */
    const std::vector<geomodelgrids::serial::Block*>& getBlocks(void) const;

    /** Does model
     *
     * @returns Model description.
     */
    bool contains(const double longitude,
                  const double latitude,
                  const double elevation) const;

    /** Get model description.
     *
     * @returns Model description.
     */
    double queryElevation(const double longitude,
                          const double latitude) const;

    /** Get model description.
     *
     * @returns Model description.
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
