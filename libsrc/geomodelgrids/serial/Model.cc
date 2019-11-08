#include <portinfo>

#include "Model.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Topography.hh" // USES Topography
#include "geomodelgrids/serial/Block.hh" // USES Block

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <algorithm> // USES std::fill()
#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Model::Model(void) :
    _projectionString(""),
    _yazimuth(0.0),
    _h5(NULL),
    _info(NULL),
    _topography(NULL) {
    _origin[0] = 0.0;
    _origin[1] = 0.0;
    _dims[0] = 0.0;
    _dims[1] = 0.0;
    _dims[2] = 0.0;
} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Model::~Model(void) {
    this->close();
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Open Model file.
void
geomodelgrids::serial::Model::open(const char* filename,
                                   ModelMode mode) {
    assert(filename);

    delete _h5;_h5 = new geomodelgrids::serial::HDF5();assert(_h5);
    hid_t h5Mode = H5F_ACC_RDONLY;
    switch (mode) {
    case READ:
        h5Mode = H5F_ACC_RDONLY;
        break;
    case READ_WRITE:
        h5Mode = H5F_ACC_RDWR;
        break;
    case READ_WRITE_TRUNCATE:
        h5Mode = H5F_ACC_TRUNC;
        break;
    default:
        std::ostringstream msg;
        msg << "Unknown mode '" << mode << "' opening HDF5 file '" << filename << "'.";
        assert(0);
        throw std::runtime_error(msg.str());
    } // switch

    _h5->open(filename, h5Mode);
} // open


// ---------------------------------------------------------------------------------------------------------------------
// Close Model file.
void
geomodelgrids::serial::Model::close(void) {
    if (_h5) {
        _h5->close();
        delete _h5;_h5 = NULL;
    } // if

    delete _info;_info = NULL;
    // delete _projection; _projection = NULL;
    delete _topography;_topography = NULL;
    for (size_t i = 0; i < _blocks.size(); ++i) {
        delete _blocks[i];_blocks[i] = NULL;
    } // for
} // close


// ---------------------------------------------------------------------------------------------------------------------
// Load metadata.
void
geomodelgrids::serial::Model::loadMetadata(void) {
    if (!_h5) {
        throw std::logic_error("Model not open. Call open() before loading metadata.");
    } // if

    delete _info; _info = new geomodelgrids::serial::ModelInfo;assert(_info);
    _info->load(_h5);

    // :TODO: Names and units of values

    _h5->readAttribute("/", "dim_x", H5T_NATIVE_DOUBLE, (void*)&_dims[0]);
    _h5->readAttribute("/", "dim_y", H5T_NATIVE_DOUBLE, (void*)&_dims[1]);
    _h5->readAttribute("/", "dim_z", H5T_NATIVE_DOUBLE, (void*)&_dims[2]);

    _projectionString = _h5->readAttribute("/", "projection");
    _h5->readAttribute("/", "origin_x", H5T_NATIVE_DOUBLE, (void*)&_origin[0]);
    _h5->readAttribute("/", "origin_y", H5T_NATIVE_DOUBLE, (void*)&_origin[1]);
    _h5->readAttribute("/", "y_azimuth", H5T_NATIVE_DOUBLE, (void*)&_yazimuth);

    delete _topography;_topography = NULL;
    if (_h5->hasDataset("topography")) {
        _topography = new geomodelgrids::serial::Topography();assert(_topography);
        _topography->loadMetadata(_h5);
    } // if

    if (_blocks.size() > 0) {
        for (size_t i = 0; i < _blocks.size(); ++i) {
            delete _blocks[i];_blocks[i] = NULL;
        } // for
        _blocks.clear();
    } // if
    std::vector<std::string> blockNames;
    _h5->getGroupDatasets(&blockNames, "blocks");
    const size_t numBlocks = blockNames.size();

    _blocks.resize(numBlocks);
    for (size_t i = 0; i < numBlocks; ++i) {
        _blocks[i] = new geomodelgrids::serial::Block(blockNames[i].c_str());assert(_blocks[i]);
    } // for
    for (size_t i = 0; i < numBlocks; ++i) {
        _blocks[i]->loadMetadata(_h5);
    } // for
} // loadMetadata


// ---------------------------------------------------------------------------------------------------------------------
// Get names of values in model.
const std::vector<std::string>&
geomodelgrids::serial::Model::getValueNames(void) const {
    return _valueNames;
} // getValueNames


// ---------------------------------------------------------------------------------------------------------------------
// Get names of units in model.
const std::vector<std::string>&
geomodelgrids::serial::Model::getValueUnits(void) const {
    return _valueUnits;
} // getValueUnits


// ---------------------------------------------------------------------------------------------------------------------
// Get model dimensions.
const double*
geomodelgrids::serial::Model::getDims(void) const {
    return _dims;
} // getDims


// ---------------------------------------------------------------------------------------------------------------------
// Get coordinates of model origin.
const double*
geomodelgrids::serial::Model::getOrigin(void) const {
    return _origin;
} // getOrigin


// ---------------------------------------------------------------------------------------------------------------------
// Get azimuth of y coordinate axies.
const double
geomodelgrids::serial::Model::getYAzimuth(void) const {
    return _yazimuth;
} // getYAzimuth


// ---------------------------------------------------------------------------------------------------------------------
// Get geographic projection for model.
const std::string&
geomodelgrids::serial::Model::getProjectionString(void) const {
    return _projectionString;
} // getProjection


// ---------------------------------------------------------------------------------------------------------------------
// Get model description information.
const geomodelgrids::serial::ModelInfo*
geomodelgrids::serial::Model::getInfo(void) const {
    return _info;
} // getInfo


// ---------------------------------------------------------------------------------------------------------------------
// Get model description.
const geomodelgrids::serial::Topography*
geomodelgrids::serial::Model::getTopography(void) const {
    return _topography;
} // getTopography


// ---------------------------------------------------------------------------------------------------------------------
// Get model description.
const std::vector<geomodelgrids::serial::Block*>&
geomodelgrids::serial::Model::getBlocks(void) const {
    return _blocks;
} // getBlocks


// ---------------------------------------------------------------------------------------------------------------------
// Does model
bool
geomodelgrids::serial::Model::contains(const double longitude,
                                       const double latitude,
                                       const double elevation) const {
    bool inModel = false;

    double xModel = 0.0;
    double yModel = 0.0;
    double zModel = 0.0;
    // _projection->project(&xyzProject);
    // Geographic coordinates to projection coordinates
    // :TODO: @brad ADD STUFF HERE

    // Projection coordinates to model coordinates

    if (( xModel >= 0.0) && ( xModel <= _dims[0]) &&
        ( yModel >= 0.0) && ( yModel <= _dims[1]) &&
        ( zModel >= 0.0) && ( zModel <= _dims[2]) ) {
        inModel = true;
    } // if

    return inModel;
} // contains


// ---------------------------------------------------------------------------------------------------------------------
// Get model description.
double
geomodelgrids::serial::Model::queryElevation(const double longitude,
                                             const double latitude) const {
    double elevation = 0;

    return elevation;
} // queryElevation


// ---------------------------------------------------------------------------------------------------------------------
// Get model description.
const double*
geomodelgrids::serial::Model::query(const double longitude,
                                    const double latitude,
                                    const double elevation) const {
    return NULL;
} // query


// End of file
