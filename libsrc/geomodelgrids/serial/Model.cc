#include <portinfo>

#include "Model.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Surface.hh" // USES Surface
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/utils/CRSTransformer.hh" // USES CRSTransformer
#include "geomodelgrids/utils/constants.hh" // USES TOLERANCE

#include <cstring> // USES strlen()
#include <strings.h> // USES strcasecmp()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <algorithm> // USES std::fill()
#include <cassert> // USES assert()
#include <cmath> // USES M_PI, cos(), sin()

// ------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::Model::Model(void) :
    _layout(VERTEX),
    _modelCRSString(""),
    _inputCRSString("EPSG:4326"),
    _yazimuth(0.0),
    _h5(NULL),
    _info(NULL),
    _surfaceTop(NULL),
    _surfaceTopoBathy(NULL),
    _crsTransformer(NULL) {
    _origin[0] = 0.0;
    _origin[1] = 0.0;
    _dims[0] = 0.0;
    _dims[1] = 0.0;
    _dims[2] = 0.0;
} // constructor


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Model::~Model(void) {
    this->close();
} // destructor


// ------------------------------------------------------------------------------------------------
// Set CRS of query input points.
void
geomodelgrids::serial::Model::setInputCRS(const std::string& value) {
    _inputCRSString = value;
} // setInputCRS


// ------------------------------------------------------------------------------------------------
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


// ------------------------------------------------------------------------------------------------
// Close Model file.
void
geomodelgrids::serial::Model::close(void) {
    if (_surfaceTop) {
        _surfaceTop->closeQuery();
    } // if
    if (_surfaceTopoBathy) {
        _surfaceTopoBathy->closeQuery();
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
    delete _surfaceTop;_surfaceTop = NULL;
    delete _surfaceTopoBathy;_surfaceTopoBathy = NULL;
    for (size_t i = 0; i < _blocks.size(); ++i) {
        delete _blocks[i];_blocks[i] = NULL;
    } // for
} // close


// ------------------------------------------------------------------------------------------------
// Load metadata.
void
geomodelgrids::serial::Model::loadMetadata(void) {
    if (!_h5) {
        throw std::logic_error("Model not open. Call open() before loading metadata.");
    } // if

    std::ostringstream msg;
    const char* indent = "            ";
    msg << indent << "Missing attributes:\n";
    bool missingAttributes = false;

    delete _info;_info = new geomodelgrids::serial::ModelInfo;assert(_info);
    try {
        _info->load(_h5);
    } catch (const std::runtime_error& err) {
        msg << err.what();
        missingAttributes = true;
    } // try/catch

    if (_h5->hasAttribute("/", "data_values")) {
        _h5->readAttribute("/", "data_values", &_valueNames);
    } else {
        msg << indent << "    /data_values\n";
        missingAttributes = true;
    } // if/else

    if (_h5->hasAttribute("/", "data_units")) {
        _h5->readAttribute("/", "data_units", &_valueUnits);
    } else {
        msg << indent << "    /data_units\n";
        missingAttributes = true;
    } // if/else

    if (_h5->hasAttribute("/", "data_layout")) {
        const std::string& layoutString = _h5->readAttribute("/", "data_layout");
        if (0 == strcasecmp(layoutString.c_str(), "vertex")) {
            _layout = VERTEX;
        } else {
            msg << indent << "    Only vertex-based data layout is supported\n";
            missingAttributes = true;
        } // if/else
    } else {
        msg << indent << "    /data_layout\n";
        missingAttributes = true;
    } // if/else

    if (_h5->hasAttribute("/", "dim_x")) {
        _h5->readAttribute("/", "dim_x", H5T_NATIVE_DOUBLE, (void*)&_dims[0]);
    } else {
        msg << indent << "    /dim_x\n";
        missingAttributes = true;
    } // if/else
    if (_h5->hasAttribute("/", "dim_y")) {
        _h5->readAttribute("/", "dim_y", H5T_NATIVE_DOUBLE, (void*)&_dims[1]);
    } else {
        msg << indent << "    /dim_y\n";
        missingAttributes = true;
    } // if/else
    if (_h5->hasAttribute("/", "dim_z")) {
        _h5->readAttribute("/", "dim_z", H5T_NATIVE_DOUBLE, (void*)&_dims[2]);
    } else {
        msg << indent << "    /dim_z\n";
        missingAttributes = true;
    } // if/else

    if (_h5->hasAttribute("/", "crs")) {
        _modelCRSString = _h5->readAttribute("/", "crs");
    } else {
        msg << indent << "    /crs\n";
        missingAttributes = true;
    } // if/else
    if (_h5->hasAttribute("/", "origin_x")) {
        _h5->readAttribute("/", "origin_x", H5T_NATIVE_DOUBLE, (void*)&_origin[0]);
    } else {
        msg << indent << "    /origin_x\n";
        missingAttributes = true;
    } // if/else
    if (_h5->hasAttribute("/", "origin_y")) {
        _h5->readAttribute("/", "origin_y", H5T_NATIVE_DOUBLE, (void*)&_origin[1]);
    } else {
        msg << indent << "    /origin_y\n";
        missingAttributes = true;
    } // if/else
    if (_h5->hasAttribute("/", "y_azimuth")) {
        _h5->readAttribute("/", "y_azimuth", H5T_NATIVE_DOUBLE, (void*)&_yazimuth);
    } else {
        msg << indent << "    /y_azimuth\n";
        missingAttributes = true;
    } // if/else

    delete _surfaceTop;_surfaceTop = NULL;
    delete _surfaceTopoBathy;_surfaceTopoBathy = NULL;
    if (_h5->hasGroup("surfaces")) {
        if (_h5->hasDataset("surfaces/top_surface")) {
            _surfaceTop = new geomodelgrids::serial::Surface("top_surface");assert(_surfaceTop);
            try {
                _surfaceTop->loadMetadata(_h5);
            } catch (const std::runtime_error& err) {
                msg << err.what();
                missingAttributes = true;
            } // try/catch
        } // if

        if (_h5->hasDataset("surfaces/topography_bathymetry")) {
            _surfaceTopoBathy = new geomodelgrids::serial::Surface("topography_bathymetry");assert(_surfaceTopoBathy);
            try {
                _surfaceTopoBathy->loadMetadata(_h5);
            } catch (const std::runtime_error& err) {
                msg << err.what();
                missingAttributes = true;
            } // try/catch
        } // if
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
        try {
            _blocks[i]->loadMetadata(_h5);
        } catch (std::runtime_error& err) {
            msg << err.what();
            missingAttributes = true;
        } // try/catch
    } // for
    std::sort(_blocks.begin(), _blocks.end(), Block::compare);

    if (missingAttributes) { throw std::runtime_error(msg.str().c_str()); }
} // loadMetadata


// ------------------------------------------------------------------------------------------------
// Initialize.
void
geomodelgrids::serial::Model::initialize(void) {
    // Initialize CRS transformation
    delete _crsTransformer;_crsTransformer = new geomodelgrids::utils::CRSTransformer();assert(_crsTransformer);
    _crsTransformer->setSrc(_inputCRSString.c_str());
    _crsTransformer->setDest(_modelCRSString.c_str());
    _crsTransformer->initialize();

    if (_surfaceTop) {
        _surfaceTop->openQuery(_h5);
    } // if
    if (_surfaceTopoBathy) {
        _surfaceTopoBathy->openQuery(_h5);
    } // if
    size_t numBlocks = _blocks.size();
    for (size_t i = 0; i < numBlocks; ++i) {
        _blocks[i]->openQuery(_h5);
    } // for
} // initialize


// ------------------------------------------------------------------------------------------------
// Get names of values in model.
const std::vector<std::string>&
geomodelgrids::serial::Model::getValueNames(void) const {
    return _valueNames;
} // getValueNames


// ------------------------------------------------------------------------------------------------
// Get names of units in model.
const std::vector<std::string>&
geomodelgrids::serial::Model::getValueUnits(void) const {
    return _valueUnits;
} // getValueUnits


// ------------------------------------------------------------------------------------------------
// Get data layout.
geomodelgrids::serial::Model::DataLayout
geomodelgrids::serial::Model::getDataLayout(void) const {
    return _layout;
} // getDataLayout


// ------------------------------------------------------------------------------------------------
// Get model dimensions.
const double*
geomodelgrids::serial::Model::getDims(void) const {
    return _dims;
} // getDims


// ------------------------------------------------------------------------------------------------
// Get coordinates of model origin.
const double*
geomodelgrids::serial::Model::getOrigin(void) const {
    return _origin;
} // getOrigin


// ------------------------------------------------------------------------------------------------
// Get azimuth of y coordinate axis.
const double
geomodelgrids::serial::Model::getYAzimuth(void) const {
    return _yazimuth;
} // getYAzimuth


// ------------------------------------------------------------------------------------------------
// Get CRS for model.
const std::string&
geomodelgrids::serial::Model::getCRSString(void) const {
    return _modelCRSString;
} // getCRSTransformer


// ------------------------------------------------------------------------------------------------
// Get model description information.
const geomodelgrids::serial::ModelInfo*
geomodelgrids::serial::Model::getInfo(void) const {
    return _info;
} // getInfo


// ------------------------------------------------------------------------------------------------
// Get model topography.
const geomodelgrids::serial::Surface*
geomodelgrids::serial::Model::getTopSurface(void) const {
    return _surfaceTop;
} // getTopSurface


// ------------------------------------------------------------------------------------------------
// Get model topography/bathymetry.
const geomodelgrids::serial::Surface*
geomodelgrids::serial::Model::getTopoBathy(void) const {
    return _surfaceTopoBathy;
} // getTopoBathy


// ------------------------------------------------------------------------------------------------
// Get model description.
const std::vector<geomodelgrids::serial::Block*>&
geomodelgrids::serial::Model::getBlocks(void) const {
    return _blocks;
} // getBlocks


// ------------------------------------------------------------------------------------------------
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


// ------------------------------------------------------------------------------------------------
// Query for elevation of top of model at point using bilinear interpolation.
double
geomodelgrids::serial::Model::queryTopElevation(const double x,
                                                const double y) {
    double elevation = 0.0;

    if (_surfaceTop) {
        double xModel = 0.0;
        double yModel = 0.0;
        _toModelXYZ(&xModel, &yModel, NULL, x, y, 0.0);
        const double zModelCRS = _surfaceTop->query(xModel, yModel);

        const double yazimuthRad = _yazimuth * M_PI / 180.0;
        const double cosAz = cos(yazimuthRad);
        const double sinAz = sin(yazimuthRad);
        const double xRel = +xModel*cosAz + yModel*sinAz;
        const double yRel = -xModel*sinAz + yModel*cosAz;
        const double xModelCRS = xRel + _origin[0];
        const double yModelCRS = yRel + _origin[1];

        double xIn = 0.0;
        double yIn = 0.0;
        _crsTransformer->inverse_transform(&xIn, &yIn, &elevation, xModelCRS, yModelCRS, zModelCRS);
    } // if

    return elevation;
} // queryTopElevation


// ------------------------------------------------------------------------------------------------
// Query for elevation of topography/bathymetry at point using bilinear interpolation.
double
geomodelgrids::serial::Model::queryTopoBathyElevation(const double x,
                                                      const double y) {
    double elevation = 0.0;

    if (_surfaceTopoBathy || _surfaceTop) {
        double xModel = 0.0;
        double yModel = 0.0;
        _toModelXYZ(&xModel, &yModel, NULL, x, y, 0.0);
        const double zModelCRS = (_surfaceTopoBathy) ? _surfaceTopoBathy->query(xModel, yModel) : _surfaceTop->query(xModel, yModel);

        const double yazimuthRad = _yazimuth * M_PI / 180.0;
        const double cosAz = cos(yazimuthRad);
        const double sinAz = sin(yazimuthRad);
        const double xRel = +xModel*cosAz + yModel*sinAz;
        const double yRel = -xModel*sinAz + yModel*cosAz;
        const double xModelCRS = xRel + _origin[0];
        const double yModelCRS = yRel + _origin[1];

        double xIn = 0.0;
        double yIn = 0.0;
        _crsTransformer->inverse_transform(&xIn, &yIn, &elevation, xModelCRS, yModelCRS, zModelCRS);
    } // if

    return elevation;
} // queryTopoBathyElevation


// ------------------------------------------------------------------------------------------------
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


// ------------------------------------------------------------------------------------------------
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
        if (_surfaceTop) {
            zGroundSurf = _surfaceTop->query(*xModel, *yModel);
        } // if
        const double zBottom = -_dims[2];
        *zModel = zBottom * (zGroundSurf - zModelCRS) / (zGroundSurf - zBottom);
        if ((*zModel > 0.0) && (*zModel < TOLERANCE)) {
            *zModel = 0.0;
        } // if
    } // if
} // _toModelXYZ


// ------------------------------------------------------------------------------------------------
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
