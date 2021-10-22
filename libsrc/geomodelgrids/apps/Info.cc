#include <portinfo>

#include "Info.hh" // implementation of class methods

#include "geomodelgrids/serial/Model.hh" // USES Model
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/serial/Surface.hh" // USES Surface
#include "geomodelgrids/utils/CRSTransformer.hh" // USES CRSTransformer

#include <cmath> // USES fabs()

#include <getopt.h> // USES getopt_long()
#include <iomanip>
#include <iostream> // USES std::cout
#include <cassert> // USES assert()
#include <sstream> // USES std::ostringstream, std::istringstream

// ------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace apps {
        namespace _Info {
            std::string join(const std::vector<std::string>& values,
                             const std::string& delimiter);

            std::string indent(const size_t level,
                               const size_t width=4);

            void verifyCoordSys(const geomodelgrids::serial::Model* model);

            void verifySurface(const geomodelgrids::serial::Surface* topography,
                               const geomodelgrids::serial::Model* model,
                               const char* const name);

            void verifyBlock(geomodelgrids::serial::Block* block,
                             const geomodelgrids::serial::Model* model);

            void verifyBlocksZ(const std::vector<geomodelgrids::serial::Block*>& blocks,
                               const double zBottom);

            const double* computeBoundingBox(const geomodelgrids::serial::Model* model);

        } // _Info
    } // apps
} // geomodelgrids

// ------------------------------------------------------------------------------------------------
// Constructor
geomodelgrids::apps::Info::Info() :
    _showHelp(false),
    _showAll(false),
    _showDescription(false),
    _showBlocks(false),
    _showCoordSys(false),
    _showValues(false),
    _doVerification(false) {}


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::apps::Info::~Info(void) {}


// ------------------------------------------------------------------------------------------------
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

        std::cout << "Model: " << _modelFilenames[i] << std::endl;
        if (_doVerification || _showAll) {
            _verify(&model);
        } else {
            try {
                model.loadMetadata();
            } catch (const std::exception& err) {
                std::cout << _Info::indent(1)
                          << "WARNING: Errors encountered while reading metadata. Information may be incomplete.\n";
                if (!_doVerification && !_showAll) {
                    std::cout << err.what();
                } // if
            } // try/catch
        } // if/else

        if (_showDescription || _showAll) { _printDescription(&model); }
        if (_showCoordSys || _showAll) { _printCoordSys(&model); }
        if (_showValues || _showAll) { _printValues(&model); }
        if (_showBlocks || _showAll) { _printBlocks(&model); }

        model.close();
    } // for

    return 0;
} // run


// ------------------------------------------------------------------------------------------------
// Parse command line arguments.
void
geomodelgrids::apps::Info::_parseArgs(int argc,
                                      char* argv[]) {
    static struct option options[9] = {
        {"help", no_argument, NULL, 'h'},
        {"description", no_argument, NULL, 'd'},
        {"blocks", no_argument, NULL, 'b'},
        {"coordsys", no_argument, NULL, 'c'},
        {"values", no_argument, NULL, 'v'},
        {"verify", no_argument, NULL, 'q'},
        {"all", no_argument, NULL, 'a'},
        {"models", required_argument, NULL, 'm'},
        {0, 0, 0, 0}
    };

    bool optionsOk = false;
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
            optionsOk = true;
            break;
        case 'd':
            _showDescription = true;
            optionsOk = true;
            break;
        case 'b':
            _showBlocks = true;
            optionsOk = true;
            break;
        case 'c':
            _showCoordSys = true;
            optionsOk = true;
            break;
        case 'v':
            _showValues = true;
            optionsOk = true;
            break;
        case 'q':
            _doVerification = true;
            optionsOk = true;
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
    if (!optionsOk) {
        _showHelp = true;
    } // if
    if (!_showHelp && (0 == _modelFilenames.size())) {
        throw std::runtime_error("Missing required command line argument --models=FILE_0,...,FILE_M.");
    } // if
} // _parseArgs


// ------------------------------------------------------------------------------------------------
// Print help information.
void
geomodelgrids::apps::Info::_printHelp(void) {
    std::cout << "Usage: geomodelgrids_info "
              << "[--help] --models=FILE_0,...,FILE_M "
              << "[--description] [--coordsys] [--values] [--blocks] [--all] [--verify]\n\n"
              << "    --help                       Print help information to stdout and exit.\n"
              << "    --models=FILE_0,...,FILE_M   Models to query (in order).\n"
              << "    --description                Display model description.\n"
              << "    --coordsys                   Display model coordinate system.\n"
              << "    --values                     Display names and units of values stored in the model.\n"
              << "    --blocks                     Display description of blocks.\n"
              << "    --all                        Display description, coordinate system, values, and blocks\n"
              << "    --verify                     Verify model conforms to GeoModelGrids specifications."
              << std::endl;
} // _printHelp


// ------------------------------------------------------------------------------------------------
// Print model description.
void
geomodelgrids::apps::Info::_printDescription(geomodelgrids::serial::Model* const model) {
    assert(model);

    const geomodelgrids::serial::ModelInfo* info = model->getInfo();assert(info);
    std::cout << _Info::indent(1) << "Title: " << info->getTitle() << "\n";
    std::cout << _Info::indent(1) << "Id: " << info->getId() << "\n";
    std::cout << _Info::indent(1) << "Description: " << info->getDescription() << "\n";
    std::cout << _Info::indent(1) << "Keywords: " << _Info::join(info->getKeywords(), ", ") << "\n";
    std::cout << _Info::indent(1) << "History: " << info->getHistory() << "\n";
    std::cout << _Info::indent(1) << "Comment: " << info->getComment() << "\n";

    std::cout << _Info::indent(1) << "Creator: "
              << info->getCreatorName() << ", "
              << info->getCreatorInstitution() << ", "
              << info->getCreatorEmail() << "\n";
    std::cout << _Info::indent(1) << "Authors: " << _Info::join(info->getAuthors(), "; ") << "\n";
    std::cout << _Info::indent(1) << "References:\n" << _Info::indent(2) <<
        _Info::join(info->getReferences(), std::string("\n") + _Info::indent(2)) << "\n";

    std::cout << _Info::indent(1) << "Acknowledgement: " << info->getAcknowledgement() << "\n";
    std::cout << _Info::indent(1) << "Repository: "
              << info->getRepositoryName()
              << " " << info->getRepositoryURL() << "\n";
    std::cout << _Info::indent(1) << "DOI: " << info->getRepositoryDOI() << "\n";
    std::cout << _Info::indent(1) << "Version: " << info->getVersion() << "\n";
    std::cout << _Info::indent(1) << "License: " << info->getLicense() << "\n";
    const std::string& auxiliary = info->getAuxiliary();
    if (!auxiliary.empty()) {
        std::cout << _Info::indent(1) << "Auxiliary information: " << auxiliary << "\n";
    } // if

    const double* dims = model->getDims();
    std::cout << _Info::indent(1) << "Dimensions of model: x=" << dims[0] << ", y="<< dims[1] << ", z=" << dims[2] << "\n";

    const double* bbox = _Info::computeBoundingBox(model);
    std::cout << _Info::indent(1) << "Bounding box (WGS84):" << std::resetiosflags(std::ios::fixed);
    for (size_t i = 0; i < 4; ++i) {
        const size_t dim = 2;
        std::cout << " ("
                  << std::setprecision(6)
                  << bbox[i*dim+0]
                  << ", "
                  << std::setprecision(7)
                  << bbox[i*dim+1]
                  << ")";
    } // for
    std::cout << "\n";
} // _printDescription


// ------------------------------------------------------------------------------------------------
// Print model coordinate system.
void
geomodelgrids::apps::Info::_printCoordSys(geomodelgrids::serial::Model* const model) {
    assert(model);

    std::cout << _Info::indent(1) << "Coordinate system:\n";

    std::cout << _Info::indent(2) << "CRS (PROJ, EPSG, WKT): " << model->getCRSString() << "\n";

    std::string xUnit;
    std::string yUnit;
    std::string zUnit;
    geomodelgrids::utils::CRSTransformer::getCRSUnits(&xUnit, &yUnit, &zUnit, model->getCRSString().c_str());
    std::cout << _Info::indent(2) << "Coordinate system units:"
              << " x=" << xUnit << ", y=" << yUnit << ", z=" << zUnit << "\n";

    const double* origin = model->getOrigin();
    std::cout << _Info::indent(2) << "Origin: x=" << origin[0] <<", y=" << origin[1] << "\n";

    std::cout << _Info::indent(2) << "Azimuth (degrees) of y axis from north: " << model->getYAzimuth() << "\n";
} // _printCoordSys


// ------------------------------------------------------------------------------------------------
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

    switch (model->getDataLayout()) {
    case geomodelgrids::serial::Model::VERTEX:
        std::cout << _Info::indent(2) << "Vertex-based data\n";
        break;
    case geomodelgrids::serial::Model::CELL:
        std::cout << _Info::indent(2) << "Cell-based data\n";
        break;
    default:
        std::cout << _Info::indent(2) << "Unknown data layout\n";
    }
} // _printValues


// ------------------------------------------------------------------------------------------------
// Print description of model blocks.
void
geomodelgrids::apps::Info::_printBlocks(geomodelgrids::serial::Model* const model) {
    assert(model);

    std::cout << _Info::indent(1) << "Surfaces\n";
    std::cout << _Info::indent(2) << "Top surface:\n";
    const geomodelgrids::serial::Surface* surfaceTop = model->getTopSurface();
    if (surfaceTop) {
        const size_t* dims = surfaceTop->getDims();
        std::cout << _Info::indent(3) << "Number of points: x=" << dims[0] << ", y=" << dims[1] << "\n";
        std::cout << _Info::indent(3) << "Resolution: "
                  << "x=" << surfaceTop->getResolutionX()
                  << ", y=" << surfaceTop->getResolutionY() << "\n";
    } else {
        std::cout << _Info::indent(3) << "None\n";
    } // if/else

    std::cout << _Info::indent(2) << "Topography/bathymetry:\n";
    const geomodelgrids::serial::Surface* surfaceTopoBathy = model->getTopoBathy();
    if (surfaceTopoBathy) {
        const size_t* dims = surfaceTopoBathy->getDims();
        std::cout << _Info::indent(3) << "Number of points: x=" << dims[0] << ", y=" << dims[1] << "\n";
        std::cout << _Info::indent(3) << "Resolution: "
                  << "x=" << surfaceTopoBathy->getResolutionX()
                  << ", y=" << surfaceTopoBathy->getResolutionY() << "\n";
    } else {
        std::cout << _Info::indent(3) << "None\n";
    } // if/else

    const std::vector<geomodelgrids::serial::Block*>& blocks = model->getBlocks();
    const size_t numBlocks = blocks.size();
    std::cout << _Info::indent(1) << "Blocks ("<< numBlocks << ")\n";
    for (size_t i = 0; i < numBlocks; ++i) {
        assert(blocks[i]);
        std::cout << _Info::indent(2) << "Block '" << blocks[i]->getName() << "'\n";
        std::cout << _Info::indent(3) << "Resolution: "
                  << "x=" << blocks[i]->getResolutionX()
                  << ", y=" << blocks[i]->getResolutionY()
                  << ", z=" << blocks[i]->getResolutionZ() << "\n";
        std::cout << _Info::indent(3) << "Elevation of top of block in logical space: " << blocks[i]->getZTop() << "\n";

        const size_t* dims = blocks[i]->getDims();
        std::cout << _Info::indent(3) << "Number of points: x=" << dims[0] << ", y=" << dims[1] << ", z=" << dims[2] << "\n";

        const double dim_x = blocks[i]->getResolutionX() * (dims[0] - 1);
        const double dim_y = blocks[i]->getResolutionY() * (dims[1] - 1);
        const double dim_z = blocks[i]->getResolutionZ() * (dims[2] - 1);
        std::cout << _Info::indent(3) << "Dimensions: x=" << dim_x << ", y=" << dim_y << ", z=" << dim_z << "\n";
    } // for
} // _printBlocks


// ------------------------------------------------------------------------------------------------
// Verify presence of metadata and consistency of model.
void
geomodelgrids::apps::Info::_verify(geomodelgrids::serial::Model* const model) {
    assert(model);

    std::cout << _Info::indent(1) << "Verification\n"
              << _Info::indent(2) << "Verifying metadata...";
    bool ok = true;

    try {
        model->loadMetadata();
    } catch (const std::runtime_error& err) {
        std::cout << "FAIL\n" << err.what();
        ok = false;
    } // try/catch
    if (ok) { std::cout << "OK\n"; }
    _Info::verifyCoordSys(model);

    const geomodelgrids::serial::Surface* surfaceTop = model->getTopSurface();
    if (surfaceTop) { _Info::verifySurface(surfaceTop, model, "top surface"); }

    const geomodelgrids::serial::Surface* surfaceTopoBathy = model->getTopoBathy();
    if (surfaceTopoBathy) { _Info::verifySurface(surfaceTopoBathy, model, "topography/bathymetry"); }

    if (surfaceTop && surfaceTopoBathy) {
        std::cout << _Info::indent(2) << "Verifying resolution of top surface matches resolution of topography/bathymetry...";
        const double topResolutionX = surfaceTop->getResolutionX();
        const double topResolutionY = surfaceTop->getResolutionY();
        const double topoResolutionX = surfaceTopoBathy->getResolutionX();
        const double topoResolutionY = surfaceTopoBathy->getResolutionY();
        if ((fabs(1.0 - topoResolutionX/topResolutionX) > 1.0e-4) ||
            (fabs(1.0 - topoResolutionY/topResolutionY) > 1.0e-4)) {
            std::cout << "FAIL\n"
                      << _Info::indent(3) << "Resolution of top surface: x=" << topResolutionX
                      << ", y=" << topResolutionY
                      << "; resolution of topography/bathymetry: x=" << topoResolutionX
                      << ", y=" << topoResolutionY << "\n";
        } else {
            std::cout << "OK\n";
        } // if/else
    } // if

    const std::vector<geomodelgrids::serial::Block*>& blocks = model->getBlocks();
    const size_t numBlocks = blocks.size();
    for (size_t i = 0; i < numBlocks; ++i) {
        _Info::verifyBlock(blocks[i], model);
    } // for

    const double zBottom = -model->getDims()[2];
    _Info::verifyBlocksZ(blocks, zBottom);
} // _verify


// ------------------------------------------------------------------------------------------------
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


// ------------------------------------------------------------------------------------------------
std::string
geomodelgrids::apps::_Info::indent(const size_t level,
                                   const size_t width) {
    std::ostringstream buffer;
    buffer << std::setw(level*width) << " ";
    return buffer.str();
} // indent


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_Info::verifyCoordSys(const geomodelgrids::serial::Model* model) {
    std::cout << _Info::indent(2) << "Verifying model coordinate system ...";

    bool ok = true;

    std::string xUnit;
    std::string yUnit;
    geomodelgrids::utils::CRSTransformer::getCRSUnits(&xUnit, &yUnit, NULL,
                                                      model->getCRSString().c_str());
    if (xUnit != yUnit) {
        if (ok) { std::cout << "FAIL\n"; }
        std::cout << _Info::indent(3) << "Units for x (" << xUnit << ") and y (" << yUnit
                  << ") in model coordinate system do not match.\n";
        ok = false;
    } // if

    if (ok) { std::cout << "OK\n"; }
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_Info::verifySurface(const geomodelgrids::serial::Surface* surface,
                                          const geomodelgrids::serial::Model* model,
                                          const char* const name) {
    assert(surface);
    assert(model);
    assert(name);

    std::cout << _Info::indent(2) << "Verifying surface '" << name << "'...";
    bool ok = true;

    const double* dimsDomain = model->getDims();
    double resolution[2];
    resolution[0] = surface->getResolutionX();
    resolution[1] = surface->getResolutionY();
    const size_t* numTopo = surface->getDims();

    const double tolerance = 1.0e-6;
    const std::string dimLabel[2] = { "x", "y" };
    for (size_t iDim = 0; iDim < 2; ++iDim) {
        if (fabs(1.0 - resolution[iDim]*(numTopo[iDim]-1)/dimsDomain[iDim]) > tolerance) {
            if (ok) { std::cout << "FAIL\n"; }
            std::cout << _Info::indent(3) << "Surface '" << name << "' does not span the domain in the " << dimLabel[iDim]
                      << " dimension (surface=" << resolution[iDim]*numTopo[iDim] << ", domain=" << dimsDomain[iDim] << ").\n";
            ok = false;
        } // if
    } // for

    if (ok) { std::cout << "OK\n"; }
}


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_Info::verifyBlock(geomodelgrids::serial::Block* block,
                                        const geomodelgrids::serial::Model* model) {
    assert(block);
    assert(model);

    std::cout << _Info::indent(2) << "Verifying block '" << block->getName() << "'...";
    bool ok = true;

    const double* dimsDomain = model->getDims();
    double resolution[2];
    resolution[0] = block->getResolutionX();
    resolution[1] = block->getResolutionY();
    const size_t* numBlock = block->getDims();

    const double tolerance = 1.0e-6;

    // Verify block spans the domain.
    const std::string dimLabel[2] = { "x", "y" };
    for (size_t iDim = 0; iDim < 2; ++iDim) {
        if (fabs(1.0 - resolution[iDim]*(numBlock[iDim]-1)/dimsDomain[iDim]) > tolerance) {
            if (ok) { std::cout << "FAIL\n"; }
            std::cout << _Info::indent(3) << "Block '" << block->getName() << "' does not span the domain in the "
                      << dimLabel[iDim] << " dimension (block=" << resolution[iDim]*numBlock[iDim] << ", domain="
                      << dimsDomain[iDim] << ").\n";
            ok = false;
        } // if
    } // for

    // Verify block resolution is integer multiple of topography resolution.
    const geomodelgrids::serial::Surface* surfaceTop = model->getTopSurface();
    if (surfaceTop) {
        double topoResolution[2];
        topoResolution[0] = surfaceTop->getResolutionX();
        topoResolution[1] = surfaceTop->getResolutionY();
        for (size_t iDim = 0; iDim < 2; ++iDim) {
            if (topoResolution[iDim] <= 0.0) { continue; }
            const int num_skip = int(0.01 + resolution[iDim] / topoResolution[iDim]);
            if (fabs(num_skip * topoResolution[iDim] - resolution[iDim]) > tolerance) {
                if (ok) { std::cout << "FAIL\n"; }
                std::cout << _Info::indent(3) << "Resolution of block '" << block->getName() << "' ("
                          << resolution[iDim] << ") is not an integer multiple of the topography resolution (" << topoResolution[iDim]
                          << ").\n";
                ok = false;
            } // if
        } // if
    } // if

    if (ok) { std::cout << "OK\n"; }
}


// ------------------------------------------------------------------------------------------------
// Verify blocks span vertical dimension of domain. Blocks are ordered top to bottom.
void
geomodelgrids::apps::_Info::verifyBlocksZ(const std::vector<geomodelgrids::serial::Block*>& blocks,
                                          const double zBottom) {
    // Blocks are ordered top to bottom.

    std::cout << _Info::indent(2) << "Verifying blocks span vertical dimension of domain...";
    bool ok = true;

    const double tolerance = 1.0e-4;
    const size_t numBlocks = blocks.size();

    if (numBlocks > 0) {
        assert(blocks[0]);
        if (fabs(blocks[0]->getZTop()) > tolerance) {
            std::cout << "FAIL\n"
                      << _Info::indent(3) << "Top block '" << blocks[0]->getName() << "' does not reach top of domain (z=0).\n";
            ok = false;
        } // if
    } // if

    for (size_t i = 1; i < numBlocks; ++i) {
        assert(blocks[i]);
        const double zBot = blocks[i-1]->getZBottom();
        const double zTop = blocks[i]->getZTop();
        if (fabs(zBot-zTop) > tolerance) {
            if (ok) { std::cout << "FAIL\n"; }
            std::cout << _Info::indent(3) << "Found vertical gap between blocks '" << blocks[i-1]->getName()
                      << "' (zBottom=" << zBot << ") and '" << blocks[i]->getName() << "' (zTop=" << zTop << ").\n";
            ok = false;
        } // if
    } // for

    if (numBlocks > 0) {
        if (fabs(blocks[numBlocks-1]->getZBottom()-zBottom) > tolerance) {
            if (ok) { std::cout << "FAIL\n"; }
            std::cout << _Info::indent(3) << "Bottom block '" << blocks[numBlocks-1]->getName()
                      << "' does not reach bottom of domain (z=" << zBottom << ").\n";
            ok = false;
        } // if
    } // if

    if (ok) { std::cout << "OK\n"; }
}


// ------------------------------------------------------------------------------------------------
const double*
geomodelgrids::apps::_Info::computeBoundingBox(const geomodelgrids::serial::Model* model) {
    assert(model);

    const double* dims = model->getDims();assert(dims);
    const double* origin = model->getOrigin();assert(origin);
    const double yazimuth = model->getYAzimuth();
    const std::string& crsString = model->getCRSString();

    const size_t npts = 4;
    const size_t dim = 2;
    const double thetaR = (360.0 - yazimuth) / 180.0 * M_PI;
    const double cosaz = cos(thetaR);
    const double sinaz = sin(thetaR);

    static double bbox_model[npts*dim];
    bbox_model[0*dim+0] = origin[0];
    bbox_model[0*dim+1] = origin[1];
    bbox_model[1*dim+0] = origin[0] + dims[0]*cosaz;
    bbox_model[1*dim+1] = origin[1] + dims[0]*sinaz;
    bbox_model[2*dim+0] = origin[0] + dims[0]*cosaz - dims[1]*sinaz;
    bbox_model[2*dim+1] = origin[1] + dims[0]*sinaz + dims[1]*cosaz;
    bbox_model[3*dim+0] = origin[0] - dims[1]*sinaz;
    bbox_model[3*dim+1] = origin[1] + dims[1]*cosaz;

    static double bbox_geo[npts*dim];
    geomodelgrids::utils::CRSTransformer transformer;
    transformer.setSrc(crsString.c_str());
    transformer.setDest("EPSG:4326");
    transformer.initialize();
    for (size_t i = 0; i < npts; ++i) {
        transformer.transform(&bbox_geo[i*dim+0], &bbox_geo[i*dim+1], NULL, bbox_model[i*dim+0], bbox_model[i*dim+1], 0.0);
    } // for

    return bbox_geo;
}


// End of file
