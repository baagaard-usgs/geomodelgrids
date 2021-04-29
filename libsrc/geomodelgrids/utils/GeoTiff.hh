/** High-level interface to GDAL GeoTiff driver.
 */

#if !defined(geomodelgrids_utils_geotiff_hh)
#define geomodelgrids_utils_geotiff_hh

#include "utilsfwd.hh" // forward declarations

#include <string> // HASA std::string
#include <vector> // HASA std::vector

class GDALDriver;
class GDALDataset;

class geomodelgrids::utils::GeoTiff {
    friend class TestGeoTiff; // Unit testing

public:

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Default constructor.
    GeoTiff(void);

    /// Destructor
    ~GeoTiff(void);

    /** Set number of rows in image.
     *
     * @param[in] numRows Number of rows in image.
     */
    void setNumRows(const size_t numRows);

    /** Get number of rows in image.
     *
     * @returns Number of rows in image.
     */
    size_t getNumRows(void) const;

    /** Set number of rows in image.
     *
     * @param[in] numCols Number of columns in image.
     */
    void setNumCols(const size_t numCols);

    /** Get number of rows in image.
     *
     * @returns Number of columns in image.
     */
    size_t getNumCols(void) const;

    /** Set number of rows in image.
     *
     * @param[in] numBands Number of bands in image.
     */
    void setNumBands(const size_t numBands);

    /** Get number of rows in image.
     *
     * @returns Number of bands in image.
     */
    size_t getNumBands(void) const;

    /** Set labels of raster bands.
     *
     * @param[in] labels Labels for raster bands.
     */
    void setBandLabels(const std::vector<std::string>& labels);

    /** Get labels of raster bands.
     *
     * @param[in] labels Labels for raster bands.
     */
    const std::vector<std::string>& getBandLabels(void) const;

    /** Set coordinate system.
     *
     * String can be EPSG:XXXX, WKT, or Proj.
     *
     * @param[in] value String specifying coordinate system.
     */
    void setCRS(const char* value);

    /** Get coordinate system.
     *
     * @returns String specifying coordinate system.
     */
    const char* getCRS(void) const;

    /** Set bounding box.
     *
     * @pre Must have set image size.
     *
     * @param[in] minX Minimum x in bounding box.
     * @param[in] maxX Maximum x in bounding box.
     * @param[in] minY Minimum y in bounding box.
     * @param[in] maxY Maximum y in bounding box.
     */
    void setBBox(const double minX,
                 const double maxX,
                 const double minY,
                 const double maxY);

    /** Get bounding box.
     *
     * @param[out] minX Minimum x in bounding box.
     * @param[out] maxX Maximum x in bounding box.
     * @param[out] minY Minimum y in bounding box.
     * @param[out] maxY Maximum y in bounding box.
     */
    void getBBox(double* minX,
                 double* maxX,
                 double* minY,
                 double* maxY);

    /** Set no data value.
     *
     * @param[in] value Value indicating no data for pixel.
     */
    void setNoDataValue(const float value);

    /** Get buffer for raster bands.
     *
     * Image data is pixel sequential [row, column, band].
     *
     * @returns Buffer holding raster bands.
     */
    float* getBands(void);

    /** Open GeoTiff file for writing.
     *
     * @pre Must have set image size, CRS, and bounding box.
     *
     * @param[in] filename Name of file to open.
     */
    void create(const char* filename);

    /// Write data to file.
    void write(void);

    /** Read data from file.
     *
     * @param[in] filename Name of image file.
     */
    void read(const char* filename);

    /// Close file.
    void close(void);

    // PRIVATE MEMBERS ----------------------------------------------------------------------------
private:

    GDALDriver* _driver; ///< GDAL driver.
    GDALDataset* _dataset; ///< GDAL dataset.
    float* _buffer; ///< Raster bands.
    float _noDataValue; ///< Value indicating no data.
    size_t _numCols; ///< Number of columns in image.
    size_t _numRows; ///< Number of rows in image.
    size_t _numBands; ///< Number of raster bands.
    std::vector<std::string> _bandLabels; ///< Labels of raster bands.
    std::string _crs; ///< CRS for image data.
    double _transform[6]; ///< Geographic transformation.

    // NOT IMPLEMENTED ----------------------------------------------------------------------------
private:

    GeoTiff(const GeoTiff&); ///< Not implemented
    const GeoTiff& operator=(const GeoTiff&); ///< Not implemented

}; // GeoTiff

#endif // geomodelgrids_utils_geotiff_hh

// End of file
