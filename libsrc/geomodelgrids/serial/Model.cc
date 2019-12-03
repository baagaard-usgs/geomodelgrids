#include <portinfo>

#include "Model.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Topography.hh" // USES Topography
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/utils/Projection.hh" // USES Projection

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <algorithm> // USES std::fill()
#include <cassert> // USES assert()
#include <cmath> // USES M_PI, cos(), sin()

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
    delete _projection;_projection = NULL;
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

    delete _info;_info = new geomodelgrids::serial::ModelInfo;assert(_info);
    _info->load(_h5);

    _h5->readAttribute("/", "data_values", &_valueNames);
    _h5->readAttribute("/", "data_units", &_valueUnits);

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
    std::sort(_blocks.begin(), _blocks.end(), Block::compare);

    // Initialize projection
    delete _projection;_projection = new geomodelgrids::utils::Projection();assert(_projection);
    _projection->fromWKT(_projectionString.c_str());
    _projection->initialize();
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
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    _toXYZ(&x, &y, &z, longitude, latitude, elevation);

    bool inModel = false;
    if (( x >= 0.0) && ( x <= _dims[0]) &&
        ( y >= 0.0) && ( y <= _dims[1]) &&
        ( z >= 0.0) && ( z <= _dims[2]) ) {
        inModel = true;
    } // if

    return inModel;
} // contains


// ---------------------------------------------------------------------------------------------------------------------
// Get model description.
double
geomodelgrids::serial::Model::queryElevation(const double longitude,
                                             const double latitude) {
    double elevation = 0;

    if (_topography) {
        double x = 0.0;
        double y = 0.0;
        _toXYZ(&x, &y, NULL, longitude, latitude, 0.0);
        elevation = _topography->query(x, y);
    } // if

    return elevation;
} // queryElevation


// ---------------------------------------------------------------------------------------------------------------------
// Get model description.
const double*
geomodelgrids::serial::Model::query(const double longitude,
                                    const double latitude,
                                    const double elevation) {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    _toXYZ(&x, &y, &z, longitude, latitude, elevation);
    assert(contains(longitude, latitude, elevation));

    geomodelgrids::serial::Block* block = _findBlock(x, y, z);assert(block);
    return block->query(x, y, z);
} // query


// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::Model::_toXYZ(double* x,
                                     double* y,
                                     double* z,
                                     const double longitude,
                                     const double latitude,
                                     const double elevation) const {
    assert(x);
    assert(y);
    double xProj = 0.0;
    double yProj = 0.0;
    _projection->project(&xProj, &yProj, longitude, latitude);
    const double yazimuthRad = _yazimuth * M_PI / 180.0;
    const double cosAz = cos(yazimuthRad);
    const double sinAz = sin(yazimuthRad);
    const double xRel = xProj - _origin[0];
    const double yRel = yProj - _origin[1];
    *x = xRel*cosAz - yRel*sinAz;
    *y = xRel*sinAz + yRel*cosAz;

    if (z) {
        double zGroundSurf = 0.0;
        if (_topography) {
            zGroundSurf = _topography->query(*x, *y);
        } // if
        const double zBottom = -_dims[2];
        *z = -zBottom / (zGroundSurf - zBottom) * (elevation - zBottom);
    } // if
} // _toXYZ


// ---------------------------------------------------------------------------------------------------------------------
geomodelgrids::serial::Block*
geomodelgrids::serial::Model::_findBlock(const double x,
                                         const double y,
                                         const double z) const {
    for (size_t i = 0; i < _blocks.size(); ++i) {
        geomodelgrids::serial::Block* block = _blocks[i];assert(block);
        if (( z <= block->getZTop()) && ( z >= block->getZBottom()) ) {
            return block;
        } // if
    } // for
    return NULL;
} // _findBlock


// End of file
