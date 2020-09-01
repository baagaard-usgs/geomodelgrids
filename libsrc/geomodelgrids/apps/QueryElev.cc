#include <portinfo>

#include "QueryElev.hh" // implementation of class methods

#include "geomodelgrids/serial/Query.hh" // USES Query
#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler

#include <getopt.h> // USES getopt_long()
#include <iomanip>
#include <fstream> // USES std::ifstream, std::ofstream
#include <sstream> // USES std::ostringstream, std::istringstream
#include <cassert> // USES assert()
#include <iostream> // USES std::cout

// ----------------------------------------------------------------------
// Constructor
geomodelgrids::apps::QueryElev::QueryElev() :
    _pointsFilename(""),
    _pointsCRS("EPSG:4326"),
    _outputFilename(""),
    _logFilename(""),
    _showHelp(false) {}


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::apps::QueryElev::~QueryElev(void) {}


// ---------------------------------------------------------------------------------------------------------------------
// Run query application.
int
geomodelgrids::apps::QueryElev::run(int argc,
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
    std::vector<std::string> valueNames;
    query.initialize(_modelFilenames, valueNames, _pointsCRS);

    std::ifstream sin(_pointsFilename);
    std::ofstream sout(_outputFilename);
    sout << std::scientific << std::setprecision(6);
    while (true) {
        double srcX, srcY;
        sin >> srcX >> srcY;
        if (sin.eof() || !sin.good()) {
            break;
        } // if

        const double elev = query.queryElevation(srcX, srcY);

        sout << std::setw(14) << srcX
             << std::setw(14) << srcY
             << std::setw(14) << elev
             << "\n";
    } // while

    query.finalize();

    return 0;
} // run


// ---------------------------------------------------------------------------------------------------------------------
// Parse command line arguments.
void
geomodelgrids::apps::QueryElev::_parseArgs(int argc,
                                           char* argv[]) {
    static struct option options[9] = {
        {"help", no_argument, NULL, 'h'},
        {"points", required_argument, NULL, 'p'},
        {"points-coordsys", required_argument, NULL, 'c'},
        {"output", required_argument, NULL, 'o'},
        {"log", required_argument, NULL, 'l'},
        {"models", required_argument, NULL, 'm'},
        {0, 0, 0, 0}
    };

    while (true) {
        // extern char* optarg;
        const char c = getopt_long(argc, argv, "hv:s:p:c:o:l:m:", options, NULL);
        if (-1 == c) { break; }
        switch (c) {
        case 'h':
            _showHelp = true;
            break;
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
            msg << "Error passing command line arguments:\n";
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


// ---------------------------------------------------------------------------------------------------------------------
// Print help information.
void
geomodelgrids::apps::QueryElev::_printHelp(void) {
    std::cout << "Usage: geomodelgrids_queryelev "
              << "[--help] --models=FILE_0,...,FILE_M --points=FILE_POINTS [--points-coordsys=PROJ|EPSG|WKT]"
              << "[--log=FILE_LOG] --output=FILE_OUTPUT\n\n"
              << "    --help                           Print help information to stdout and exit.\n"
              << "    --models=FILE_0,...,FILE_M       Models to query (in order).\n"
              << "    --points=FILE_POINTS             Read input points from FILE_POINTS.\n"
              << "    --points-coordsys=PROJ|EPSG|WKT  Coordinate system of input points (default=EPSG:4326).\n"
              << "    --log=FILE_LOG                   Write logging information to FILE_LOG."
              << "    --output=FILE_OUTPUT             Write values to FILE_OUTPUT."
              << std::endl;
} // _printHelp


// End of file
