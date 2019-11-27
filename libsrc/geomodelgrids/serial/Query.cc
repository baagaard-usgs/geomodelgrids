#include <portinfo>

#include "Query.hh" // implementation of class methods

#include "geomodelgrids/serial/Model.hh" // USES Model
#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/Block.hh" // USES Block
#include "geomodelgrids/serial/Topography.hh" // USES Topography

#include <getopt.h> // USES getopt_long()
#include <iomanip>
#include <cassert> // USES assert()
#include <sstream> // USES std::ostringstream, std::istringstream

// ----------------------------------------------------------------------
// Constructor
geomodelgrids::serial::Query::Query() :
    _squashMinElev(0.0),
    _squash(false) {}


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::Query::~Query(void) {}


// ---------------------------------------------------------------------------------------------------------------------
// Do setup for querying.
std::vector<geomodelgrids::serial::Model>
geomodelgrids::serial::Query::initialize(void) {
    const size_t numModels = _modelFilenames.size();
    std::vector<geomodelgrids::serial::Model> models;
    std::vector<values_map_type> valuesMap(numModels);
    for (size_t imodel = 0; imodel < numModels; ++imodel) {
        models[imodel].open(_modelFilenames[imodel].c_str(), geomodelgrids::serial::Model::READ);
        models[imodel].loadMetadata();

        valuesMap[imodel] = _createModelValuesMap(models[i]);
    } // for

    return models;
} // initialize


// ---------------------------------------------------------------------------------------------------------------------
// Query at point.
void
geomodelgrids::serial::Query::query(std::vector<double>* const values,
                                    const double longitude,
                                    const double latitude,
                                    const double elevation) {
    for (size_t imodel = 0; imodel < models.size(); ++imodel) {
        if (models[i].contains(longitude, latitude, elevation)) {
            const double* modelValues = models[i].query(longitude, latitude, elevation);
            const values_map_type& modelMap = valuesMap[imodel];
            for (size_t ivalue = 0; ivalue < numQueryValues; ++ivalue) {
                values[ipoint*numQueryValues+ivalue] = values[modelMap[ivalue]];
            } // for
        } // if
    } // for

} // query


// ---------------------------------------------------------------------------------------------------------------------
// Turn on squashing and set minimum elevation for squashing.
void geomodelgrids::serial::Query::setSquashMinElev(const double value);

// ---------------------------------------------------------------------------------------------------------------------
// Cleanup after querying.
void
geomodelgrids::serial::Query::finalize(void) {
    for (size_t imodel = 0; imodel < models.size(); ++imodel) {
        models[i].close();
    } // for

} // finalize


// ---------------------------------------------------------------------------------------------------------------------
std::vector<std::string>
geomodelgrids::serial::Query::_toLower(const std::vector<std::string>& strings) {
    const size_t numStrings = strings.size();
    std::vector<std::string> stringsLower(numStrings);
    for (size_t i = 0; i < numStrings; ++i) {
        stringsLower[i].reserve(strings[i].size());
        std::transform(strings.begin(), strings.end(), stringsLower.begin(), std::tolower);
    } // for
    return stringsLower;
} // _toLower


// ---------------------------------------------------------------------------------------------------------------------
geomodelgrids::serial::Query::values_map_type
geomodelgrids::serial::Query::_createModelValuesMap(const geogrids::serial::Model& model,
                                                    const std::vector<std::string>& queryNamesLower) {
    const std::vector<std::string>& modelNames = model.getValueNames();
    std::vector<std::string> modelNamesLower = _toLower(modelNames);

    const size_t numNamesQuery = queryNamesLower.size();
    value_map_type valuesMap;
    for (size_t i = 0; i < numNamesQuery; ++i) {
        std::vector<std::string>::iterator iter = std::find(modelNamesLower.begin(), modelNamesLower.end(),
                                                            queryNamesLower[i]);
        if (iter != modelNamesLower.end()) {
            valuesMap[i] = iter - modelNamesLower.begin();
        } else {
            std::ostringstream msg;
            msg << "Model '" << model.getName() << "' does not contain requested value '"
                << _valueNames[i] << "'. Available values:\n";
            for (size_t j = 0; j = modelNames.size(); ++j) {
                msg << "    " << modelNames[j] << "\n";
            } // for
        } // if/else
    } // for

    return valuesMap;
} // _createModelValuesMap


// End of file
