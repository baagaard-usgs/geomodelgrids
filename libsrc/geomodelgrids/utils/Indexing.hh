/** Transform from one georeferenced coordinate system to another.
 */

#if !defined(geomodelgrids_utils_indexing_hh)
#define geomodelgrids_utils_indexing_hh

#include "utilsfwd.hh" // forward declarations

#include <cstddef> // USES size_t

class geomodelgrids::utils::Indexing {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Constructor.
    Indexing(void);

    /// Destructor.
    virtual ~Indexing(void);

    /** Get index.
     *
     * @param[in] x Coordinate value.
     * @returns Index for coordinate value.
     */
    virtual
    double getIndex(const double x) const = 0;

    // NOT IMPLEMENTED ----------------------------------------------------------------------------
private:

    Indexing(const Indexing&); ///< Not implemented
    const Indexing& operator=(const Indexing&); ///< Not implemented

}; // Indexing

class geomodelgrids::utils::IndexingUniform : public Indexing {
    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /** Constructor.
     *
     * @param[in] dx Resolution along coordinate axis.
     */
    IndexingUniform(const double dx);

    /** Get index.
     *
     * @param[in] x Distance along coordinate axis from beginning.
     */
    double getIndex(const double x) const;

    // PRIVATE ------------------------------------------------------------------------------------
private:

    const double _dx; ///< Resolution along coordinate axis.

    // NOT IMPLEMENTED ----------------------------------------------------------------------------
private:

    IndexingUniform(const IndexingUniform&); ///< Not implemented
    const IndexingUniform& operator=(const IndexingUniform&); ///< Not implemented

}; // IndexingUniform

class geomodelgrids::utils::IndexingVariable : public Indexing {
    // PUBLIC ENUMS -------------------------------------------------------------------------------
public:

    enum SortOrder {
        ASCENDING=0,
        DESCENDING=1,
    };

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /** Constructor.
     *
     * @param[in] x Array of coordinates along axis.
     * @param[in] numX Number of coordinates along axis.
     * @param[in] sortOrder Order of coordinate indexing.
     */
    IndexingVariable(const double* x,
                     const size_t numX,
                     SortOrder sortOrder=ASCENDING);

    /// Destructor.
    ~IndexingVariable(void);

    /** Get index.
     *
     * @param[in] x Distance along coordinate axis from beginning.
     */
    double getIndex(const double x) const;

    inline static
    bool less(const double x,
              const double y) {
        return x < y;
    }

    inline static
    bool greater(const double x,
                 const double y) {
        return x >= y;
    }

    // PRIVATE ------------------------------------------------------------------------------------
private:

    double* _x; ///< Coordinates along axis.
    const size_t _numX; ///< Number of coordinates along axis.
    SortOrder _order; ///< Order of coordinates.

    // NOT IMPLEMENTED ----------------------------------------------------------------------------
private:

    IndexingVariable(const IndexingVariable&); ///< Not implemented
    const IndexingVariable& operator=(const IndexingVariable&); ///< Not implemented

}; // IndexingVariable

#endif // geomodelgrids_utils_indexing_hh

// End of file
