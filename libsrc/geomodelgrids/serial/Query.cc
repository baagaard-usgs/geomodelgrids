#include <portinfo>

#include "Query.hh" // implementation of class methods

#include "geomodelgrids/serial/Model.hh" // USES Model
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/serial/Topography.hh" // USES Topography

#include <getopt.h> // USES getopt_long()
#include <algorithm> // USES std::transform
#include <cctype> // USES std::lower
#include <cassert> // USES assert()
#include <sstream> // USES std::ostringstream, std::istringstream

// ---------------------------------------------------------------------------------------------------------------------
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

    static
    unsigned char tolower(unsigned char c);

}; // _Query

// ---------------------------------------------------------------------------------------------------------------------
// Constructor
geomodelgrids::serial::Query::Query() :
    _squashMinElev(0.0),
    _squash(false) {}


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Query::~Query(void) {
    for (size_t i = 0; i < _models.size(); ++i) {
        delete _models[i];_models[i] = NULL;
    } // for
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Do setup for querying.
void
geomodelgrids::serial::Query::initialize(const std::vector<std::string>& modelFilenames,
                                         const std::vector<std::string>& valueNames,
                                         const std::string& inputCRSString) {
    const std::vector<std::string>& _valuesLowercase = _Query::toLower(valueNames);

    for (size_t i = 0; i < _models.size(); ++i) {
        delete _models[i];_models[i] = NULL;
    } // for

    const size_t numModels = modelFilenames.size();
    _models.resize(numModels);
    _valuesIndex.resize(numModels);
    for (size_t i = 0; i < numModels; ++i) {
        _models[i] = new geomodelgrids::serial::Model();assert(_models[i]);
        _models[i]->setInputCRS(inputCRSString);
        _models[i]->open(modelFilenames[i].c_str(), geomodelgrids::serial::Model::READ);
        _models[i]->loadMetadata();
        _models[i]->initialize();

        _valuesIndex[i] = _Query::createModelValuesIndex(*_models[i], _valuesLowercase);
    } // for
} // initialize


// ---------------------------------------------------------------------------------------------------------------------
// Turn on squashing and set minimum z for squashing.
void
geomodelgrids::serial::Query::setSquashMinElev(const double value) {
    _squashMinElev = value;
    _squash = true;
} // setSquashMinElev


// ---------------------------------------------------------------------------------------------------------------------
// Turn squashing on/off.
void
geomodelgrids::serial::Query::setSquashing(const bool value) {
    _squash = value;
} // setSquashing


// ---------------------------------------------------------------------------------------------------------------------
// Query for elevation of ground surface at point.
double
geomodelgrids::serial::Query::queryElevation(const double x,
                                             const double y) {
    double elevation = 0.0;

    for (size_t i = 0; i < _models.size(); ++i) {
        assert(_models[i]);
        elevation = _models[i]->queryElevation(x, y);
        if (_models[i]->contains(x, y, elevation)) {
            break;
        } // if
    } // for

    return elevation;
} // query


// ---------------------------------------------------------------------------------------------------------------------
// Query at point.
int
geomodelgrids::serial::Query::query(double* const values,
                                    const double x,
                                    const double y,
                                    const double z) {
    assert(values);

    const size_t numQueryValues = _valuesLowercase.size();
    std::fill(values, values+numQueryValues, -9999);
    int err = 1;
    for (size_t i = 0; i < _models.size(); ++i) {
        assert(_models[i]);
        if (_models[i]->contains(x, y, z)) {
            double elevationSquash = z;
            if (_squash && (z > _squashMinElev)) {
                const double groundElev = _models[i]->queryElevation(x, y);
                elevationSquash = z - groundElev;
            } // if
            err = 0;

            const double* modelValues = _models[i]->query(x, y, elevationSquash);
            values_map_type& modelMap = _valuesIndex[i];
            for (size_t ivalue = 0; ivalue < numQueryValues; ++ivalue) {
                values[ivalue] = modelValues[modelMap[ivalue]];
            } // for
        } // if
    } // for

    return err;
} // query


// ---------------------------------------------------------------------------------------------------------------------
// Cleanup after querying.
void
geomodelgrids::serial::Query::finalize(void) {
    for (size_t i = 0; i < _models.size(); ++i) {
        assert(_models[i]);
        _models[i]->close();
    } // for
} // finalize


// ---------------------------------------------------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------------------------------------------------
unsigned char
geomodelgrids::serial::_Query::tolower(unsigned char c) {
    return std::tolower(c);
} // tolower


// ---------------------------------------------------------------------------------------------------------------------
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
                << modelNames[i] << "'. Available values:\n";
            for (size_t j = 0; j < modelNames.size(); ++j) {
                msg << "    " << modelNames[j] << "\n";
            } // for
        } // if/else
    } // for

    return valuesIndex;
} // createModelValuesIndex


// End of file
