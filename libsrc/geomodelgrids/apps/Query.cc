#include <portinfo>

#include "Query.hh" // implementation of class methods

#include "geomodelgrids/serial/Query.hh" // USES Query

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
    _outputFilename(""),
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

    if (0 == _modelFilenames.size()) {
        std::cout << "WARNING: No models provided. Exiting." << std::endl;
        return 0;
    } // if

    geomodelgrids::serial::Query query;
    query.initialize(_modelFilenames, _valueNames);
    if (_squash) {
        query.setSquashMinElev(_squashMinElev);
    } // if

    std::ifstream sin(_pointsFilename);
    std::ofstream sout(_outputFilename);
    const size_t numQueryValues = _valueNames.size();
    std::vector<double> values(numQueryValues);
    while (!sin.eof() && sin.good()) {
        double longitude, latitude, elevation;
        sin >> longitude >> latitude >> elevation;

        query.query(&values[0], longitude, latitude, elevation);

        sout << longitude
             << latitude
             << elevation;
        for (size_t i = 0; i < numQueryValues; ++i) {
            sout << values[i];
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
    static struct option options[7] = {
        {"help", no_argument, NULL, 'h'},
        {"values", required_argument, NULL, 'v'},
        {"squash-min-elev", required_argument, NULL, 's'},
        {"points", required_argument, NULL, 'p'},
        {"output", required_argument, NULL, 'o'},
        {"models", required_argument, NULL, 'm'},
        {0, 0, 0, 0}
    };

    while (true) {
        // extern char* optarg;
        const char c = getopt_long(argc, argv, "hv:s:p:o:m:", options, NULL);
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
        case 'o': {
            _outputFilename = optarg;
            break;
        } // 'o'
        case 'm': {
            _modelFilenames.clear();
            std::istringstream tokenStream(optarg);
            std::string token;
            while (std::getline(tokenStream, token, ',')) {
                _modelFilenames.push_back(token);
            } // while
            break;
        } // 'm'
        case '?':
            break;
        default: {
            std::ostringstream msg;
            msg << "Error passing command line arguments:";
            for (int i = 0; i < argc; ++i) {
                msg << "    " << argv[i] << "\n";
            } // for
            msg << "Unknown option '" << c << "'";
            throw std::logic_error(msg.str().c_str());
        } // default
        } // switch
    } // while

    if (!_showHelp) { // Verify required arguments were provided.
        bool optionsOkay = true;
        std::ostringstream msg;
        if (_valueNames.empty()) {
            msg << "    - Missing names of values. Use --values=VALUE_0,...,VALUE_N\n";
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
            std::cerr << "Missing required command line arguments:\n"
                      << msg.str() << std::endl;
            exit(-1);
        } // if
    } // if
} // _parseArgs


// ---------------------------------------------------------------------------------------------------------------------
// Print help information.
void
geomodelgrids::apps::Query::_printHelp(void) {
    std::cout << "Usage: geogrids_query "
              << "[--help] [--values=VALUE_0,...,VALUE_N] [--squash-min-elev=ELEV] --models=FILE_0,...,FILE_M "
              << "--points=FILE_POINTS --output=FILE_OUTPUT\n\n"
              << "    --help                         Print help information to stdout and exit.\n"
              << "    --values=VALUE_0,...,VALUE_N   Values to return in query.\n"
              << "    --squash-min-elev=ELEV         Vertical coordinates is interpreted as -depth instead of "
              << "elevation if the elevation is above ELEV.\n"
              << "    --models=FILE_0,...,FILE_M     Models to query (in order).\n"
              << "    --points=FILE_POINTS           Read input points from FILE_POINTS.\n"
              << "    --output=FILE_OUTPUT           Write values to FILE_OUTPUT."
              << std::endl;
} // _printHelp


// End of file
