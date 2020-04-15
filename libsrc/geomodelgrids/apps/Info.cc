#include <portinfo>

#include "Info.hh" // implementation of class methods

#include "geomodelgrids/serial/Model.hh" // USES Model
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/serial/Topography.hh" // USES Topography

#include <getopt.h> // USES getopt_long()
#include <iomanip>
#include <iostream> // USES std::cout
#include <cassert> // USES assert()
#include <sstream> // USES std::ostringstream, std::istringstream

// ---------------------------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace apps {
        namespace _Info {
            std::string join(const std::vector<std::string>& values,
                             const std::string& delimiter);

            std::string indent(const size_t level,
                               const size_t width=4);

        } // _Info
    } // apps
} // geomodelgrids

// ---------------------------------------------------------------------------------------------------------------------
// Constructor
geomodelgrids::apps::Info::Info() :
    _showHelp(false),
    _showAll(false),
    _showDescription(false),
    _showBlocks(false),
    _showCoordSys(false),
    _showValues(false) {}


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::apps::Info::~Info(void) {}


// ---------------------------------------------------------------------------------------------------------------------
// Run info application.
int
geomodelgrids::apps::Info::run(int argc,
                               char* argv[]) {
    _parseArgs(argc, argv);

    if (_showHelp) {
        _printHelp();
        return 0;
    } // if

    geomodelgrids::serial::Model model;
    const size_t numModels = _modelFilenames.size();
    for (size_t i = 0; i < numModels; ++i) {
        model.open(_modelFilenames[i].c_str(), geomodelgrids::serial::Model::READ);
        model.loadMetadata();

        std::cout << "Model: " << _modelFilenames[i] << std::endl;
        if (_showDescription || _showAll) { _printDescription(&model); }
        if (_showCoordSys || _showAll) { _printCoordSys(&model); }
        if (_showValues || _showAll) { _printValues(&model); }
        if (_showBlocks || _showAll) { _printBlocks(&model); }

        model.close();
    } // for

    return 0;
} // run


// ---------------------------------------------------------------------------------------------------------------------
// Parse command line arguments.
void
geomodelgrids::apps::Info::_parseArgs(int argc,
                                      char* argv[]) {
    static struct option options[8] = {
        {"help", no_argument, NULL, 'h'},
        {"description", no_argument, NULL, 'd'},
        {"blocks", no_argument, NULL, 'b'},
        {"coordsys", no_argument, NULL, 'c'},
        {"values", no_argument, NULL, 'v'},
        {"all", no_argument, NULL, 'a'},
        {"models", required_argument, NULL, 'm'},
        {0, 0, 0, 0}
    };

    while (true) {
        // extern char* optarg;
        const char c = getopt_long(argc, argv, "hdbcvam:", options, NULL);
        if (-1 == c) { break; }
        switch (c) {
        case 'h':
            _showHelp = true;
            break;
        case 'a':
            _showAll = true;
            break;
        case 'd':
            _showDescription = true;
            break;
        case 'b':
            _showBlocks = true;
            break;
        case 'c':
            _showCoordSys = true;
            break;
        case 'v':
            _showValues = true;
            break;
        case 'm': {
            _modelFilenames.clear();
            std::istringstream tokenStream(optarg);
            if (tokenStream.str().find(",") != std::string::npos) {
                std::string token;
                while (std::getline(tokenStream, token, ',')) {
                    _modelFilenames.push_back(token);
                } // while
            } else {
                _modelFilenames.push_back(optarg);
            } // if/else
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
    if (!_showHelp && !_showAll && !_showDescription && !_showCoordSys && !_showValues && !_showBlocks) {
        _showHelp = true;
    } // if
    if (!_showHelp && (0 == _modelFilenames.size())) {
        throw std::runtime_error("Missing required command line argument --models=FILE_0,...,FILE_M.");
    } // if
} // _parseArgs


// ---------------------------------------------------------------------------------------------------------------------
// Print help information.
void
geomodelgrids::apps::Info::_printHelp(void) {
    std::cout << "Usage: geogrids_info "
              << "[--help] [--description] [--coordsys] [--values] [--blocks] [--all] --models=FILE_0,...,FILE_M\n\n"
              << "    --help    Print help information to stdout and exit.\n"
              << "    --description    Print model description.\n"
              << "    --coordsys       Print model coordinate system.\n"
              << "    --values         Print names and units of values stored in the model.\n"
              << "    --blocks         Print description of blocks.\n"
              << "    --all            Print description, coordinate system, values, and blocks"
              << std::endl;
} // _printHelp


// ---------------------------------------------------------------------------------------------------------------------
// Print model description.
void
geomodelgrids::apps::Info::_printDescription(geomodelgrids::serial::Model* const model) {
    assert(model);

    const geomodelgrids::serial::ModelInfo* info = model->getInfo();assert(info);
    std::cout << _Info::indent(1) << "Title: " << info->getTitle() << "\n";
    std::cout << _Info::indent(1) << "Id: " << info->getId() << "\n";
    std::cout << _Info::indent(1) << "Description: " << info->getDescription() << "\n";
    std::cout << _Info::indent(1) << "Keywords: " << _Info::join(info->getKeywords(), ", ") << "\n";

    std::cout << _Info::indent(1) << "Creator: "
              << info->getCreatorName() << ", "
              << info->getCreatorInstitution() << ", "
              << info->getCreatorEmail() << "\n";
    std::cout << _Info::indent(1) << "Authors: " << _Info::join(info->getAuthors(), "; ") << "\n";
    std::cout << _Info::indent(1) << "References:\n" << _Info::indent(2) <<
        _Info::join(info->getReferences(), std::string("\n") + _Info::indent(2)) << "\n";

    std::cout << _Info::indent(1) << "Acknowledgments: " << info->getAcknowledgments() << "\n";
    std::cout << _Info::indent(1) << "DOI: " << info->getDOI() << "\n";
    std::cout << _Info::indent(1) << "Version: " << info->getVersion() << "\n";

    const double* dims = model->getDims();
    std::cout << _Info::indent(1) << "Dimensions of model (m): x=" << dims[0] << ", y="<< dims[1] << ", z=" << dims[2] << "\n";
} // _printDescription


// ---------------------------------------------------------------------------------------------------------------------
// Print model coordinate system.
void
geomodelgrids::apps::Info::_printCoordSys(geomodelgrids::serial::Model* const model) {
    assert(model);

    std::cout << _Info::indent(1) << "Coordinate system:\n";

    std::cout << _Info::indent(2) << "CRS (PROJ, EPSG, WKT): " << model->getCRSString() << "\n";

    const double* origin = model->getOrigin();
    std::cout << _Info::indent(2) << "Origin: x=" << origin[0] <<", y=" << origin[1] << "\n";

    std::cout << _Info::indent(2) << "Azimuth (degrees) of y axis from north: " << model->getYAzimuth() << "\n";
} // _printCoordSys


// ---------------------------------------------------------------------------------------------------------------------
// Print model coordinate system.
void
geomodelgrids::apps::Info::_printValues(geomodelgrids::serial::Model* const model) {
    assert(model);

    std::cout << _Info::indent(1) << "Values stored in model:\n";

    const std::vector<std::string>& names = model->getValueNames();
    const std::vector<std::string>& units = model->getValueUnits();
    const size_t size = names.size();
    assert(units.size() == size);
    for (size_t i = 0; i < size; ++i) {
        std::cout << _Info::indent(2) << i << ": " << names[i] << " (" << units[i] << ")" << "\n";
    } // for
} // _printValues


// ---------------------------------------------------------------------------------------------------------------------
// Print description of model blocks.
void
geomodelgrids::apps::Info::_printBlocks(geomodelgrids::serial::Model* const model) {
    assert(model);

    std::cout << _Info::indent(1) << "Topography:\n";
    const geomodelgrids::serial::Topography* topography = model->getTopography();
    if (topography) {
        std::cout << _Info::indent(2) << "Horizontal resolution (m): " << topography->getResolutionHoriz() << "\n";
    } else {
        std::cout << _Info::indent(2) << "None\n";
    } // if/else

    const std::vector<geomodelgrids::serial::Block*>& blocks = model->getBlocks();
    const size_t numBlocks = blocks.size();
    std::cout << _Info::indent(1) << "Blocks ("<< numBlocks << ")\n";
    for (size_t i = 0; i < numBlocks; ++i) {
        assert(blocks[i]);
        std::cout << _Info::indent(2) << "Block '" << blocks[i]->getName() << "'\n";
        std::cout << _Info::indent(3) << "Resolution (m): horizontal="
                  << blocks[i]->getResolutionHoriz()
                  << ", vertical=" << blocks[i]->getResolutionVert() << "\n";
        std::cout << _Info::indent(3) << "Elevation (m) of top of block in logical space: " << blocks[i]->getZTop() << "\n";

        const size_t* dims = blocks[i]->getDims();
        std::cout << _Info::indent(3) << "Number of points: x=" << dims[0] << ", y=" << dims[1] << ", z=" << dims[2] << "\n";

        const double dim_x = blocks[i]->getResolutionHoriz() * (dims[0] - 1);
        const double dim_y = blocks[i]->getResolutionHoriz() * (dims[1] - 1);
        const double dim_z = blocks[i]->getResolutionVert() * (dims[2] - 1);
        std::cout << _Info::indent(3) << "Dimensions (m): x=" << dim_x << ", y=" << dim_y << ", z=" << dim_z << "\n";
    } // for
} // _printBlocks


// ---------------------------------------------------------------------------------------------------------------------
std::string
geomodelgrids::apps::_Info::join(const std::vector<std::string>& values,
                                 const std::string& delimiter) {
    std::ostringstream sout;
    const size_t size = values.size();
    for (size_t i = 0; i < size; ++i) {
        sout << values[i];
        if (i < size-1) {
            sout << delimiter;
        } // if
    } // for
    return sout.str();
} // join


// ---------------------------------------------------------------------------------------------------------------------
std::string
geomodelgrids::apps::_Info::indent(const size_t level,
                                   const size_t width) {
    std::ostringstream buffer;
    buffer << std::setw(level*width) << " ";
    return buffer.str();
} // indent


// End of file
