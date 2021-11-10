#include <portinfo>

#include "Query.hh" // implementation of class methods

#include "geomodelgrids/serial/Model.hh" // USES Model
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/serial/Surface.hh" // USES Surface
#include "geomodelgrids/utils/ErrorHandler.hh" // USES ErrorHandler
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include <getopt.h> // USES getopt_long()
#include <algorithm> // USES std::transform
#include <cctype> // USES std::lower
#include <cassert> // USES assert()
#include <sstream> // USES std::ostringstream, std::istringstream

// ------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace serial {
        class _Query;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::_Query {
public:

    /** Transform array of strings to lowercase.
     *
     * @param[in] strings Array of strings.
     * @returns Array of lowercase strings.
     */
    static
    std::vector<std::string> toLower(const std::vector<std::string>& strings);

    /** Create map from index of query values to index of values in model.
     * @param[in] model Model to query.
     * @param[in] queryNamesLower Array of lowercase names of values to query.
     * @returns Map of value indices for query values.
     */
    static
    geomodelgrids::serial::Query::values_map_type createModelValuesIndex(const geomodelgrids::serial::Model& model,
                                                                         const std::vector<std::string>& queryNamesLower);

    /** Check consistency of units in model.
     *
     * @param[in] valueUnits Map from index of query value to units.
     * @param[in] modelValueIndex Map from index of query value to index of model value.
     * @param[in] modelValues Names of model values.
     * @param[in] modelUnitsLower Units of model values (lowercase).
     */
    static
    void checkUnits(std::map<size_t, std::string>* valueUnits,
                    std::map<size_t, size_t>& modelValueIndex,
                    const std::vector<std::string>& modelValues,
                    const std::vector<std::string>& modelUnitsLower);

    static
    unsigned char tolower(unsigned char c);

}; // _Query

// ------------------------------------------------------------------------------------------------
// Constructor
geomodelgrids::serial::Query::Query() :
    _squashMinElev(0.0),
    _errorHandler(new geomodelgrids::utils::ErrorHandler),
    _squash(SQUASH_NONE) {}


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Query::~Query(void) {
    for (size_t i = 0; i < _models.size(); ++i) {
        delete _models[i];_models[i] = NULL;
    } // for
    delete _errorHandler;_errorHandler = NULL;
} // destructor


// ------------------------------------------------------------------------------------------------
// Get error handler.
geomodelgrids::utils::ErrorHandler&
geomodelgrids::serial::Query::getErrorHandler(void) {
    assert(_errorHandler);
    return *_errorHandler;
} // getErrorHandler


// ------------------------------------------------------------------------------------------------
// Do setup for querying.
void
geomodelgrids::serial::Query::initialize(const std::vector<std::string>& modelFilenames,
                                         const std::vector<std::string>& valueNames,
                                         const std::string& inputCRSString) {
    _valuesLowercase = _Query::toLower(valueNames);

    for (size_t i = 0; i < _models.size(); ++i) {
        delete _models[i];_models[i] = NULL;
    } // for

    const size_t numModels = modelFilenames.size();
    _models.resize(numModels);
    _valuesIndex.resize(numModels);
    std::map<size_t, std::string> valueUnits;
    for (size_t iModel = 0; iModel < numModels; ++iModel) {
        _models[iModel] = new geomodelgrids::serial::Model();assert(_models[iModel]);
        _models[iModel]->setInputCRS(inputCRSString);
        _models[iModel]->open(modelFilenames[iModel].c_str(), geomodelgrids::serial::Model::READ);
        _models[iModel]->loadMetadata();
        _models[iModel]->initialize();

        _valuesIndex[iModel] = _Query::createModelValuesIndex(*_models[iModel], _valuesLowercase);

        const std::vector<std::string>& modelValues = _models[iModel]->getValueNames();
        const std::vector<std::string>& modelUnitsLower = _Query::toLower(_models[iModel]->getValueUnits());
        _Query::checkUnits(&valueUnits, _valuesIndex[iModel], modelValues, modelUnitsLower);
    } // for
} // initialize


// ------------------------------------------------------------------------------------------------
// Turn on squashing and set minimum z for squashing.
void
geomodelgrids::serial::Query::setSquashMinElev(const double value) {
    _squashMinElev = value;
    if (_squash == SQUASH_NONE) {
        _squash = SQUASH_TOP_SURFACE;
    } // if
} // setSquashMinElev


// ------------------------------------------------------------------------------------------------
// Turn squashing on/off.
void
geomodelgrids::serial::Query::setSquashing(const SquashingEnum value) {
    _squash = value;
} // setSquashing


// ------------------------------------------------------------------------------------------------
// Query for elevation of top of model at point.
double
geomodelgrids::serial::Query::queryTopElevation(const double x,
                                                const double y) {
    double elevation = NODATA_VALUE;
    const double zOffset = -1.0e-3;
    for (size_t i = 0; i < _models.size(); ++i) {
        assert(_models[i]);
        const double elevationTmp = _models[i]->queryTopElevation(x, y);
        if (_models[i]->contains(x, y, elevationTmp+zOffset)) {
            elevation = elevationTmp;
            break;
        } // if
    } // for

    return elevation;
} // queryTopElevation


// ------------------------------------------------------------------------------------------------
// Query for elevation of topography/bathymetry at point.
double
geomodelgrids::serial::Query::queryTopoBathyElevation(const double x,
                                                      const double y) {
    double elevation = NODATA_VALUE;
    const double zOffset = -1.0e-3;
    for (size_t i = 0; i < _models.size(); ++i) {
        assert(_models[i]);
        const double elevationTmp = _models[i]->queryTopoBathyElevation(x, y);
        if (_models[i]->contains(x, y, elevationTmp+zOffset)) {
            elevation = elevationTmp;
            break;
        } // if
    } // for

    return elevation;
} // queryTopoBathyElevation


// ------------------------------------------------------------------------------------------------
// Query at point.
int
geomodelgrids::serial::Query::query(double* const values,
                                    const double x,
                                    const double y,
                                    const double z) {
    if (!values) {
        assert(_errorHandler);
        _errorHandler->setError("geomodelgrids::serial::Query::query() passed NULL for values argument.");
        return geomodelgrids::utils::ErrorHandler::ERROR;
    } // if

    const size_t numQueryValues = _valuesLowercase.size();
    std::fill(values, values+numQueryValues, NODATA_VALUE);
    bool found = false;
    for (size_t i = 0; i < _models.size(); ++i) {
        assert(_models[i]);
        double zSquash = z;
        switch (_squash) {
        case SQUASH_NONE:
            break;
        case SQUASH_TOP_SURFACE:
            if (z > _squashMinElev) {
                const double topElev = _models[i]->queryTopElevation(x, y);
                zSquash = z + topElev;
            } // if
            break;
        case SQUASH_TOPOGRAPHY_BATHYMETRY:
            if (z > _squashMinElev) {
                const double groundElev = _models[i]->queryTopoBathyElevation(x, y);
                zSquash = z + groundElev;
            } // if
            break;
        default:
            throw std::logic_error("Unknown squashing type.");
        } // switch
	if (_models[i]->contains(x, y, zSquash)) {
            const double* modelValues = _models[i]->query(x, y, zSquash);
            values_map_type& modelMap = _valuesIndex[i];
            for (size_t iValue = 0; iValue < numQueryValues; ++iValue) {
                values[iValue] = modelValues[modelMap[iValue]];
            } // for

            found = true;
            break;
        } // if
    } // for

    return found ? geomodelgrids::utils::ErrorHandler::OK : geomodelgrids::utils::ErrorHandler::WARNING;
} // query


// ------------------------------------------------------------------------------------------------
// Cleanup after querying.
void
geomodelgrids::serial::Query::finalize(void) {
    for (size_t i = 0; i < _models.size(); ++i) {
        assert(_models[i]);
        _models[i]->close();
    } // for
} // finalize


// ------------------------------------------------------------------------------------------------
std::vector<std::string>
geomodelgrids::serial::_Query::toLower(const std::vector<std::string>& strings) {
    const size_t numStrings = strings.size();
    std::vector<std::string> stringsLower(numStrings);
    for (size_t i = 0; i < numStrings; ++i) {
        stringsLower[i].resize(strings[i].size());
        std::transform(strings[i].begin(), strings[i].end(), stringsLower[i].begin(), _Query::tolower);
        assert(strings[i].size() == stringsLower[i].size());
    } // for
    return stringsLower;
} // toLower


// ------------------------------------------------------------------------------------------------
unsigned char
geomodelgrids::serial::_Query::tolower(unsigned char c) {
    return std::tolower(c);
} // tolower


// ------------------------------------------------------------------------------------------------
geomodelgrids::serial::Query::values_map_type
geomodelgrids::serial::_Query::createModelValuesIndex(const geomodelgrids::serial::Model& model,
                                                      const std::vector<std::string>& queryNamesLower) {
    const std::vector<std::string>& modelNames = model.getValueNames();
    std::vector<std::string> modelNamesLower = _Query::toLower(modelNames);

    const size_t numNamesQuery = queryNamesLower.size();
    geomodelgrids::serial::Query::values_map_type valuesIndex;
    for (size_t i = 0; i < numNamesQuery; ++i) {
        std::vector<std::string>::iterator iter = std::find(modelNamesLower.begin(), modelNamesLower.end(),
                                                            queryNamesLower[i]);
        if (iter != modelNamesLower.end()) {
            valuesIndex[i] = iter - modelNamesLower.begin();
        } else {
            std::ostringstream msg;
            const geomodelgrids::serial::ModelInfo* info = model.getInfo();assert(info);
            msg << "Model '" << info->getTitle() << "' does not contain requested value '"
                << queryNamesLower[i] << "'. Available values:\n";
            for (size_t j = 0; j < modelNames.size(); ++j) {
                msg << "    " << modelNames[j] << "\n";
            } // for
            throw std::invalid_argument(msg.str());
        } // if/else
    } // for

    return valuesIndex;
} // createModelValuesIndex


// ------------------------------------------------------------------------------------------------
void
geomodelgrids::serial::_Query::checkUnits(std::map<size_t,std::string>* valueUnits,
                                          std::map<size_t, size_t>& valuesIndex,
                                          const std::vector<std::string>& modelValues,
                                          const std::vector<std::string>& modelUnits) {
    assert(valueUnits);

    for (size_t iValue = 0; iValue < valuesIndex.size(); ++iValue) {
        const size_t indexModel = valuesIndex[iValue];
        if (valueUnits->count(iValue) == 0) {
            (*valueUnits)[iValue] = modelUnits[indexModel];
        } else if ((*valueUnits)[iValue] != modelUnits[indexModel]) {
            std::ostringstream msg;
            msg << "Inconsistent units among models for value '" << modelValues[indexModel] << "'. "
                << "Inconsistent units are '" << (*valueUnits)[iValue] << "' and '"
                << modelUnits[indexModel] << "'.";
            throw std::runtime_error(msg.str());
        } // if/else
    } // for
}


// End of file
