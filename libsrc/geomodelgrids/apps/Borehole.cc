#include <portinfo>

#include "Borehole.hh" // implementation of class methods

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
        namespace _Borehole {
            static const double NOVALUE = 1.0e+20;
        } // _Borehole
    } // apps
} // geomodelgrids

// ---------------------------------------------------------------------------------------------------------------------
// Constructor
geomodelgrids::apps::Borehole::Borehole() :
    _pointsCRS("EPSG:4326"),
    _outputFilename(""),
    _logFilename(""),
    _maxDepth(5000.0),
    _dz(10.0),
    _showHelp(false) {
    _location[0] = _Borehole::NOVALUE;
    _location[1] = _Borehole::NOVALUE;
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::apps::Borehole::~Borehole(void) {}


// ---------------------------------------------------------------------------------------------------------------------
// Run query application.
int
geomodelgrids::apps::Borehole::run(int argc,
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

    const double groundSurf = query.queryElevation(_location[0], _location[1]);
    const size_t numPoints = size_t(1 + _maxDepth / _dz);

    std::ofstream sout(_outputFilename);
    sout << _createOutputHeader(argc, argv);

    const size_t numQueryValues = _valueNames.size();
    std::vector<double> values(numQueryValues);
    sout << std::scientific << std::setprecision(6);
    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double elevation = groundSurf - _dz*iPt;
        query.query(&values[0], _location[0], _location[1], elevation);

        sout << std::setw(14) << elevation;
        for (size_t i = 0; i < numQueryValues; ++i) {
            sout << std::setw(14) << values[i];
        } // for
        sout << "\n";
    } // while

    query.finalize();

    return 0;
} // run


// ---------------------------------------------------------------------------------------------------------------------
// Parse command line arguments.
void
geomodelgrids::apps::Borehole::_parseArgs(int argc,
                                          char* argv[]) {
    static struct option options[11] = {
        {"help", no_argument, NULL, 'h'},
        {"values", required_argument, NULL, 'v'},
        {"max-depth", required_argument, NULL, 'd'},
        {"location", required_argument, NULL, 'p'},
        {"dz", required_argument, NULL, 'r'},
        {"points-coordsys", required_argument, NULL, 'c'},
        {"output", required_argument, NULL, 'o'},
        {"log", required_argument, NULL, 'l'},
        {"models", required_argument, NULL, 'm'},
        {0, 0, 0, 0}
    };

    while (true) {
        // extern char* optarg;
        const char c = getopt_long(argc, argv, "hv:d:o:r:p:c:o:l:m:", options, NULL);
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
        case 'd': {
            _maxDepth = atof(optarg);
            break;
        } // 'd'
        case 'p': {
            std::istringstream tokenStream(optarg);
            std::string token;
            int index = 0;
            while (std::getline(tokenStream, token, ',')) {
                _location[index++] = std::stod(token);
            } // while
            break;
        } // 'o'
        case 'r': {
            _dz = atof(optarg);
            break;
        } // 'r'
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

    if (!_showHelp) { // Verify required arguments were provided.
        bool optionsOkay = true;
        std::ostringstream msg;
        if (_outputFilename.empty()) {
            msg << "    - Missing filename for output. Use --output=FILE_OUTPUT\n";
            optionsOkay = false;
        } // if
        if (_modelFilenames.empty()) {
            msg << "    - Missing list of model filenames. Use --models=FILE_0,...,FILE_M\n";
            optionsOkay = false;
        } // if
        if ((_location[0] == _Borehole::NOVALUE) || (_location[1] == _Borehole::NOVALUE)) {
            msg << "    - Missing boreole location. Use --location=X,Y\n";
            optionsOkay = false;
        } // if

        if (!optionsOkay) {
            throw std::runtime_error(std::string("Missing required command line arguments:\n")+ msg.str());
        } // if
    } // if
} // _parseArgs


// ---------------------------------------------------------------------------------------------------------------------
// Print help information.
void
geomodelgrids::apps::Borehole::_printHelp(void) {
    std::cout << "Usage: geomodelgrids_borehole "
              << "[--help] [--log=FILE_LOG] --location=X,Y --models=FILE_0,...,FILE_M --output=FILE_OUTPUT "
              << "[--values=VALUE_0,...,VALUE_N] [--max-depth=Z] [--dz=RESOLUTION] [--points-coordsys=PROJ|EPSG|WKT]\n\n"
              << "    --help                           Print help information to stdout and exit.\n"
              << "    --log=FILE_LOG                   Write logging information to FILE_LOG.\n"
              << "    --location=X,Y                   Location of virtual borehole in point coordinate system.\n"
              << "    --models=FILE_0,...,FILE_M       Models to query (in order).\n"
              << "    --output=FILE_OUTPUT             Write values to FILE_OUTPUT.\n"
              << "    --values=VALUE_0,...,VALUE_N     Values (in order) to return in borehole query (default is all "
              << "values in model).\n"
              << "    --max-depth=DEPTH                Depth extent of virtual borehole in point coordinate system "
              << "vertical units (default=5000m).\n"
              << "    --dz=RESOLUTION                  Vertical resolution of query points in virtual borehole "
              << "in point coordinate system vertical units (default=10m).\n"
              << "    --points-coordsys=PROJ|EPSG|WKT  Coordinate system of input points (default=EPSG:4326).\n"
              << std::endl;
} // _printHelp


// ---------------------------------------------------------------------------------------------------------------------
// Create header for output.
std::string
geomodelgrids::apps::Borehole::_createOutputHeader(int argc,
                                                   char* argv[]) {
    std::ostringstream header;
    header << "#";
    for (int i = 0; i < argc; ++i) {
        header << " " << argv[i];
    } // for
    header << "\n";
    return header.str();
} // _createOutputHeader


// End of file