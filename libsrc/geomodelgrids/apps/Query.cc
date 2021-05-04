#include <portinfo>

#include "Query.hh" // implementation of class methods

#include "geomodelgrids/serial/Query.hh" // USES Query
#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler

#include <getopt.h> // USES getopt_long()
#include <iomanip>
#include <fstream> // USES std::ifstream, std::ofstream
#include <sstream> // USES std::ostringstream, std::istringstream
#include <cassert> // USES assert()
#include <iostream> // USES std::cout

namespace geomodelgrids {
    namespace apps {
        namespace _Query {
            static const int cwidth = 14;
            static const int precision = 6;
        } // _Query
    } // apps
} // geomodelgrids

// ------------------------------------------------------------------------------------------------
// Constructor
geomodelgrids::apps::Query::Query() :
    _pointsFilename(""),
    _pointsCRS("EPSG:4326"),
    _outputFilename(""),
    _logFilename(""),
    _squashMinElev(0.0),
    _squash(geomodelgrids::serial::Query::SQUASH_NONE),
    _showHelp(false) {}


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::apps::Query::~Query(void) {}


// ------------------------------------------------------------------------------------------------
// Run query application.
int
geomodelgrids::apps::Query::run(int argc,
                                char* argv[]) {
    _parseArgs(argc, argv);

    if (_showHelp) {
        _printHelp();
        return 0;
    } // if

    geomodelgrids::serial::Query query;
    if (!_logFilename.empty()) {
        geomodelgrids::utils::ErrorHandler& errorHandler = query.getErrorHandler();
        errorHandler.setLogFilename(_logFilename.c_str());
        errorHandler.setLoggingOn(true);
    } // if
    query.initialize(_modelFilenames, _valueNames, _pointsCRS);
    if (geomodelgrids::serial::Query::SQUASH_NONE != _squash) {
        query.setSquashing(_squash);
        query.setSquashMinElev(_squashMinElev);
    } // if

    std::ifstream sin(_pointsFilename);
    if (!sin.is_open() && !sin.good()) {
        std::ostringstream msg;
        msg << "Could not open points file '" << _pointsFilename << "' for reading.";
        throw std::runtime_error(msg.str().c_str());
    } // if

    std::ofstream sout(_outputFilename);
    if (!sout.is_open() && !sout.good()) {
        std::ostringstream msg;
        msg << "Could not open output file '" << _outputFilename << "' for writing.";
        throw std::runtime_error(msg.str().c_str());
    } // if

    sout << _createOutputHeader(argc, argv);
    const size_t numQueryValues = _valueNames.size();
    std::vector<double> values(numQueryValues);
    sout << std::scientific << std::setprecision(_Query::precision);
    while (true) {
        double srcX, srcY, srcZ;
        sin >> srcX >> srcY >> srcZ;
        if (sin.eof() || !sin.good()) {
            break;
        } // if

        query.query(&values[0], srcX, srcY, srcZ);

        sout << std::setw(_Query::cwidth) << srcX
             << std::setw(_Query::cwidth) << srcY
             << std::setw(_Query::cwidth) << srcZ;
        for (size_t i = 0; i < numQueryValues; ++i) {
            sout << std::setw(_Query::cwidth) << values[i];
        } // for
        sout << "\n";
    } // while

    query.finalize();

    return 0;
} // run


// ------------------------------------------------------------------------------------------------
// Parse command line arguments.
void
geomodelgrids::apps::Query::_parseArgs(int argc,
                                       char* argv[]) {
    static struct option options[10] = {
        {"help", no_argument, NULL, 'h'},
        {"values", required_argument, NULL, 'v'},
        {"squash-min-elev", required_argument, NULL, 's'},
        {"squash-surface", required_argument, NULL, 'r'},
        {"points", required_argument, NULL, 'p'},
        {"points-coordsys", required_argument, NULL, 'c'},
        {"output", required_argument, NULL, 'o'},
        {"log", required_argument, NULL, 'l'},
        {"models", required_argument, NULL, 'm'},
        {0, 0, 0, 0}
    };

    while (true) {
        // extern char* optarg;
        const char c = getopt_long(argc, argv, "hv:s:r:p:c:o:l:m:", options, NULL);
        if (-1 == c) { break; }
        switch (c) {
        case 'h':
            _showHelp = true;
            break;
        case 'v': {
            _valueNames.clear();
            std::istringstream tokenStream(optarg);
            std::string token;
            while (std::getline(tokenStream, token, ',')) {
                _valueNames.push_back(token);
            } // while
            break;
        } // 'v'
        case 's': {
            if (geomodelgrids::serial::Query::SQUASH_NONE == _squash) {
                _squash = geomodelgrids::serial::Query::SQUASH_TOP_SURFACE;
            } // if
            _squashMinElev = std::stod(optarg);
            break;
        } // 's'
        case 'r': {
            const std::string& surface = optarg;
            if (std::string("top_surface") == surface) {
                _squash = geomodelgrids::serial::Query::SQUASH_TOP_SURFACE;
            } else if (std::string("topography_bathymetry") == surface) {
                _squash = geomodelgrids::serial::Query::SQUASH_TOPOGRAPHY_BATHYMETRY;
            } else {
                _squash = geomodelgrids::serial::Query::SQUASH_NONE;
            }
            break;
        } // 's'
        case 'p': {
            _pointsFilename = optarg;
            break;
        } // 'p'
        case 'c': {
            _pointsCRS = optarg;
            break;
        } // 'p'
        case 'o': {
            _outputFilename = optarg;
            break;
        } // 'o'
        case 'l': {
            _logFilename = optarg;
            break;
        } // 'l'
        case 'm': {
            _modelFilenames.clear();
            std::istringstream tokenStream(optarg);
            std::string token;
            while (std::getline(tokenStream, token, ',')) {
                _modelFilenames.push_back(token);
            } // while
            break;
        } // 'm'
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
        if (_valueNames.empty()) {
            msg << "    - Missing names of values to return in queries. Use --values=VALUE_0,...,VALUE_N\n";
            optionsOkay = false;
        } // if
        if (_pointsFilename.empty()) {
            msg << "    - Missing filename for list of points. Use --points=FILE_POINTS\n";
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
geomodelgrids::apps::Query::_printHelp(void) {
    std::cout << "Usage: geomodelgrids_query "
              << "[--help]  [--log=FILE_LOG] --values=VALUE_0,...,VALUE_N --models=FILE_0,...,FILE_M "
              << "--points=FILE_POINTS  --output=FILE_OUTPUT [--squash-min-elev=ELEV] "
              << "[--squash-surface=none|top_surface|topography_bathymetry] [--points-coordsys=PROJ|EPSG|WKT]\n\n"
              << "    --help                           Print help information to stdout and exit.\n"
              << "    --log=FILE_LOG                   Write logging information to FILE_LOG.\n"
              << "    --values=VALUE_0,...,VALUE_N     Values (in order) to return in query.\n"
              << "    --models=FILE_0,...,FILE_M       Models to query (in order).\n"
              << "    --points=FILE_POINTS             Read input points from FILE_POINTS.\n"
              << "    --output=FILE_OUTPUT             Write values to FILE_OUTPUT.\n"
              << "    --squash-min-elev=ELEV           Vertical coordinate is interpreted as -depth instead of elevation.\n"
              << "    --squash-surface=none|top_surface|topography_bathymetry    Surface reference for squashing (default=none).\n"
              << "    --points-coordsys=PROJ|EPSG|WKT  Coordinate system of input points (default=EPSG:4326)."
              << std::endl;
} // _printHelp


// ------------------------------------------------------------------------------------------------
// Create header for output.
std::string
geomodelgrids::apps::Query::_createOutputHeader(int argc,
                                                char* argv[]) {
    std::ostringstream header;
    header << "#";
    for (int i = 0; i < argc; ++i) {
        header << " " << argv[i];
    } // for
    header << "\n#"
           << std::setw(_Query::cwidth-1) << "x0"
           << std::setw(_Query::cwidth) << "x1"
           << std::setw(_Query::cwidth) << "x2";
    for (size_t i = 0; i < _valueNames.size(); ++i) {
        header << std::setw(_Query::cwidth) << _valueNames[i];
    } // for
    header << "\n";
    return header.str();
} // _createOutputHeader


// End of file
