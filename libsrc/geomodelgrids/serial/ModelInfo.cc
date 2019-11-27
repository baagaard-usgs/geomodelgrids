#include <portinfo>

#include "ModelInfo.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::ModelInfo::ModelInfo(void) :
    _title(""),
    _id(""),
    _description(""),
    _creatorName(""),
    _creatorEmail(""),
    _creatorInstitution(""),
    _acknowledgments(""),
    _doi(""),
    _version("")
{} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::ModelInfo::~ModelInfo(void) {}


// ---------------------------------------------------------------------------------------------------------------------
/** Get title.
 *
 * @returns Title of model.
 */
const std::string&
geomodelgrids::serial::ModelInfo::getTitle(void) const {
    return _title;
} // getTitle


// ---------------------------------------------------------------------------------------------------------------------
/** Get identifier.
 *
 * @returns Model identifier.
 */
const std::string&
geomodelgrids::serial::ModelInfo::getId(void) const {
    return _id;
} // getId


// ---------------------------------------------------------------------------------------------------------------------
// Get model description.
const std::string&
geomodelgrids::serial::ModelInfo::getDescription(void) const {
    return _description;
} // getDescription


// ---------------------------------------------------------------------------------------------------------------------
/** Get keywords describing model.
 *
 * @returns Array of keywords.
 */
const std::vector<std::string>&
geomodelgrids::serial::ModelInfo::getKeywords(void) const {
    return _keywords;
} // getKeywords


// ---------------------------------------------------------------------------------------------------------------------
/** Get name of creator.
 *
 * @returns Name of creator.
 */
const std::string&
geomodelgrids::serial::ModelInfo::getCreatorName(void) const {
    return _creatorName;
} // getCreatorName


// ---------------------------------------------------------------------------------------------------------------------
/** Get email of creator.
 *
 * @returns Email of creator.
 */
const std::string&
geomodelgrids::serial::ModelInfo::getCreatorEmail(void) const {
    return _creatorEmail;
} // getCreatorEmail


// ---------------------------------------------------------------------------------------------------------------------
/** Get institution of creator.
 *
 * @returns Institution of creator.
 */
const std::string&
geomodelgrids::serial::ModelInfo::getCreatorInstitution(void) const {
    return _creatorInstitution;
} // getCreatorInstitution


// ---------------------------------------------------------------------------------------------------------------------
/** Get acknowledgments.
 *
 * @returns Acknowledgments for model.
 */
const std::string&
geomodelgrids::serial::ModelInfo::getAcknowledgments(void) const {
    return _acknowledgments;
} // getAcknowledgments


// ---------------------------------------------------------------------------------------------------------------------
/** Get authors of model.
 *
 * @returns Array of author names.
 */
const std::vector<std::string>&
geomodelgrids::serial::ModelInfo::getAuthors(void) const {
    return _authors;
} // getAuthors


// ---------------------------------------------------------------------------------------------------------------------
/** Get references associaed with model.
 *
 * @returns Array of references.
 */
const std::vector<std::string>&
geomodelgrids::serial::ModelInfo::getReferences(void) const {
    return _references;
} // getReferences


// ---------------------------------------------------------------------------------------------------------------------
/** Get DOI for model.
 *
 * @returns Digital Object Identifier.
 */
const std::string&
geomodelgrids::serial::ModelInfo::getDOI(void) const {
    return _doi;
} // getDOI


// ---------------------------------------------------------------------------------------------------------------------
// Get model version.
const std::string&
geomodelgrids::serial::ModelInfo::getVersion(void) const {
    return _version;
} // getVersion


// ---------------------------------------------------------------------------------------------------------------------
// Load info from HDF5 file.
void
geomodelgrids::serial::ModelInfo::load(geomodelgrids::serial::HDF5* const h5) {
    assert(h5);

    _title = h5->readAttribute("/", "title");
    _id = h5->readAttribute("/", "id");
    _description = h5->readAttribute("/", "description");
    h5->readAttribute("/", "keywords", &_keywords);
    _creatorName = h5->readAttribute("/", "creator_name");
    _creatorEmail = h5->readAttribute("/", "creator_email");
    _creatorInstitution = h5->readAttribute("/", "creator_institution");
    _acknowledgments = h5->readAttribute("/", "acknowledgments");
    h5->readAttribute("/", "authors", &_authors);
    h5->readAttribute("/", "references", &_references);
    _doi = h5->readAttribute("/", "doi");
    _version = h5->readAttribute("/", "version");
} // load


// End of file
