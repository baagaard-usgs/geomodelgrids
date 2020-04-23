/** Transform from one georeferenced coordinate system to another.
 */

#if !defined(geomodelgrids_utils_crstransform_hh)
#define geomodelgrids_utils_crstransform_hh

#include "utilsfwd.hh" // forward declarations

#include "proj.h" // HOLDSA PJ

#include <string> // HASA std::string

class geomodelgrids::utils::CRSTransformer {
    friend class TestCRSTransformer; // Unit testing

    // PUBLIC METHODS --------------------------------------------------------------------------------------------------
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
     * @param[in] srcX X coordinate in source coordinate system.
     * @param[in] srcY Y coordinate in source coordinate system.
     */
    void transform(double* destX,
                   double* destY,
                   const double srcX,
                   const double srcY);

    // PRIVATE MEMBERS -------------------------------------------------------------------------------------------------
private:

    std::string _srcString;
    std::string _destString;
    PJ* _proj;

}; // CRSTransformer

#endif // geomodelgrids_utils_crstransform_hh

// End of file
