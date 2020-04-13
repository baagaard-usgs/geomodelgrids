/** Points sampling the models in the tests/data directory.
 */

#if !defined(geomodelgrids_serial_modelpoints_hh)
#define geomodelgrids_serial_modelpoints_hh

#include <cstddef> // USES size_t

namespace geomodelgrids {
    namespace serial {
        class ModelPoints;
        class OneBlockFlatPoints;
        class OneBlockTopoPoints;
        class OneBlockSquashPoints;
        class ThreeBlocksFlatPoints;
        class ThreeBlocksTopoPoints;
        class ThreeBlocksSquashPoints;
        class OutsideDomainPoints;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::ModelPoints {
    // PUBLIC METHODS --------------------------------------------------------------------------------------------------
public:

    /// Default constructor.
    ModelPoints(const size_t numPoints);

    /// Destructor.
    ~ModelPoints(void);

    /** Get number of points.
     *
     * @returns Number of sample points.
     */
    size_t getNumPoints(void) const;

    /** Get CRS for longitude, latitude, and elevation=.
     *
     * @returns String for CRS.
     */
    const char* getCRSLonLatElev(void) const;

    /** Get geographic coordinates of sample points.
     *
     * @returns Coordinates (latitude, longitude, elevation) of sample points.
     */
    const double* getLatLonElev(void) const;

    /** Get model coordinates of sample points.
     *
     * @returns Coordinates (x, y, z) of sample points.
     */
    const double* getXYZ(void) const;

    /** Compute elevation of ground surface at point.
     *
     * @param[in] x X coordinate of point.
     * @parma[in] y Y coordinate of point.
     * @returns Elevation (m) of ground surface.
     */
    static
    double computeElevation(const double x,
                            const double y);

    /** Compute value 'one' at point.
     *
     * @param[in] x X coordinate of point.
     * @parma[in] y Y coordinate of point.
     * @parma[in] z Z coordinate of point.
     * @returns Value 'one' at point.
     */
    static
    double computeValueOne(const double x,
                           const double y,
                           const double z);

    /** Compute value 'two' at point.
     *
     * @param[in] x X coordinate of point.
     * @parma[in] y Y coordinate of point.
     * @parma[in] z Z coordinate of point.
     * @returns Value 'two' at point.
     */
    static
    double computeValueTwo(const double x,
                           const double y,
                           const double z);

    // PROTECTED MEMBERS -----------------------------------------------------------------------------------------------
protected:

    const size_t _numPoints;
    const double* _pointsLLE;
    double*  _pointsXYZ;
    const char* _inCRS;
    const char* _modelCRS;

}; // ModelPoints

class geomodelgrids::serial::OneBlockFlatPoints : public ModelPoints {
public:

    OneBlockFlatPoints(void);
}; // OneBlockFlatPoints

class geomodelgrids::serial::OneBlockTopoPoints : public ModelPoints {
public:

    OneBlockTopoPoints(void);
}; // OneBlockTopoPoints

class geomodelgrids::serial::OneBlockSquashPoints : public ModelPoints {
public:

    OneBlockSquashPoints(const double squashMinElev);
}; // OneBlockSquashPoints

class geomodelgrids::serial::ThreeBlocksFlatPoints : public ModelPoints {
public:

    ThreeBlocksFlatPoints(void);
}; // ThreeBlocksFlatPoints

class geomodelgrids::serial::ThreeBlocksTopoPoints : public ModelPoints {
public:

    ThreeBlocksTopoPoints(void);
}; // ThreeBlocksTopoPoints

class geomodelgrids::serial::ThreeBlocksSquashPoints : public ModelPoints {
public:

    ThreeBlocksSquashPoints(const double squashMinElev);
}; // ThreeBlocksSquashPoints

class geomodelgrids::serial::OutsideDomainPoints : public ModelPoints {
public:

    OutsideDomainPoints(void);
}; // OutsideDomainPoints

#endif // geomodelgrids_serial_modelpoints_hh

// End of file
