/** Model stored as HDF5 file.
 */

#if !defined(geomodelgrids_serial_model_hh)
#define geomodelgrids_serial_model_hh

// Include directives ---------------------------------------------------
#include "serialfwd.hh" // forward declarations
#include "geomodelgrids/utils/utilsfwd.hh" // HOLDSA CRSTransformer

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

    /** Set CRS of query input points.
     *
     * @param[in] value CRS of input points as string (PROJ, EPSG, WKT).
     */
    void setInputCRS(const std::string& value);

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

    /** Initialize.
     */
    void initialize(void);

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

    /** Get coordinates of model origin (model CRS).
     *
     * @returns Coordinates of model origin [x, y].
     */
    const double* getOrigin(void) const;

    /** Get azimuth of y coordinate axies.
     *
     * @returns Azimuth (degrees) of y coordinate axis.
     */
    const double getYAzimuth(void) const;

    /** Get coordinate system of model.
     *
     * @returns CRS of model as string (PROJ, EPSG, or WKT).
     */
    const std::string& getCRSString(void) const;

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
     * @param[in] x X coordinate of point (in input CRS).
     * @param[in] y Y coordinate of point (in input CRS).
     * @param[in] z Z coordinate of point (in input CRS).
     * @returns True if model contains given point, false otherwise.
     */
    bool contains(const double x,
                  const double y,
                  const double z) const;

    /** Query for elevation of ground surface at point using bilinear interpolation.
     *
     * @param[in] x X coordinate of point (in input CRS).
     * @param[in] y Y coordinate of point (in input CRS).
     * @returns Elevation (m) of ground surface at point.
     */
    double queryElevation(const double x,
                          const double y);

    /** Query for model values at point using bilinear interpolation.
     *
     * @param[in] x X coordinate of point (in input CRS).
     * @param[in] y Y coordinate of point (in input CRS).
     * @param[in] z Z coordinate of point (in input CRS).
     * @returns Array of model values at point.
     */
    const double* query(const double x,
                        const double y,
                        const double z);

    // PRIVATE MEMBERS -------------------------------------------------------------------------------------------------

    /** Convert xyz in input CRS to xyz in model CRS.
     *
     * @param[out] xModel Model x coordinate of point.
     * @param[out] yModel Model y coordinate of point.
     * @param[out] zModel Model z coordinate of point.
     * @param[in] x X coordinate of point (in input CRS).
     * @param[in] y Y coordinate of point (in input CRS).
     * @param[in] z Z coordinate of point (in input CRS).
     */
    void _toModelXYZ(double* xModel,
                     double* yModel,
                     double* zModel,
                     const double x,
                     const double y,
                     const double z) const;

    /** Find block containing point.
     *
     * @param[in] x Model x coordinate of point.
     * @param[in] y Model y coordinate of point.
     * @param[in] z Model z coordinate of point.
     * @returns Block containing point.
     */
    geomodelgrids::serial::Block* _findBlock(const double x,
                                             const double y,
                                             const double z) const;

    // PRIVATE METHODS -------------------------------------------------------------------------------------------------
private:

    std::vector<std::string> _valueNames; ///< Names of values in model.
    std::vector<std::string> _valueUnits; ///< Units of values in model.
    std::string _modelCRSString; ///< Model CRS as string (PROJ, EPSG, or WKT).
    std::string _inputCRSString; ///< CRS as string (PROJ, EPSG, WKT for input points).
    double _origin[2]; ///< x and y coordinates of model origin.
    double _yazimuth; ///< Azimuth of y coordinate axis.
    double _dims[3]; ///< Dimensions of model along coordinate axes.

    geomodelgrids::serial::HDF5* _h5; ///< Model file.
    geomodelgrids::serial::ModelInfo* _info; ///< Model description information.
    geomodelgrids::serial::Topography* _topography; ///< Model topography.
    geomodelgrids::utils::CRSTransformer* _crsTransformer; ///< Coordinate system transformer.
    std::vector<geomodelgrids::serial::Block*> _blocks; ///< Model blocks.

}; // Model

#endif // geomodelgrids_serial_model_hh

// End of file
