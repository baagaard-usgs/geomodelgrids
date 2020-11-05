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

    // Verify attributes exist
    std::ostringstream msg;
    bool missingAttributes = false;

    if (h5->hasAttribute("/", "title")) {
        _title = h5->readAttribute("/", "title");
    } else {
        msg << "    /title";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "id")) {
        _id = h5->readAttribute("/", "id");
    } else {
        msg << "    /id";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "description")) {
        _description = h5->readAttribute("/", "description");
    } else {
        msg << "    /description";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "keywords")) {
        h5->readAttribute("/", "keywords", &_keywords);
    } else {
        msg << "    /keywords";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "creator_name")) {
        _creatorName = h5->readAttribute("/", "creator_name");
    } else {
        msg << "    /creator_name";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "creator_email")) {
        _creatorEmail = h5->readAttribute("/", "creator_email");
    } else {
        msg << "    /creator_email";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "creator_institution")) {
        _creatorInstitution = h5->readAttribute("/", "creator_institution");
    } else {
        msg << "    /creator_institution";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "acknowledgments")) {
        _acknowledgments = h5->readAttribute("/", "acknowledgments");
    } else {
        msg << "    /acknowledgments";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "authors")) {
        h5->readAttribute("/", "authors", &_authors);
    } else {
        msg << "    /authors";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "references")) {
        h5->readAttribute("/", "references", &_references);
    } else {
        msg << "    /references";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "doi")) {
        _doi = h5->readAttribute("/", "doi");
    } else {
        msg << "    /doi";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "version")) {
        _version = h5->readAttribute("/", "version");
    } else {
        msg << "    /version";
        missingAttributes = true;
    } // if/else

    if (missingAttributes) { throw std::runtime_error(msg.str().c_str()); }
} // load


// End of file
