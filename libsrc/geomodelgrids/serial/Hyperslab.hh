/** Hyperslab for a chunk of data in an HDF5 file.
 *
 * The hyperslab always contains all of the values at a point and that dimension is not given in the constructor.
 */

#if !defined(geomodelgrids_serial_hyperslab_hh)
#define geomodelgrids_serial_hyperslab_hh

#include "serialfwd.hh" // forward declarations

#include <cstdlib> // USES size_t
#include <hdf5.h> // USES hsize_t
#include <string> // USES std::string

// Forward declarations of helper classes.
namespace geomodelgrids {
    namespace serial {
        class _Hyperslab;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::Hyperslab {
    friend class _Hyperslab; // Helper class for getting slab.
    friend class TestHyperslab; // Unit testing

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /** Constructor.
     *
     * @param[in] h5 HDF5 with model.
     * @param[in] path Full path to dataset.
     * @param[in] dims Array of hyperslab dimensions.
     * @param[in] ndims Number of dimensions in hyperslab.
     */
    Hyperslab(geomodelgrids::serial::HDF5* const h5,
              const char* path,
              const hsize_t dims[],
              const size_t ndims);

    /// Destructor
    ~Hyperslab(void);

    /** Compute values at point using bilinear interpolation.
     *
     * @param[out] values Preallocated array for interpolated values.
     * @param[in] indexFloat Index of target point as floating point values.
     */
    void interpolate(double* const values,
                     const double indexFloat[]);

    /** Get values at nearest point.
     *
     * @param[out] values Preallocated array for values.
     * @param[in] indexFloat Index of target point as floating point values.
     */
    void nearest(double* const values,
                 const double indexFloat[]);

    // PRIVATE MEMBERS ----------------------------------------------------------------------------
private:

    geomodelgrids::serial::HDF5* const _h5; ///< HDF5 data.
    const std::string _datasetPath; ///< Full path to dataset.

    const size_t _ndims; ///< Number of dimensions in hyperslab.
    hsize_t* _origin; ///< Origin of hyperslab relative to dataset.
    hsize_t* _dims; ///< Dimensions of hyperslab.
    hsize_t* _dimsAll; ///< Dimensions of entire dataset.
    double* _values; ///< Hyperslab values.

    geomodelgrids::serial::_Hyperslab* _hyperslab; ///< Helper object.

    // NOT IMPLEMENTED ----------------------------------------------------------------------------
private:

    Hyperslab(const Hyperslab&); ///< Not implemented
    const Hyperslab& operator=(const Hyperslab&); ///< Not implemented

}; // Hyperslab

#endif // geomodelgrids_serial_hyperslab_hh

// End of file
