/** Points sampling the models in the tests/testdata directory.
 */

#if !defined(geomodelgrids_serial_modelpoints_hh)
#define geomodelgrids_serial_modelpoints_hh

#include <cstddef> // USES size_t

namespace geomodelgrids {
    namespace testdata {
        class ModelPoints;
        class OneBlockFlatPoints;
        class OneBlockFlatBorehole;
        class OneBlockFlatIsosurface;
        class OneBlockTopoPoints;
        class OneBlockSquashPoints;
        class ThreeBlocksFlatPoints;
        class ThreeBlocksTopoPoints;
        class ThreeBlocksTopoBorehole;
        class ThreeBlocksTopoIsosurface;
        class ThreeBlocksSquashTopPoints;
        class ThreeBlocksSquashTopoBathyPoints;
        class OutsideDomainPoints;
    } // testdata
} // geomodelgrids

class geomodelgrids::testdata::ModelPoints {
    // PUBLIC STRUCT ------------------------------------------------------------------------------
public:

    struct Domain {
        double originX;
        double originY;
        double yAzimuth;
        double zBottom;
        bool hasTopSurface;
        bool hasTopoBathy;

        Domain(void) :
            originX(0.0),
            originY(0.0),
            yAzimuth(0.0),
            zBottom(0.0),
            hasTopSurface(false),
            hasTopoBathy(false) {}


    };

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Default constructor.
    ModelPoints(const size_t numPoints);

    /// Destructor.
    ~ModelPoints(void);

    /** Get domain metadata.
     *
     * @returns Domain metadata.
     */
    const Domain& getDomain(void) const;

    /** Get number of points.
     *
     * @returns Number of sample points.
     */
    size_t getNumPoints(void) const;

    /** Get CRS for longitude, latitude, and elevation.
     *
     * @returns String for CRS.
     */
    const char* getCRSLatLonElev(void) const;

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

    /** Compute elevation of top surface at point.
     *
     * @param[in] x X coordinate of point.
     * @parma[in] y Y coordinate of point.
     * @returns Elevation (m) of top surface.
     */
    static
    double computeTopElevation(const double x,
                               const double y);

    /** Compute elevation of topography/bathymetry at point.
     *
     * @param[in] x X coordinate of point.
     * @parma[in] y Y coordinate of point.
     * @returns Elevation (m) of topography/bathymetry surface.
     */
    static
    double computeTopoBathyElevation(const double x,
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

    /** Compute elevation of isosurface for value 'one' at point.
     *
     * @param[in] x X coordinate of point.
     * @parma[in] y Y coordinate of point.
     * @param[in] isoValue Isosurface value.
     * @param[in] zTop Elevation of top surface.
     * @param[in] zBottom Elevation of domain bottom.
     * @returns z Elevation of 'one' isosurface at point.
     */
    static
    double computeIsosurfaceOne(const double x,
                                const double y,
                                const double isoValue,
                                const double zTop,
                                const double zBottom);

    /** Compute elevation of isosurface for value 'two' at point.
     *
     * @param[in] x X coordinate of point.
     * @parma[in] y Y coordinate of point.
     * @param[in] isoValue Isosurface value.
     * @param[in] zTop Elevation of top surface.
     * @param[in] zBottom Elevation of domain bottom.
     * @returns z Elevation of 'two' isosurface at point.
     */
    static
    double computeIsosurfaceTwo(const double x,
                                const double y,
                                const double isoValue,
                                const double zTop,
                                const double zBottom);

    // PROTECTED MEMBERS -----------------------------------------------------------------------------------------------
protected:

    Domain _domain;
    const double* _pointsLLE;
    double*  _pointsXYZ;
    const size_t _numPoints;
    const char* _inCRS;
    const char* _modelCRS;

}; // ModelPoints

class geomodelgrids::testdata::OneBlockFlatPoints : public ModelPoints {
public:

    OneBlockFlatPoints(void);
}; // OneBlockFlatPoints

class geomodelgrids::testdata::OneBlockFlatBorehole : public ModelPoints {
public:

    OneBlockFlatBorehole(void);
}; // OneBlockFlatBorehole

class geomodelgrids::testdata::OneBlockFlatIsosurface : public ModelPoints {
public:

    OneBlockFlatIsosurface(void);
}; // OneBlockFlatIsosurface

class geomodelgrids::testdata::OneBlockTopoPoints : public ModelPoints {
public:

    OneBlockTopoPoints(void);
}; // OneBlockTopoPoints

class geomodelgrids::testdata::OneBlockSquashPoints : public ModelPoints {
public:

    OneBlockSquashPoints(const double squashMinElev);
}; // OneBlockSquashPoints

class geomodelgrids::testdata::ThreeBlocksFlatPoints : public ModelPoints {
public:

    ThreeBlocksFlatPoints(void);
}; // ThreeBlocksFlatPoints

class geomodelgrids::testdata::ThreeBlocksTopoPoints : public ModelPoints {
public:

    ThreeBlocksTopoPoints(void);
}; // ThreeBlocksTopoPoints

class geomodelgrids::testdata::ThreeBlocksTopoBorehole : public ModelPoints {
public:

    ThreeBlocksTopoBorehole(void);
}; // ThreeBlocksTopoBorehole

class geomodelgrids::testdata::ThreeBlocksTopoIsosurface : public ModelPoints {
public:

    ThreeBlocksTopoIsosurface(void);
}; // ThreeBlocksTopoIsosurface

class geomodelgrids::testdata::ThreeBlocksSquashTopPoints : public ModelPoints {
public:

    ThreeBlocksSquashTopPoints(const double squashMinElev);
}; // ThreeBlocksSquashPoints

class geomodelgrids::testdata::ThreeBlocksSquashTopoBathyPoints : public ModelPoints {
public:

    ThreeBlocksSquashTopoBathyPoints(const double squashMinElev);
}; // ThreeBlocksSquashPoints

class geomodelgrids::testdata::OutsideDomainPoints : public ModelPoints {
public:

    OutsideDomainPoints(void);
}; // OutsideDomainPoints

#endif // geomodelgrids_serial_modelpoints_hh

// End of file
