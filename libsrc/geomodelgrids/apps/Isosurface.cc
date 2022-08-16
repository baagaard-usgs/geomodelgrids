#include <portinfo>

#include "Isosurface.hh" // implementation of class methods

#include "geomodelgrids/serial/Query.hh" // USES Query
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE
#include "geomodelgrids/utils/CRSTransformer.hh" // USES CRSTransformer
#include "geomodelgrids/utils/GeoTiff.hh" // USES GeoTiff
#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler

#include <cmath>
#include <strings.h> // USES strcasecmp()
#include <algorithm>
#include <getopt.h> // USES getopt_long()
#include <iomanip>
#include <fstream> // USES std::ofstream
#include <sstream> // USES std::ostringstream, std::istringstream
#include <cassert> // USES assert()
#include <iostream> // USES std::cout

namespace geomodelgrids {
    namespace apps {
        // ----------------------------------------------------------------------------------------
        class Isosurfacer {
public:

            typedef std::pair<std::string,double> isosurface_t;

            Isosurfacer(const Isosurface& app);
            ~Isosurfacer(void);

            geomodelgrids::serial::Query* getQuery(void) const;

            void initialize(void);

            void query(double* values,
                       const double x,
                       const double y);

            void finalize(void);

private:

            const Isosurface& _app;
            geomodelgrids::serial::Query* _query;
            size_t _numLevels;
            std::vector<double> _vbuffer;

        }; // Isosurfacer

        // ----------------------------------------------------------------------------------------
        class LineSearch {
public:

            LineSearch(geomodelgrids::serial::Query* query,
                       std::vector<double>& vbuffer,
                       const size_t numSeachPoints,
                       const double x,
                       const double y);

            virtual ~LineSearch(void);

            virtual
            size_t search(const double zTop,
                          const double zBot,
                          const double dz,
                          const double vTarget,
                          const size_t iValue) = 0;

protected:

            geomodelgrids::serial::Query* _query;
            std::vector<double>& _vbuffer;
            const size_t _numSearchPoints;
            const double _x;
            const double _y;

        }; // LineSearch

        class LineSearchDown : public LineSearch {
public:

            LineSearchDown(geomodelgrids::serial::Query* query,
                           std::vector<double>& vbuffer,
                           const size_t numSeachPoints,
                           const double x,
                           const double y);

            size_t search(const double zTop,
                          const double zBot,
                          const double dz,
                          const double vTarget,
                          const size_t iValue);

        }; // LineSearchDown

        class LineSearchUp : public LineSearch {
public:

            LineSearchUp(geomodelgrids::serial::Query* query,
                         std::vector<double>& vbuffer,
                         const size_t numSeachPoints,
                         const double x,
                         const double y);

            size_t search(const double zTop,
                          const double zBot,
                          const double dz,
                          const double vTarget,
                          const size_t iValue);

        }; // LineSearchUp
    } // apps
} // geomodelgrids

// ------------------------------------------------------------------------------------------------
// Constructor
geomodelgrids::apps::Isosurface::Isosurface(void) :
    _bboxCRS("EPSG:4326"),
    _outputFilename(""),
    _logFilename(""),
    _minX(geomodelgrids::NODATA_VALUE),
    _maxX(geomodelgrids::NODATA_VALUE),
    _minY(geomodelgrids::NODATA_VALUE),
    _maxY(geomodelgrids::NODATA_VALUE),
    _horizRes(0.0),
    _vertRes(10.0),
    _maxDepth(0.0),
    _numSearchPoints(10),
    _depthSurface(geomodelgrids::serial::Query::SQUASH_TOPOGRAPHY_BATHYMETRY),
    _preferShallow(true),
    _showHelp(false) {
    _isosurfaces.resize(2);
    _isosurfaces[0] = Isosurfacer::isosurface_t("Vs", 1.0e+3);
    _isosurfaces[1] = Isosurfacer::isosurface_t("Vs", 2.5e+3);
}


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::apps::Isosurface::~Isosurface(void) {}


// ------------------------------------------------------------------------------------------------
// Run query application.
int
geomodelgrids::apps::Isosurface::run(int argc,
                                     char* argv[]) {
#if !defined(WITH_GDAL)
    std::cout << "Isosurface generation requires building with GDAL support." << std::endl;
    return 1;
#else
    _parseArgs(argc, argv);

    if (_showHelp) {
        _printHelp();
        return 0;
    } // if

    geomodelgrids::utils::CRSTransformer* toXYOrder =
        geomodelgrids::utils::CRSTransformer::createGeoToXYAxisOrder(_bboxCRS.c_str());
    assert(toXYOrder);
    toXYOrder->transform(&_minX, &_minY, NULL, _minX, _minY, 0.0);
    toXYOrder->transform(&_maxX, &_maxY, NULL, _maxX, _maxY, 0.0);

    try {
        Isosurfacer isosurfacer(*this);
        isosurfacer.initialize();
        if (!_logFilename.empty()) {
            geomodelgrids::serial::Query* query = isosurfacer.getQuery();assert(query);
            geomodelgrids::utils::ErrorHandler& errorHandler = query->getErrorHandler();
            errorHandler.setLogFilename(_logFilename.c_str());
            errorHandler.setLoggingOn(true);
        } // if

        const size_t numX = size_t((_maxX + 0.5*_horizRes - _minX) / _horizRes);
        const size_t numY = size_t((_maxY + 0.5*_horizRes - _minY) / _horizRes);
        const size_t numIsosurfaces = _isosurfaces.size();

        std::vector<std::string> bandLabels(numIsosurfaces);
        for (size_t i = 0; i < numIsosurfaces; ++i) {
            std::ostringstream label;
            label << _isosurfaces[i].first << "=" << _isosurfaces[i].second;
            bandLabels[i] = label.str();
        } // for

        geomodelgrids::utils::GeoTiff writer;
        writer.setNumCols(numX);
        writer.setNumRows(numY);
        writer.setNumBands(numIsosurfaces);
        writer.setBandLabels(bandLabels);
        writer.setCRS(_bboxCRS.c_str());
        writer.setBBox(_minX, _maxX, _minY, _maxY);
        writer.setNoDataValue(geomodelgrids::NODATA_VALUE);
        writer.create(_outputFilename.c_str());

        std::vector<double> values(numIsosurfaces);
        float* buffer = writer.getBands();
        for (size_t iY = 0, iPt = 0; iY < numY; ++iY) {
            const size_t row = numY - iY - 1;
            const double y = _minY + (iY + 0.5) * _horizRes;
            double xCRS, yCRS;

            for (size_t iX = 0; iX < numX; ++iX, ++iPt) {
                const size_t col = iX;
                const double x = _minX + (iX + 0.5) * _horizRes;

                toXYOrder->inverse_transform(&xCRS, &yCRS, NULL, x, y, 0.0);
                isosurfacer.query(&values[0], xCRS, yCRS);
                for (size_t iValue = 0; iValue < numIsosurfaces; ++iValue) {
                    buffer[iValue*numY*numX + row*numX + col] = values[iValue];
                } // for
            } // for
        } // for
        delete toXYOrder;toXYOrder = NULL;
        writer.write();
        writer.close();
        isosurfacer.finalize();
    } catch (const std::exception& err) {
        delete toXYOrder;toXYOrder = NULL;
        throw;
    } catch (...) {
        delete toXYOrder;toXYOrder = NULL;
        throw;
    } // try/catch

    return 0;
#endif
} // run


// ------------------------------------------------------------------------------------------------
// Parse command line arguments.
void
geomodelgrids::apps::Isosurface::_parseArgs(int argc,
                                            char* argv[]) {
    static struct option options[14] = {
        {"help", no_argument, NULL, 'h'},
        {"log", required_argument, NULL, 'l'},
        {"bbox", required_argument, NULL, 'b'},
        {"hresolution", required_argument, NULL, 'r'},
        {"vresolution", required_argument, NULL, 'v'},
        {"isosurface", required_argument, NULL, 'i'},
        {"depth-reference", required_argument, NULL, 's'},
        {"max-depth", required_argument, NULL, 'd'},
        {"num-search-points", required_argument, NULL, 'n'},
        {"models", required_argument, NULL, 'm'},
        {"output", required_argument, NULL, 'o'},
        {"prefer-deep", no_argument, NULL, 'p'},
        {"bbox-coordsys", required_argument, NULL, 'c'},
        {0, 0, 0, 0}
    };

    _isosurfaces.clear();
    while (true) {
        // extern char* optarg;
        const char c = getopt_long(argc, argv, "hl:b:r:v:i:s:d:m:o:pc:", options, NULL);
        if (-1 == c) { break; }
        switch (c) {
        case 'h':
            _showHelp = true;
            break;
        case 'l': {
            _logFilename = optarg;
            break;
        } // 'l'
        case 'b': {
            std::istringstream tokenStream(optarg);
            std::string token;
            int index = 0;
            double bbox[4] = {
                geomodelgrids::NODATA_VALUE,
                geomodelgrids::NODATA_VALUE,
                geomodelgrids::NODATA_VALUE,
                geomodelgrids::NODATA_VALUE,
            };
            while (std::getline(tokenStream, token, ',') && index < 4) {
                bbox[index++] = std::stod(token);
            } // while
            _minX = bbox[0];
            _maxX = bbox[1];
            _minY = bbox[2];
            _maxY = bbox[3];
            break;
        } // 'b'
        case 'r': {
            _horizRes = atof(optarg);
            break;
        } // 'r'
        case 'v': {
            _vertRes = atof(optarg);
            break;
        } // 'v'
        case 'i': {
            std::istringstream tokenStream(optarg);
            std::string token;
            std::vector<std::string> values;
            while (std::getline(tokenStream, token, ',')) {
                values.push_back(token);
            } // while
            if (values.size() == 2) {
                Isosurfacer::isosurface_t isosurface(values[0], atof(values[1].c_str()));
                _isosurfaces.push_back(isosurface);
            } // if
            break;
        } // 'i'
        case 's': {
            if (0 == strcasecmp("top_surface", optarg)) {
                _depthSurface = geomodelgrids::serial::Query::SQUASH_TOP_SURFACE;
            } else if (0 == strcasecmp("topography_bathymetry", optarg)) {
                _depthSurface = geomodelgrids::serial::Query::SQUASH_TOPOGRAPHY_BATHYMETRY;
            } else {
                _depthSurface = geomodelgrids::serial::Query::SQUASH_NONE;
            } // if/else
            break;
        } // 's'
        case 'd': {
            _maxDepth = atof(optarg);
            break;
        } // 'd'
        case 'n': {
            _numSearchPoints = atoi(optarg);
            break;
        } // 'n'
        case 'm': {
            _modelFilenames.clear();
            std::istringstream tokenStream(optarg);
            std::string token;
            while (std::getline(tokenStream, token, ',')) {
                _modelFilenames.push_back(token);
            } // while
            break;
        } // 'm'
        case 'o': {
            _outputFilename = optarg;
            break;
        } // 'o'
        case 'p': {
            _preferShallow = false;
            break;
        } // 'p'
        case 'c': {
            _bboxCRS = optarg;
            break;
        } // 'c'
        case '?': {
            std::ostringstream msg;
            msg << "Error parsing command line arguments:\n";
            for (int i = 0; i < argc; ++i) {
                msg << argv[i] << " ";
            } // for
            throw std::logic_error(msg.str().c_str());
        } // ?
        } // switch
    } // while

    if (1 == argc) {
        _showHelp = true;
    } // if
    if (!_showHelp) { // Verify required arguments were provided.
        bool optionsOkay = true;
        std::ostringstream msg;
        if ((_minX == geomodelgrids::NODATA_VALUE) || (_maxX == geomodelgrids::NODATA_VALUE)
            || (_minY == geomodelgrids::NODATA_VALUE) || (_maxY == geomodelgrids::NODATA_VALUE)) {
            msg << "    - Missing bounding box. Use --bbox=XMIN,XMAX,YMIN,YMAX\n";
            optionsOkay = false;
        } // if
        if (((_maxX - _minX)/_horizRes < 0) || ((_maxY - _minY) / _horizRes < 0)) {
            msg << "    - Invalid bounding box. Use --bbox=XMIN,XMAX,YMIN,YMAX\n";
        } // if
        if (0.0 == _horizRes) {
            msg << "    - Missing horizontal resolution. Use --resolution=RESOLUTION\n";
            optionsOkay = false;
        } // if
        if (_horizRes < 0.0) {
            msg << "    - Horizontal resolution (" << _horizRes << ") must be positive.\n";
            optionsOkay = false;
        } // if
        if (_vertRes <= 0.0) {
            msg << "    - Vertical resolution (" << _vertRes << ") must be positive.\n";
            optionsOkay = false;
        } // if
        if (0.0 == _maxDepth) {
            msg << "    - Missing maximum depth. Use --max-depth=DEPTH\n";
            optionsOkay = false;
        } // if
        if (_maxDepth < 0.0) {
            msg << "    - Maximum depth (" << _maxDepth << ") must be positive.\n";
            optionsOkay = false;
        } // if
        if (_numSearchPoints < 2) {
            msg << "    - Number of search points (" << _numSearchPoints << ") must be at least 2.\n";
            optionsOkay = false;
        } // if
        if (_isosurfaces.empty()) {
            msg << "    - Missing isosurfaces. Use --isosurface=NAME,VALUE (can be repeated)\n";
            optionsOkay = false;
        } // if
        if (geomodelgrids::serial::Query::SQUASH_NONE == _depthSurface) {
            msg << "    - Error parsing depth surface. Use --depth-surface=top_surface or "
                << "--depth-surface=topography_bathymetry\n";
            optionsOkay = false;
        } // if
        if (_outputFilename.empty()) {
            msg << "    - Missing filename for output. Use --output=FILE_OUTPUT\n";
            optionsOkay = false;
        } // if
        if (_modelFilenames.empty()) {
            msg << "    - Missing list of model filenames. Use --models=FILE_0,...,FILE_M\n";
            optionsOkay = false;
        } // if

        if (!optionsOkay) {
            throw std::runtime_error(std::string("Missing required command line arguments:\n")+ msg.str());
        } // if
    } // if
} // _parseArgs


// ------------------------------------------------------------------------------------------------
// Print help information.
void
geomodelgrids::apps::Isosurface::_printHelp(void) {
    std::cout << "Usage: geomodelgrids_isosurface "
              << "[--help] [--log=FILE_LOG] --bbox=XMIN,XMAX,YMIN,YMAX --hresolution=RESOLUTION "
              << "[--vresolution=RESOLUTION] --isosurface=NAME,VALUE [--depth-reference=SURFACE] "
              << "--max-depth=DEPTH [--num-search-points=NUM] --models=FILE_0,...,FILE_M --output=FILE_OUTPUT "
              << " [--prefer-deep] [--bbox-coordsys=PROJ|EPSG|WKT]\n\n"
              << "    --help                           Print help information to stdout and exit.\n"
              << "    --log=FILE_LOG                   Write logging information to FILE_LOG.\n"
              << "    --bbox=XMIN,XMAX,YMIN,YMAX       Bounding box for iosurface.\n"
              << "    --isosurface=NAME,VALUE         Name and values for isosurfaces (default=Vs,1.0 and Vs,2.5; "
              << "repeat for multiple values).\n"
              << "    --depth-reference=SURFACE        Surface to use for calculating depth "
              << "(default=topography_bathymetry)\n"
              << "    --max-depth=DEPTH                Maximum depth allowed for isosurface.\n"
              << "    --num-search-points=NUM          Number of search points in each iteration (default=10).\n"
              << "    --models=FILE_0,...,FILE_M       Models to query (in order).\n"
              << "    --output=FILE_OUTPUT             Write values to FILE_OUTPUT.\n"
              << "    --hresolution=RESOLUTION         Horizontal resolution of isosurface.\n"
              << "    --vresolution=RESOLUTION         Vertical resolution for depth of isosurface (default=10.0).\n"
              << "    --prefer-deep                    Prefer deepest elevation for isosurface rather than "
              << "shallowest (default=shallowest).\n"
              << "    --bbox-coordsys=PROJ|EPSG|WKT    Coordinate system for isosurface points (default=EPSG:4326)."
              << std::endl;
} // _printHelp


// ------------------------------------------------------------------------------------------------
geomodelgrids::apps::Isosurfacer::Isosurfacer(const Isosurface& app) :
    _app(app),
    _query(NULL) {}


// ------------------------------------------------------------------------------------------------
geomodelgrids::apps::Isosurfacer::~Isosurfacer(void) {
    delete _query;_query = NULL;
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::Isosurfacer::initialize(void) {
    delete _query;_query = new geomodelgrids::serial::Query();assert(_query);

    const size_t numIsosurfaces = _app._isosurfaces.size();
    std::vector<std::string> valueNames(numIsosurfaces);
    for (size_t i = 0; i < numIsosurfaces; ++i) {
        valueNames[i] = _app._isosurfaces[i].first;
    } // for
    _query->initialize(_app._modelFilenames, valueNames, _app._bboxCRS);

    _numLevels = size_t(ceil(log(_app._maxDepth/_app._vertRes) / log(_app._numSearchPoints)));
    assert(_numLevels >= 1);

    const size_t numValues = _app._isosurfaces.size();
    _vbuffer.resize(numValues);
}


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::Query*
geomodelgrids::apps::Isosurfacer::getQuery(void) const {
    return _query;
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::Isosurfacer::query(double* values,
                                        const double x,
                                        const double y) {
    assert(values);
    assert(_query);

    double topElev = 0.0;
    switch (_app._depthSurface) {
    case geomodelgrids::serial::Query::SQUASH_TOPOGRAPHY_BATHYMETRY:
        topElev = _query->queryTopoBathyElevation(x, y);
        break;
    case geomodelgrids::serial::Query::SQUASH_TOP_SURFACE:
        topElev = _query->queryTopElevation(x, y);
        break;
    default:
        throw std::logic_error("Unknown top surface in Isosurfacer::query().");
    } // switch
    if (topElev == geomodelgrids::NODATA_VALUE) {
        const size_t numValues = _vbuffer.size();
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            values[iValue] = geomodelgrids::NODATA_VALUE;
        } // for
        return;
    } // if

    LineSearch* lineSearch = _app._preferShallow ?
                             (LineSearch*) new LineSearchDown(_query, _vbuffer, _app._numSearchPoints, x, y) :
                             (LineSearch*) new LineSearchUp(_query, _vbuffer, _app._numSearchPoints, x, y);

    const size_t numValues = _vbuffer.size();
    for (size_t iValue = 0; iValue < numValues; ++iValue) {
        const double vTarget = _app._isosurfaces[iValue].second;
        double zTop = topElev - 1.0e-4;
        double zBot = topElev - _app._maxDepth;
        assert(zTop > zBot);

        for (size_t iLevel = 0; iLevel < _numLevels; ++iLevel) {
            const double dz = (zTop - zBot) / (_app._numSearchPoints-1);
            const size_t iTop = lineSearch->search(zTop, zBot, dz, vTarget, iValue);
            zTop -= iTop*dz;
            zBot = zTop - dz;
        } // for

        // Final interpolation
        _query->query(&_vbuffer[0], x, y, zTop);
        const double vTop = _vbuffer[iValue];

        _query->query(&_vbuffer[0], x, y, zBot);
        const double vBot = _vbuffer[iValue];

        if (((vTop <= vTarget) && (vTarget <= vBot)) || ((vTop >= vTarget) && (vTarget >= vBot))) {
            const double a = (vTop-vBot) / (zTop-zBot);
            const double b = vTop - a*zTop;
            values[iValue] = topElev - (vTarget - b) / a;
        } else if (vTarget < vTop) {
            values[iValue] = 0.0;
        } else {
            values[iValue] = geomodelgrids::NODATA_VALUE;
        } // if/else

    } // for

    delete lineSearch;lineSearch = NULL;
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::Isosurfacer::finalize(void) {
    assert(_query);
    _query->finalize();
}


// ------------------------------------------------------------------------------------------------
geomodelgrids::apps::LineSearch::LineSearch(geomodelgrids::serial::Query* query,
                                            std::vector<double>& vbuffer,
                                            const size_t numSeachPoints,
                                            const double x,
                                            const double y) :
    _query(query),
    _vbuffer(vbuffer),
    _numSearchPoints(numSeachPoints),
    _x(x),
    _y(y) {}


// ------------------------------------------------------------------------------------------------
geomodelgrids::apps::LineSearch::~LineSearch(void) {}


// ------------------------------------------------------------------------------------------------
geomodelgrids::apps::LineSearchDown::LineSearchDown(geomodelgrids::serial::Query* query,
                                                    std::vector<double>& vbuffer,
                                                    const size_t numSeachPoints,
                                                    const double x,
                                                    const double y) :
    LineSearch(query, vbuffer, numSeachPoints, x, y) {}


// ------------------------------------------------------------------------------------------------
size_t
geomodelgrids::apps::LineSearchDown::search(const double zTop,
                                            const double zBot,
                                            const double dz,
                                            const double vTarget,
                                            const size_t iValue) {
    size_t iTop = 0;
    for (size_t iPt = 1; iPt < _numSearchPoints; ++iPt) {
        const double z = zTop - iPt*dz;
        _query->query(&_vbuffer[0], _x, _y, z);
        const double v = _vbuffer[iValue];
        if (v >= vTarget) {
            iTop = iPt - 1;
            break;
        } // if

    } // for
    return iTop;
}


// ------------------------------------------------------------------------------------------------
geomodelgrids::apps::LineSearchUp::LineSearchUp(geomodelgrids::serial::Query* query,
                                                std::vector<double>&vbuffer,
                                                const size_t numSeachPoints,
                                                const double x,
                                                const double y) :
    LineSearch(query, vbuffer, numSeachPoints, x, y) {}


// ------------------------------------------------------------------------------------------------
size_t
geomodelgrids::apps::LineSearchUp::search(const double zTop,
                                          const double zBot,
                                          const double dz,
                                          const double vTarget,
                                          const size_t iValue) {
    size_t iTop = 0;
    for (size_t iPt = 1; iPt < _numSearchPoints; ++iPt) {
        const double z = zBot + iPt * dz;
        _query->query(&_vbuffer[0], _x, _y, z);
        const double v = _vbuffer[iValue];
        if (v < vTarget) {
            iTop = _numSearchPoints - iPt - 1;
            break;
        } // if

    } // for
    return iTop;
}


// End of file
