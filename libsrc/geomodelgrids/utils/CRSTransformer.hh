/** Transform from one georeferenced coordinate system to another.
 */

#if !defined(geomodelgrids_utils_crstransform_hh)
#define geomodelgrids_utils_crstransform_hh

#include "utilsfwd.hh" // forward declarations

#include "proj.h" // HOLDSA PJ

#include <string> // HASA std::string

class geomodelgrids::utils::CRSTransformer {
    friend class TestCRSTransformer; // Unit testing

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Default constructor.
    CRSTransformer(void);

    /// Destructor
    ~CRSTransformer(void);

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

    /// Initialize transfomer.
    void initialize(void);

    /** Transform coordinates from source to destination coordinate system.
     *
     * @param[out] destX X coordinate in destination coordinate system.
     * @param[out] destY Y coordinate in destination coordinate system.
     * @param[out] destZ Z coordinate in destination coordinate system (can be NULL).
     * @param[in] srcX X coordinate in source coordinate system.
     * @param[in] srcY Y coordinate in source coordinate system.
     * @param[in] srcZ Z coordinate in source coordinate system.
     */
    void transform(double* destX,
                   double* destY,
                   double* destZ,
                   const double srcX,
                   const double srcY,
                   const double srcZ);

    /** Transform coordinates from destination to source coordinate system.
     *
     * @param[out] srcX X coordinate in source coordinate system.
     * @param[out] srcY Y coordinate in source coordinate system.
     * @param[out] srcZ Z coordinate in source coordinate system (can be NULL).
     * @param[in] destX X coordinate in destination coordinate system.
     * @param[in] destY Y coordinate in destination coordinate system.
     * @param[in] destZ Z coordinate in destination coordinate system.
     */
    void inverse_transform(double* srcX,
                           double* srcY,
                           double* srcZ,
                           const double destX,
                           const double destY,
                           const double destZ);

    /** Create CRSTransformer that transforms axis order from geo to xy order.
     *
     * @param[in] crsString CRS for coordinate system.
     * @returns CRSTransformer.
     */
    static
    CRSTransformer* createGeoToXYAxisOrder(const char* crsString);

    /** Get units for CRS.
     *
     * @param[out] xUnit Units for x axis (can be NULL).
     * @param[out] yUnit Units for y axis (can be NULL).
     * @param[out] zUnit Units for z axis (can be NULL).
     * @param[in] crsString CRS for coordinate system.
     */
    static
    void getCRSUnits(std::string* xUnit,
                     std::string* yUnit,
                     std::string* zUnit,
                     const char* crsString);

    // PRIVATE MEMBERS ----------------------------------------------------------------------------
private:

    std::string _srcString;
    std::string _destString;
    PJ* _proj;

    // NOT IMPLEMENTED ----------------------------------------------------------------------------
private:

    CRSTransformer(const CRSTransformer&); ///< Not implemented
    const CRSTransformer& operator=(const CRSTransformer&); ///< Not implemented

}; // CRSTransformer

#endif // geomodelgrids_utils_crstransform_hh

// End of file
