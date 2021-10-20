#include <portinfo>

#include "Indexing.hh" // implementation of class methods

#include <algorithm> // USES std::sort()
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ------------------------------------------------------------------------------------------------
geomodelgrids::utils::Indexing::Indexing(void) {}


// ------------------------------------------------------------------------------------------------
geomodelgrids::utils::Indexing::~Indexing(void) {}


// ------------------------------------------------------------------------------------------------
geomodelgrids::utils::IndexingUniform::IndexingUniform(const double dx) :
    _dx(dx) {
    if (dx <= 0.0) {
        std::ostringstream msg;
        msg << "Non-positive value (" << dx << ") for uniform resolution indexing.";
        throw std::invalid_argument(msg.str());
    } // if
}


// ------------------------------------------------------------------------------------------------
double
geomodelgrids::utils::IndexingUniform::getIndex(const double x) const {
    assert(_dx > 0.0);
    return x / _dx;
}


// ------------------------------------------------------------------------------------------------
geomodelgrids::utils::IndexingVariable::IndexingVariable(const double* x,
                                                         const size_t numX,
                                                         SortOrder sortOrder) :
    _numX(numX) {
    if (!x) {
        std::ostringstream msg;
        msg << "NULL coordinates array for variable resolution indexing.";
        throw std::invalid_argument(msg.str());
    } // if
    if (!numX) {
        std::ostringstream msg;
        msg << "Zero length coordinates array for variable resolution indexing.";
        throw std::invalid_argument(msg.str());
    } // if

    _x = (numX > 0) ? new double[numX] : NULL;
    for (size_t i = 0; i < _numX; ++i) {
        _x[i] = x[i];
    } // for

    _order = sortOrder;
    if (ASCENDING == sortOrder) {
        std::sort(_x, _x + _numX);
    } else {
        std::sort(_x, _x + _numX, greater);
    } // if/else
}


// ------------------------------------------------------------------------------------------------
geomodelgrids::utils::IndexingVariable::~IndexingVariable(void) {
    delete[] _x;_x = NULL;
}


// ------------------------------------------------------------------------------------------------
double
geomodelgrids::utils::IndexingVariable::getIndex(const double x) const {
    assert(_x);

    double index = -1.0;
    size_t indexL = 0;
    size_t indexR = _numX - 1;
    const double xN = (_order == ASCENDING) ? _x[0] + x : _x[0] - x;
    const double tolerance = 1.0e-6;
    assert((ASCENDING == _order && (xN >= _x[indexL]-tolerance) && (xN <= _x[indexR]+tolerance)) ||
           (DESCENDING == _order && (xN <= _x[indexL]+tolerance) && (xN >= _x[indexR]-tolerance)));

    typedef bool (*cmp_fn)(const double,
                           const double);
    cmp_fn compare = (ASCENDING == _order) ? less : greater;

    while (indexR - indexL > 1) {
        size_t indexM = indexL + (indexR-indexL) / 2;
        if (compare(xN, _x[indexM])) {
            indexR = indexM;
        } else {
            indexL = indexM;
        } // if/else
    } // while
    assert((ASCENDING == _order && xN >= _x[indexL]-tolerance && _x[indexR] > _x[indexL]) ||
           (DESCENDING == _order && xN <= _x[indexL]+tolerance && _x[indexR] <= _x[indexL]));

    index = double(indexL) + (xN - _x[indexL]) / (_x[indexR] - _x[indexL]);
    return index;
}


// End of file
