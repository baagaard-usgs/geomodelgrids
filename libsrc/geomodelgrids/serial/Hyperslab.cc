#include <portinfo>

#include "Hyperslab.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()
#include <cmath> // USES floor()
#include <algorithm> // USES std::min(), std::max()

#if !defined(CALL_MEMBER_FN)
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))
#endif

// ------------------------------------------------------------------------------------------------
class geomodelgrids::serial::_Hyperslab {
public:

    /** Constructor.
     *
     * * @param[in] Hyperslab
     */
    _Hyperslab(Hyperslab& hyperslab);

    /// Destructor.
    ~_Hyperslab(void);

    /** Get values for hyperslab containing target point.
     *
     * Use current hyperslab if possible.
     *
     * @param[in] indexFloat Floating point index of target point.
     */
    void getSlab(const double indexFloat[]);

    /** Compute values at point using bilinear interpolation.
     *
     * @param[out] values Preallocated array for interpolated values.
     * @param[in] indexFloat Floating point index of target point.
     */
    void interpolate(double* const values,
                     const double indexFloat[]);

    /** Get values at nearest point.
     *
     * @param[out] values Preallocated array for interpolated values.
     * @param[in] indexFloat Floating point index of target point.
     */
    void nearest(double* const values,
                 const double indexFloat[]);

private:

    typedef void (_Hyperslab::*interpolate_fn_type)(double* const values,
                                                    const double indexFloat[]);

    /** Compute values at point using bilinear interpolation in 2-D.
     *
     * @param[out] values Preallocated array for interpolated values.
     * @param[in] indexFloat Floating point index of target point.
     */
    void _interpolate2D(double* const values,
                        const double indexFloat[]);

    /** Compute values at point using bilinear interpolation in 3-D.
     *
     * @param[out] values Preallocated array for interpolated values.
     * @param[in] indexFloat Floating point index of target point.
     */
    void _interpolate3D(double* const values,
                        const double indexFloat[]);

    /** Get nearest values in 2-D.
     *
     * @param[out] values Preallocated array for interpolated values.
     * @param[in] indexFloat Floating point index of target point.
     */
    void _nearest2D(double* const values,
                    const double indexFloat[]);

    /** Get nearest values in 3-D.
     *
     * @param[out] values Preallocated array for interpolated values.
     * @param[in] indexFloat Floating point index of target point.
     */
    void _nearest3D(double* const values,
                    const double indexFloat[]);

    geomodelgrids::serial::Hyperslab& _hyperslab; ///< Reference to hyperslab.
    interpolate_fn_type _interpolate; ///< Function for interpolation.
    interpolate_fn_type _nearest; ///< Function for nearest.

}; // _Hyperslab

// ------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Hyperslab::Hyperslab(geomodelgrids::serial::HDF5* const h5,
                                            const char* path,
                                            const hsize_t dims[],
                                            const size_t ndims) :
    _h5(h5),
    _datasetPath(path),
    _ndims(ndims),
    _origin(NULL),
    _dims(_ndims > 0 ? new hsize_t[_ndims] : NULL),
    _dimsAll(NULL),
    _values(NULL),
    _hyperslab(NULL) {
    assert(_h5);
    int ndimsAll = 0;
    h5->getDatasetDims(&_dimsAll, &ndimsAll, path);

    assert(ndimsAll >= 0);
    if (_ndims != size_t(ndimsAll)) {
        std::ostringstream msg;
        msg << "Dimensions of hyperslab for dataset '" << path << "' have different rank than the dataset. "
            << "Hyperslab has rank " << _ndims << ", but dataset has rank " << ndimsAll << ".";
        delete[] _dims;_dims = NULL;
        delete[] _dimsAll;_dimsAll = NULL;
        throw std::length_error(msg.str());
    } // if

    hsize_t totalSize = 1;
    for (size_t i = 0; i < ndims; ++i) {
        _dims[i] = std::min(dims[i], _dimsAll[i]);
        totalSize *= _dims[i];
    } // for
    _values = (totalSize > 0) ? new double[totalSize] : NULL;

    delete _hyperslab;_hyperslab = new geomodelgrids::serial::_Hyperslab(*this);
} // constructor


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Hyperslab::~Hyperslab(void) {
    delete[] _origin;_origin = NULL;
    delete[] _dims;_dims = NULL;
    delete[] _dimsAll;_dimsAll = NULL;
    delete[] _values;_values = NULL;

    delete _hyperslab;_hyperslab = NULL;
} // destructor


// ------------------------------------------------------------------------------------------------
// Compute values at point using bilinear interpolation.
void
geomodelgrids::serial::Hyperslab::interpolate(double* const values,
                                              const double indexFloat[]) {
    assert(_hyperslab);
    _hyperslab->getSlab(indexFloat);
    _hyperslab->interpolate(values, indexFloat);
} // interpolate


// ------------------------------------------------------------------------------------------------
// Get values at nearest point.
void
geomodelgrids::serial::Hyperslab::nearest(double* const values,
                                          const double indexFloat[]) {
    assert(_hyperslab);
    _hyperslab->getSlab(indexFloat);
    _hyperslab->nearest(values, indexFloat);
} // nearest


// ------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::_Hyperslab::_Hyperslab(geomodelgrids::serial::Hyperslab& hyperslab) :
    _hyperslab(hyperslab) {
    if (3 == hyperslab._ndims-1) {
        _interpolate = &geomodelgrids::serial::_Hyperslab::_interpolate3D;
        _nearest = &geomodelgrids::serial::_Hyperslab::_nearest3D;
    } else if (2 == hyperslab._ndims-1) {
        _interpolate = &geomodelgrids::serial::_Hyperslab::_interpolate2D;
        _nearest = &geomodelgrids::serial::_Hyperslab::_nearest2D;
    } else {
        std::ostringstream msg;
        msg << "Expect ndims (" << hyperslab._ndims << ") to be 2 or 3 in geomodelgrids::serial::Hyperslab.";
        throw std::domain_error(msg.str());
    } // if/else
} // constructor


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::_Hyperslab::~_Hyperslab(void) {}


// ------------------------------------------------------------------------------------------------
// Get values for hyperslab containing target point.
void
geomodelgrids::serial::_Hyperslab::getSlab(const double indexFloat[]) {
    const size_t ndims = _hyperslab._ndims;
    hsize_t* origin = _hyperslab._origin;
    const hsize_t* dims = _hyperslab._dims;
    const hsize_t* dimsAll = _hyperslab._dimsAll;

    bool needsNewSlab = false;
    const size_t spaceDim = ndims - 1; // last dimension is values
    if (origin) {
        for (size_t i = 0; i < spaceDim; ++i) {
            if (( indexFloat[i] - double(origin[i]) < 0.0) ||
                ( indexFloat[i] >= double(origin[i]+dims[i]-1)) ) {
                needsNewSlab = true;
                break;
            } // if
        } // for
    } else {
        origin = _hyperslab._origin = (ndims > 0) ? new hsize_t[ndims] : NULL;
        std::fill(&origin[0], &origin[ndims], 0);
        needsNewSlab = true;
    } // if/else

    if (needsNewSlab) {
        // Get hyperslab with target point in the center.
        for (size_t i = 0; i < spaceDim; ++i) {
            hsize_t index = (indexFloat[i] >= dims[i]-1) ? hsize_t(std::floor(indexFloat[i] - (dims[i]-1)/ 2)) : 0;
            index = std::min(index, dimsAll[i]-dims[i]);
            origin[i] = index;
        } // for

        _hyperslab._h5->readDatasetHyperslab(_hyperslab._values, _hyperslab._datasetPath.c_str(), origin, dims, ndims,
                                             H5T_NATIVE_DOUBLE);
    } // if
} // getSlab


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_Hyperslab::interpolate(double* const values,
                                               const double indexFloat[]) {
    assert(_interpolate);
    CALL_MEMBER_FN(*this, _interpolate)(values, indexFloat);
} // interpolate


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_Hyperslab::nearest(double* const values,
                                           const double indexFloat[]) {
    assert(_nearest);
    CALL_MEMBER_FN(*this, _nearest)(values, indexFloat);
} // nearest


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_Hyperslab::_interpolate2D(double* const values,
                                                  const double indexFloat[]) {
    assert(values);
    assert(indexFloat);
    assert(_hyperslab._values);
    assert(_hyperslab._origin);

    const size_t spaceDim = 2;

    // Coordinates within hyperslab
    const double indexSlab[spaceDim] = {
        indexFloat[0] - _hyperslab._origin[0],
        indexFloat[1] - _hyperslab._origin[1],
    };
    assert(indexSlab[0] >= 0.0 && indexSlab[0] <= _hyperslab._dims[0]-1);
    assert(indexSlab[1] >= 0.0 && indexSlab[1] <= _hyperslab._dims[1]-1);

    // Coordinate of "lower" point (corner of cell with lowest indices containing target point).
    const double tolerance = 1.0e-12;
    const double dfloor[spaceDim] = {
        std::max(0.0, std::floor(indexSlab[0]-tolerance)),
        std::max(0.0, std::floor(indexSlab[1]-tolerance)),
    };

    // Index of "lower" point
    const hsize_t ifloor[spaceDim] = {
        hsize_t(dfloor[0]),
        hsize_t(dfloor[1]),
    };
    assert(ifloor[0] < _hyperslab._dims[0]);
    assert(ifloor[1] < _hyperslab._dims[1]);

    // Coordinates within cell relative to "lower" point.
    const double xRef[spaceDim] = {
        indexSlab[0] - dfloor[0],
        indexSlab[1] - dfloor[1],
    };

    const double wts[2][2] = {
        {
            (1.0 - xRef[0]) * (1.0 - xRef[1]),
            (1.0 - xRef[0]) * xRef[1],
        },{
            xRef[0] * (1.0 - xRef[1]),
            xRef[0] * xRef[1],
        },
    };

    // Indices into hyperslab values for cell corners
    const hsize_t* dims = _hyperslab._dims;
    const hsize_t ii[2][2] = {
        {
            (ifloor[0]+0)*(dims[1]*dims[2]) + (ifloor[1]+0)*(dims[2]),
            (ifloor[0]+0)*(dims[1]*dims[2]) + (ifloor[1]+1)*(dims[2]),
        },{
            (ifloor[0]+1)*(dims[1]*dims[2]) + (ifloor[1]+0)*(dims[2]),
            (ifloor[0]+1)*(dims[1]*dims[2]) + (ifloor[1]+1)*(dims[2]),
        },
    };

    const hsize_t numValues = _hyperslab._dims[spaceDim];
    for (hsize_t iValue = 0; iValue < numValues; ++iValue) {
        values[iValue] = 0;
        for (hsize_t iDim = 0; iDim < 2; ++iDim) {
            for (hsize_t jDim = 0; jDim < 2; ++jDim) {
                values[iValue] += wts[iDim][jDim] * _hyperslab._values[ii[iDim][jDim] + iValue];
            } // for
        } // for
    } // for

} // interpolate2D


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_Hyperslab::_interpolate3D(double* const values,
                                                  const double indexFloat[]) {
    assert(values);
    assert(indexFloat);
    assert(_hyperslab._values);
    assert(_hyperslab._dimsAll);
    assert(_hyperslab._origin);

    const size_t spaceDim = 3;

    // Coordinates within hyperslab
    const double indexSlab[spaceDim] = {
        indexFloat[0] - _hyperslab._origin[0],
        indexFloat[1] - _hyperslab._origin[1],
        indexFloat[2] - _hyperslab._origin[2],
    };
    assert(indexSlab[0] >= 0.0 && indexSlab[0] <= _hyperslab._dims[0]-1);
    assert(indexSlab[1] >= 0.0 && indexSlab[1] <= _hyperslab._dims[1]-1);
    assert(indexSlab[2] >= 0.0 && indexSlab[2] <= _hyperslab._dims[2]-1);

    // Coordinate of "lower" point (corner of cell with lowest indices containing target point).
    const double tolerance = 1.0e-12;
    const double dfloor[spaceDim] = {
        std::max(0.0, std::floor(indexSlab[0]-tolerance)),
        std::max(0.0, std::floor(indexSlab[1]-tolerance)),
        std::max(0.0, std::floor(indexSlab[2]-tolerance)),
    };

    // Index of "lower" point
    const hsize_t ifloor[spaceDim] = {
        hsize_t(dfloor[0]),
        hsize_t(dfloor[1]),
        hsize_t(dfloor[2]),
    };
    assert(ifloor[0] < _hyperslab._dims[0]);
    assert(ifloor[1] < _hyperslab._dims[1]);
    assert(ifloor[2] < _hyperslab._dims[2]);

    // Coordinates within cell relative to "lower" point.
    const double xRef[spaceDim] = {
        indexSlab[0] - dfloor[0],
        indexSlab[1] - dfloor[1],
        indexSlab[2] - dfloor[2],
    };

    const double wts[2][2][2] = {
        {
            {
                (1.0 - xRef[0]) * (1.0 - xRef[1]) * (1.0 - xRef[2]),
                (1.0 - xRef[0]) * (1.0 - xRef[1]) * xRef[2],
            },{
                (1.0 - xRef[0]) * xRef[1] * (1.0 - xRef[2]),
                (1.0 - xRef[0]) * xRef[1] * xRef[2],
            },
        },{
            {
                xRef[0] * (1.0 - xRef[1]) * (1.0 - xRef[2]),
                xRef[0] * (1.0 - xRef[1]) * xRef[2],
            },{
                xRef[0] * xRef[1] * (1.0 - xRef[2]),
                xRef[0] * xRef[1] * xRef[2],
            },
        },
    };

    // Indices into hyperslab values for cell corners
    const hsize_t* dims = _hyperslab._dims;
    const hsize_t ii[2][2][2] = {
        {
            {
                (ifloor[0]+0)*(dims[1]*dims[2]*dims[3]) + (ifloor[1]+0)*(dims[2]*dims[3]) + (ifloor[2]+0)*(dims[3]),
                (ifloor[0]+0)*(dims[1]*dims[2]*dims[3]) + (ifloor[1]+0)*(dims[2]*dims[3]) + (ifloor[2]+1)*(dims[3]),
            },{
                (ifloor[0]+0)*(dims[1]*dims[2]*dims[3]) + (ifloor[1]+1)*(dims[2]*dims[3]) + (ifloor[2]+0)*(dims[3]),
                (ifloor[0]+0)*(dims[1]*dims[2]*dims[3]) + (ifloor[1]+1)*(dims[2]*dims[3]) + (ifloor[2]+1)*(dims[3]),
            },
        },{
            {
                (ifloor[0]+1)*(dims[1]*dims[2]*dims[3]) + (ifloor[1]+0)*(dims[2]*dims[3]) + (ifloor[2]+0)*(dims[3]),
                (ifloor[0]+1)*(dims[1]*dims[2]*dims[3]) + (ifloor[1]+0)*(dims[2]*dims[3]) + (ifloor[2]+1)*(dims[3]),
            },{
                (ifloor[0]+1)*(dims[1]*dims[2]*dims[3]) + (ifloor[1]+1)*(dims[2]*dims[3]) + (ifloor[2]+0)*(dims[3]),
                (ifloor[0]+1)*(dims[1]*dims[2]*dims[3]) + (ifloor[1]+1)*(dims[2]*dims[3]) + (ifloor[2]+1)*(dims[3]),
            },
        },
    };

    const hsize_t numValues = _hyperslab._dims[spaceDim];
    for (hsize_t iValue = 0; iValue < numValues; ++iValue) {
        values[iValue] = 0;
        bool hasNoDataValue = false;
        for (hsize_t iDim = 0; iDim < 2; ++iDim) {
            for (hsize_t jDim = 0; jDim < 2; ++jDim) {
                for (hsize_t kDim = 0; kDim < 2; ++kDim) {
                    const double interpolateValue = _hyperslab._values[ii[iDim][jDim][kDim] + iValue];
                    if (fabs(1.0 - interpolateValue/geomodelgrids::NODATA_VALUE) < 1.0e-3) {
                        hasNoDataValue = true;
                    } // if
                    values[iValue] += wts[iDim][jDim][kDim] * interpolateValue;
                } // for
            } // for
        } // for
        if (hasNoDataValue) {
            // Set value to NODATA_VALUE if any values used in interpolation are NODATA_VALUE.
            values[iValue] = geomodelgrids::NODATA_VALUE;
        } // if
    } // for

} // _interpolate3D


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_Hyperslab::_nearest2D(double* const values,
                                              const double indexFloat[]) {
    assert(values);
    assert(indexFloat);
    assert(_hyperslab._values);
    assert(_hyperslab._dimsAll);
    assert(_hyperslab._origin);

    const size_t spaceDim = 2;

    // Coordinates within hyperslab
    const double indexSlab[spaceDim] = {
        indexFloat[0] - _hyperslab._origin[0],
        indexFloat[1] - _hyperslab._origin[1],
    };
    assert(indexSlab[0] >= 0.0 && indexSlab[0] <= _hyperslab._dims[0]-1);
    assert(indexSlab[1] >= 0.0 && indexSlab[1] <= _hyperslab._dims[1]-1);

    // Coordinate of nearest point to target point.
    const double dnearest[spaceDim] = {
        std::round(indexSlab[0]),
        std::round(indexSlab[1]),
    };

    // Index of nearest point
    const hsize_t inearest[spaceDim] = {
        hsize_t(dnearest[0]),
        hsize_t(dnearest[1]),
    };
    assert(inearest[0] < _hyperslab._dims[0]);
    assert(inearest[1] < _hyperslab._dims[1]);

    // Indices into hyperslab values for nearest point.
    const hsize_t* dims = _hyperslab._dims;
    const hsize_t ii = inearest[0]*(dims[1]*dims[2]) + inearest[1]*(dims[2]);

    const hsize_t numValues = _hyperslab._dims[spaceDim];
    for (hsize_t iValue = 0; iValue < numValues; ++iValue) {
        values[iValue] = 0;
        const double nearestValue = _hyperslab._values[ii + iValue];
        if (fabs(1.0 - nearestValue/geomodelgrids::NODATA_VALUE) < 1.0e-3) {
            values[iValue] = geomodelgrids::NODATA_VALUE;
        } // if
    } // for

} // _nearest2D


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_Hyperslab::_nearest3D(double* const values,
                                              const double indexFloat[]) {
    assert(values);
    assert(indexFloat);
    assert(_hyperslab._values);
    assert(_hyperslab._dimsAll);
    assert(_hyperslab._origin);

    const size_t spaceDim = 3;

    // Coordinates within hyperslab
    const double indexSlab[spaceDim] = {
        indexFloat[0] - _hyperslab._origin[0],
        indexFloat[1] - _hyperslab._origin[1],
        indexFloat[2] - _hyperslab._origin[2],
    };
    assert(indexSlab[0] >= 0.0 && indexSlab[0] <= _hyperslab._dims[0]-1);
    assert(indexSlab[1] >= 0.0 && indexSlab[1] <= _hyperslab._dims[1]-1);
    assert(indexSlab[2] >= 0.0 && indexSlab[2] <= _hyperslab._dims[2]-1);

    // Coordinate of nearest point to target point.
    const double dnearest[spaceDim] = {
        std::round(indexSlab[0]),
        std::round(indexSlab[1]),
        std::round(indexSlab[2]),
    };

    // Index of nearest point
    const hsize_t inearest[spaceDim] = {
        hsize_t(dnearest[0]),
        hsize_t(dnearest[1]),
        hsize_t(dnearest[2]),
    };
    assert(inearest[0] < _hyperslab._dims[0]);
    assert(inearest[1] < _hyperslab._dims[1]);
    assert(inearest[2] < _hyperslab._dims[2]);

    // Indices into hyperslab values for nearest point.
    const hsize_t* dims = _hyperslab._dims;
    const hsize_t ii =
        inearest[0]*(dims[1]*dims[2]*dims[3]) + inearest[1]*(dims[2]*dims[3]) + inearest[2]*(dims[3]);

    const hsize_t numValues = _hyperslab._dims[spaceDim];
    for (hsize_t iValue = 0; iValue < numValues; ++iValue) {
        values[iValue] = 0;
        const double nearestValue = _hyperslab._values[ii + iValue];
        if (fabs(1.0 - nearestValue/geomodelgrids::NODATA_VALUE) < 1.0e-3) {
            values[iValue] = geomodelgrids::NODATA_VALUE;
        } // if
    } // for

} // _nearest3D


// End of file
