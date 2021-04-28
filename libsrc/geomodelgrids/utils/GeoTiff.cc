#include <portinfo>

#include "GeoTiff.hh" // implementation of class methods

#include "gdal/gdal_priv.h" // USES GDAL
#include "proj.h" // USES PJ

#include <cassert>
#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::runtime_error

// ------------------------------------------------------------------------------------------------
geomodelgrids::utils::GeoTiff::GeoTiff(void) :
    _driver(NULL),
    _dataset(NULL),
    _buffer(NULL),
    _noDataValue(-1.0e+20),
    _numCols(0),
    _numRows(0),
    _numBands(0) {
    GDALAllRegister();
    for (int i = 0; i < 6; ++i) {
        _transform[i] = 0.0;
    } // for
}


// ------------------------------------------------------------------------------------------------
geomodelgrids::utils::GeoTiff::~GeoTiff(void) {
    this->close();
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::setNumRows(const size_t numRows) {
    if (0 == numRows) {
        throw std::invalid_argument("Number of rows in image must be positive.");
    } // if
    _numRows = numRows;
}


// ------------------------------------------------------------------------------------------------
size_t
geomodelgrids::utils::GeoTiff::getNumRows(void) const {
    return _numRows;
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::setNumCols(const size_t numCols) {
    if (0 == numCols) {
        throw std::invalid_argument("Number of columns in image must be positive.");
    } // if
    _numCols = numCols;
}


// ------------------------------------------------------------------------------------------------
size_t
geomodelgrids::utils::GeoTiff::getNumCols(void) const {
    return _numCols;
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::setNumBands(const size_t numBands) {
    if (0 == numBands) {
        throw std::invalid_argument("Number of raster bands in image must be positive.");
    } // if
    _numBands = numBands;
}


// ------------------------------------------------------------------------------------------------
size_t
geomodelgrids::utils::GeoTiff::getNumBands(void) const {
    return _numBands;
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::setBandLabels(const std::vector<std::string>& labels) {
    if (labels.size() != _numBands) {
        std::ostringstream msg;
        msg << "Number of raster band labels (" << labels.size() << ") must match the number of raster bands ("
            << _numBands << ").";
        throw std::invalid_argument(msg.str());
    } // if
    _bandLabels = labels;
}


// ------------------------------------------------------------------------------------------------
const std::vector<std::string>&
geomodelgrids::utils::GeoTiff::getBandLabels(void) const {
    return _bandLabels;
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::setCRS(const char* value) {
    if (!value || (strlen(value) == 0)) {
        throw std::invalid_argument("CRS string must be a PROJ, WKT, or EPGS code.");
    } // if
    _crs = value;
}


// ------------------------------------------------------------------------------------------------
const char*
geomodelgrids::utils::GeoTiff::getCRS(void) const {
    return _crs.c_str();
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::setBBox(const double minX,
                                       const double maxX,
                                       const double minY,
                                       const double maxY) {
    const double dx = (maxX-minX) / _numCols;
    _transform[0] = minX;
    _transform[1] = dx;

    const double dy = (minY-maxY) / _numRows;
    _transform[3] = maxY;
    _transform[5] = dy;
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::getBBox(double* minX,
                                       double* maxX,
                                       double* minY,
                                       double* maxY) {
    const double _minX = _transform[0];
    const double dx = _transform[1];
    if (minX) { *minX = _minX; }
    if (maxX) { *maxX = _minX + dx * _numCols; }

    const double _maxY = _transform[3];
    const double dy = _transform[5];
    if (minY) { *minY = _maxY - dy * _numRows; }
    if (maxY) { *maxY = _maxY; }
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::setNoDataValue(const float value) {
    _noDataValue = value;
}


// ------------------------------------------------------------------------------------------------
float*
geomodelgrids::utils::GeoTiff::getBands(void) {
    if (!_buffer) { _buffer = new float[_numCols * _numRows * _numBands]; }
    return _buffer;
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::create(const char* filename) {
    this->close();
    _driver = GetGDALDriverManager()->GetDriverByName("GTiff");
    if (!_driver) {
        throw std::runtime_error("Could not get GDAL GeoTiff driver.");
    } // if

    char** options = NULL;
    options = CSLSetNameValue(options, "COMPRESS", "DEFLATE");
    _dataset = _driver->Create(filename, _numCols, _numRows, _numBands, GDT_Float32, options);
    CSLDestroy(options);options = NULL;
    if (!_dataset) {
        std::ostringstream msg;
        msg << "Could not open GeoTiff file '" << filename << "'.";
        throw std::runtime_error(msg.str().c_str());
    } // if

    // Set projection
    PJ_CONTEXT* context = NULL;
    PJ* proj = proj_create(context, _crs.c_str());
    _dataset->SetProjection(proj_as_wkt(context, proj, PJ_WKT1_GDAL, options));
    proj_destroy(proj);proj = NULL;

    // Set geographic transformation.
    _dataset->SetGeoTransform(_transform);

    // Set band labels.
    assert(_bandLabels.size() == size_t(_dataset->GetRasterCount()));
    for (size_t i = 0; i < _bandLabels.size(); ++i) {
        GDALRasterBand* band = _dataset->GetRasterBand(i+1);
        assert(band);
        band->SetDescription(_bandLabels[i].c_str());
        band->SetNoDataValue(_noDataValue);
    } // for
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::write(void) {
    CPLErr err = _dataset->RasterIO(GF_Write, 0, 0, _numCols, _numRows, _buffer,
                                    _numCols, _numRows, GDT_Float32, _numBands, NULL, 0, 0, 0, NULL);
    if (err != CE_None) { throw std::runtime_error("Error while writing row of raster bands."); }
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::read(const char* filename) {
    this->close();
    try {
        _driver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if (!_driver) { throw std::runtime_error("Could not get GDAL GeoTiff driver."); }
        _dataset = (GDALDataset*) GDALOpen(filename, GA_ReadOnly);
        if (!_dataset) { throw std::runtime_error("Could not open file."); }

        _numCols = _dataset->GetRasterXSize();
        _numRows = _dataset->GetRasterYSize();
        _numBands = _dataset->GetRasterCount();
        const char* value = _dataset->GetProjectionRef();
        if (value) { _crs = value; }

        CPLErr err = _dataset->GetGeoTransform(_transform);
        if (err != CE_None) { throw std::runtime_error("Could not get geographic transformation."); }
        if ((_transform[2] != 0.0) || (_transform[4] != 0.0)) {
            throw std::runtime_error("Unsupported geographic transformsion.");
        } // if

        _bandLabels.resize(_numBands);
        for (size_t i = 0; i < _numBands; ++i) {
            GDALRasterBand* band = _dataset->GetRasterBand(i+1);
            const char* label = band->GetDescription();
            if (label) {
                _bandLabels[i] = label;
            } // if
        } // for

        const size_t bufferSize = _numCols * _numRows * _numBands;
        _buffer = (bufferSize > 0) ? new float[bufferSize] : NULL;assert(_buffer);

        err = _dataset->RasterIO(GF_Read, 0, 0, _numCols, _numRows, _buffer, _numCols, _numRows,
                                 GDT_Float32, _numBands, NULL, 0, 0, 0, NULL);
        if (err != CE_None) { throw std::runtime_error("Error while reading row of raster bands."); }
    } catch (const std::exception& err) {
        this->close();
        std::ostringstream msg;
        msg << "Error while reading GeoTiff file '" << filename << "'. "
            << err.what();
        throw std::runtime_error(msg.str());
    } catch (...) {
        this->close();
        std::ostringstream msg;
        msg << "Unknown error while reading GeoTiff file '" << filename << "'.";
        throw std::runtime_error(msg.str());
    } // try/catch
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::utils::GeoTiff::close(void) {
    delete[] _buffer;_buffer = NULL;
    if (_dataset) {
        GDALClose(_dataset);
        _dataset = NULL;
    } // if
    if (_driver) {
        GDALDestroyDriverManager();
        _driver = NULL;
    } // if
}


// End of file
