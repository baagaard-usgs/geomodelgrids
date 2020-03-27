#include <portinfo>

#include "Hyperslab.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()
#include <cmath> // USES floor()
#include <algorithm> // USES std::min(), std::max()

#if !defined(CALL_MEMBER_FN)
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))
#endif

// ---------------------------------------------------------------------------------------------------------------------
class geomodelgrids::serial::_Hyperslab {
public:

    /** Constructor.
     *
     * * @param[in] Hyperslab
     */
    _Hyperslab(Hyperslab& hyperslab);

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

    geomodelgrids::serial::Hyperslab& _hyperslab;
    interpolate_fn_type _interpolate; ///< Function for interpolation.

}; // _Hyperslab

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Hyperslab::Hyperslab(geomodelgrids::serial::HDF5* const h5,
                                            const char* path,
                                            size_t dims[],
                                            const size_t ndims) :
    _h5(h5),
    _datasetPath(path),
    _ndims(ndims+1),
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
        throw std::runtime_error(msg.str());
    } // if

    for (size_t i = 0; i < ndims; ++i) {
        _dims[i] = dims[i];
    } // for
    _dims[ndims] = _dimsAll[ndims];

    // :TODO: Limit dims to be smaller than dimsAll.

    delete _hyperslab;_hyperslab = new geomodelgrids::serial::_Hyperslab(*this);
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Hyperslab::~Hyperslab(void) {
    delete[] _origin;_origin = NULL;
    delete[] _dims;_dims = NULL;
    delete[] _dimsAll;_dimsAll = NULL;
    delete[] _values;_values = NULL;

    delete _hyperslab;_hyperslab = NULL;
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Compute values at point using bilinear interpolation.
void
geomodelgrids::serial::Hyperslab::interpolate(double* const values,
                                              const double indexFloat[]) {
    assert(_hyperslab);
    _hyperslab->getSlab(indexFloat);
    _hyperslab->interpolate(values, indexFloat);
} // interpolate


// ---------------------------------------------------------------------------------------------------------------------
// Constructor.
geomodelgrids::serial::_Hyperslab::_Hyperslab(geomodelgrids::serial::Hyperslab& hyperslab) :
    _hyperslab(hyperslab) {
    if (3 == hyperslab._ndims) {
        _interpolate = &geomodelgrids::serial::_Hyperslab::_interpolate3D;
    } else if (2 == hyperslab._ndims) {
        _interpolate = &geomodelgrids::serial::_Hyperslab::_interpolate2D;
    } else {
        std::ostringstream msg;
        msg << "Expect ndims (" << hyperslab._ndims << ") to be 2 or 3 in geomodelgrids::serial::Hyperslab.";
        throw std::logic_error(msg.str());
    } // if/else
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
geomodelgrids::serial::_Hyperslab::~_Hyperslab(void) {}


// ---------------------------------------------------------------------------------------------------------------------
// Get values for hyperslab containing target point.
void
geomodelgrids::serial::_Hyperslab::getSlab(const double indexFloat[]) {
    const hsize_t ndims = _hyperslab._ndims;
    hsize_t* origin = _hyperslab._origin;
    const hsize_t* dims = _hyperslab._dims;
    const hsize_t* dimsAll = _hyperslab._dimsAll;

    bool needsNewSlab = true;
    if (origin) {
        for (hsize_t i = 0; i < ndims; ++i) {
            if (( indexFloat[0] - double(origin[i]) < 0.0) ||
                ( indexFloat[0] >= double(origin[i]+dims[i])) ) {
                needsNewSlab = true;
            } // if
        } // for
    } else {
        _hyperslab._origin = (ndims > 0) ? new hsize_t[ndims] : NULL;
        origin = _hyperslab._origin;
    } // if/else

    if (needsNewSlab) {
        // Get hyperslab with target point in the center.
        for (hsize_t i = 0; i < ndims; ++i) {
            hsize_t index = (indexFloat[i] > dims[i]) ? hsize_t(std::floor(indexFloat[i])) - dims[i] / 2 : 0;
            index = std::min(index, dimsAll[i]-dims[i]);
            origin[i] = index;
        } // for

        // _hyperslab._h5->readDatasetHyperslab(&_hyperslab.values, _hyperslab._path, origin, dims, datatype);
    } // if
} // getSlab


// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_Hyperslab::interpolate(double* const values,
                                               const double indexFloat[]) {
    assert(_interpolate);
    CALL_MEMBER_FN(*this, _interpolate)(values, indexFloat);
} // interpolate


// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_Hyperslab::_interpolate2D(double* const values,
                                                  const double indexFloat[]) {
    assert(values);
    assert(indexFloat);
    assert(_hyperslab._values);
    assert(_hyperslab._dimsAll);
    assert(_hyperslab._origin);

    const size_t spaceDim = 2;

    // Coordinates within hyperslab
    const double indexSlab[spaceDim] = {
        indexFloat[0] - _hyperslab._origin[0]*_hyperslab._dimsAll[0],
        indexFloat[1] - _hyperslab._origin[1]*_hyperslab._dimsAll[1],
    };
    assert(indexSlab[0] >= 0.0);
    assert(indexSlab[1] >= 0.0);

    // Coordinate of "lower" point (corner of cell with lowest indices containing target point).
    const double dfloor[spaceDim] = {
        std::floor(indexSlab[0]),
        std::floor(indexSlab[1]),
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
                values[iValue] += wts[iDim][jDim] * values[ii[iDim][jDim] + iValue];
            } // for
        } // for
    } // for

} // interpolate3D


// ---------------------------------------------------------------------------------------------------------------------
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
        indexFloat[0] - _hyperslab._origin[0]*_hyperslab._dimsAll[0],
        indexFloat[1] - _hyperslab._origin[1]*_hyperslab._dimsAll[1],
        indexFloat[2] - _hyperslab._origin[2]*_hyperslab._dimsAll[2],
    };
    assert(indexSlab[0] >= 0.0);
    assert(indexSlab[1] >= 0.0);
    assert(indexSlab[2] >= 0.0);

    // Coordinate of "lower" point (corner of cell with lowest indices containing target point).
    const double dfloor[spaceDim] = {
        std::floor(indexSlab[0]),
        std::floor(indexSlab[1]),
        std::floor(indexSlab[2]),
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
        for (hsize_t iDim = 0; iDim < 2; ++iDim) {
            for (hsize_t jDim = 0; jDim < 2; ++jDim) {
                for (hsize_t kDim = 0; jDim < 2; ++jDim) {
                    values[iValue] += wts[iDim][jDim][kDim] * values[ii[iDim][jDim][kDim] + iValue];
                } // for
            } // for
        } // for
    } // for

} // _interpolate3D


// End of file
