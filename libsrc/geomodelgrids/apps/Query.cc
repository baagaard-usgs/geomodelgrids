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

// ----------------------------------------------------------------------
// Constructor
geomodelgrids::apps::Query::Query() :
    _pointsFilename(""),
    _pointsCRS("EPSG:4326"),
    _outputFilename(""),
    _logFilename(""),
    _squashMinElev(0.0),
    _squash(false),
    _showHelp(false) {}


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::apps::Query::~Query(void) {}


// ---------------------------------------------------------------------------------------------------------------------
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
    if (_squash) {
        query.setSquashMinElev(_squashMinElev);
    } // if

    std::ifstream sin(_pointsFilename);
    std::ofstream sout(_outputFilename);
    const size_t numQueryValues = _valueNames.size();
    std::vector<double> values(numQueryValues);
    sout << std::scientific << std::setprecision(6);
    while (!sin.eof() && sin.good()) {
        double srcX, srcY, srcZ;
        sin >> srcX >> srcY >> srcZ;

        query.query(&values[0], srcX, srcY, srcZ);

        sout << std::setw(14) << srcX
             << std::setw(14) << srcY
             << std::setw(14) << srcZ;
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
geomodelgrids::apps::Query::_parseArgs(int argc,
                                       char* argv[]) {
    static struct option options[9] = {
        {"help", no_argument, NULL, 'h'},
        {"values", required_argument, NULL, 'v'},
        {"squash-min-elev", required_argument, NULL, 's'},
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
            _squash = true;
            _squashMinElev = atof(optarg);
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
geomodelgrids::apps::Query::_printHelp(void) {
    std::cout << "Usage: geogrids_query "
              << "[--help] [--values=VALUE_0,...,VALUE_N] [--squash-min-elev=ELEV] --models=FILE_0,...,FILE_M "
              << "--points=FILE_POINTS [--points-coordsys=PROJ|EPSG|WKT] [--log=FILE_LOG] --output=FILE_OUTPUT\n\n"
              << "    --help                           Print help information to stdout and exit.\n"
              << "    --values=VALUE_0,...,VALUE_N     Values to return in query.\n"
              << "    --squash-min-elev=ELEV           Vertical coordinates is interpreted as -depth instead of "
              << "elevation if the elevation is above ELEV.\n"
              << "    --models=FILE_0,...,FILE_M       Models to query (in order).\n"
              << "    --points=FILE_POINTS             Read input points from FILE_POINTS.\n"
              << "    --points-coordsys=PROJ|EPSG|WKT  Coordinate system of intput points (default=EPSG:4326).\n"
              << "    --log=FILE_LOG                   Write logging information to FILE_LOG."
              << "    --output=FILE_OUTPUT             Write values to FILE_OUTPUT."
              << std::endl;
} // _printHelp


// End of file
