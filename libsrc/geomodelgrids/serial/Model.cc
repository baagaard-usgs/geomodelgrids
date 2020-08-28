#include <portinfo>

#include "Model.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Topography.hh" // USES Topography
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/utils/CRSTransformer.hh" // USES CRSTransformer

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <algorithm> // USES std::fill()
#include <cassert> // USES assert()
#include <cmath> // USES M_PI, cos(), sin()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Model::Model(void) :
    _modelCRSString(""),
    _inputCRSString("EPSG:4326"),
    _yazimuth(0.0),
    _h5(NULL),
    _info(NULL),
    _topography(NULL),
    _crsTransformer(NULL) {
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
// Set CRS of query input points.
void
geomodelgrids::serial::Model::setInputCRS(const std::string& value) {
    _inputCRSString = value;
} // setInputCRS


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
        throw std::logic_error(msg.str());
    } // switch

    _h5->open(filename, h5Mode);
} // open


// ---------------------------------------------------------------------------------------------------------------------
// Close Model file.
void
geomodelgrids::serial::Model::close(void) {
    if (_topography) {
        _topography->closeQuery();
    } // if
    size_t numBlocks = _blocks.size();
    for (size_t i = 0; i < numBlocks; ++i) {
        if (_blocks[i]) {
            _blocks[i]->closeQuery();
        } // if
    } // for

    if (_h5) {
        _h5->close();
        delete _h5;_h5 = NULL;
    } // if

    delete _info;_info = NULL;
    delete _crsTransformer;_crsTransformer = NULL;
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

    _modelCRSString = _h5->readAttribute("/", "crs");
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
} // loadMetadata


// ---------------------------------------------------------------------------------------------------------------------
// Initialize.
void
geomodelgrids::serial::Model::initialize(void) {
    // Initialize CRS transformation
    delete _crsTransformer;_crsTransformer = new geomodelgrids::utils::CRSTransformer();assert(_crsTransformer);
    _crsTransformer->setSrc(_inputCRSString.c_str());
    _crsTransformer->setDest(_modelCRSString.c_str());
    _crsTransformer->initialize();

    if (_topography) {
        _topography->openQuery(_h5);
    } // if
    size_t numBlocks = _blocks.size();
    for (size_t i = 0; i < numBlocks; ++i) {
        _blocks[i]->openQuery(_h5);
    } // for
} // initialize


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
// Get CRS for model.
const std::string&
geomodelgrids::serial::Model::getCRSString(void) const {
    return _modelCRSString;
} // getCRSTransformer


// ---------------------------------------------------------------------------------------------------------------------
// Get model description information.
const geomodelgrids::serial::ModelInfo*
geomodelgrids::serial::Model::getInfo(void) const {
    return _info;
} // getInfo


// ---------------------------------------------------------------------------------------------------------------------
// Get model topography.
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
geomodelgrids::serial::Model::contains(const double x,
                                       const double y,
                                       const double z) const {
    double xModel = 0.0;
    double yModel = 0.0;
    double zModel = 0.0;
    _toModelXYZ(&xModel, &yModel, &zModel, x, y, z);

    bool inModel = false;
    if (( xModel >= 0.0) && ( xModel <= _dims[0]) &&
        ( yModel >= 0.0) && ( yModel <= _dims[1]) &&
        ( zModel <= 0.0) && ( zModel >= -_dims[2]) ) {
        inModel = true;
    } // if

    return inModel;
} // contains


// ---------------------------------------------------------------------------------------------------------------------
// Get model description.
double
geomodelgrids::serial::Model::queryElevation(const double x,
                                             const double y) {
    double elevation = 0;

    if (_topography) {
        double xModel = 0.0;
        double yModel = 0.0;
        _toModelXYZ(&xModel, &yModel, NULL, x, y, 0.0);
        elevation = _topography->query(xModel, yModel);
    } // if

    return elevation;
} // queryElevation


// ---------------------------------------------------------------------------------------------------------------------
// Get model description.
const double*
geomodelgrids::serial::Model::query(const double x,
                                    const double y,
                                    const double z) {
    double xModel = 0.0;
    double yModel = 0.0;
    double zModel = 0.0;
    _toModelXYZ(&xModel, &yModel, &zModel, x, y, z);
    assert(contains(x, y, z));

    geomodelgrids::serial::Block* block = _findBlock(xModel, yModel, zModel);assert(block);
    return block->query(xModel, yModel, zModel);
} // query


// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::Model::_toModelXYZ(double* xModel,
                                          double* yModel,
                                          double* zModel,
                                          const double x,
                                          const double y,
                                          const double z) const {
    assert(xModel);
    assert(yModel);
    assert(_crsTransformer);

    double xModelCRS = 0.0;
    double yModelCRS = 0.0;
    double zModelCRS = 0.0;
    _crsTransformer->transform(&xModelCRS, &yModelCRS, &zModelCRS, x, y, z);
    const double yazimuthRad = _yazimuth * M_PI / 180.0;
    const double cosAz = cos(yazimuthRad);
    const double sinAz = sin(yazimuthRad);
    const double xRel = xModelCRS - _origin[0];
    const double yRel = yModelCRS - _origin[1];
    *xModel = xRel*cosAz - yRel*sinAz;
    *yModel = xRel*sinAz + yRel*cosAz;

    if (zModel) {
        double zGroundSurf = 0.0;
        if (_topography) {
            zGroundSurf = _topography->query(*xModel, *yModel);
        } // if
        const double zBottom = -_dims[2];
        *zModel = zBottom * (zGroundSurf - zModelCRS) / (zGroundSurf - zBottom);
    } // if
} // _toModelXYZ


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
