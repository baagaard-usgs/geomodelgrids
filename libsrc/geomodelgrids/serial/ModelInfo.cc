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
    _acknowledgements(""),
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
geomodelgrids::serial::ModelInfo::getAcknowledgements(void) const {
    return _acknowledgements;
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
    const char* indent = "            ";
    bool missingAttributes = false;

    if (h5->hasAttribute("/", "title")) {
        _title = h5->readAttribute("/", "title");
    } else {
        msg << indent << "    /title\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "id")) {
        _id = h5->readAttribute("/", "id");
    } else {
        msg << indent << "    /id\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "description")) {
        _description = h5->readAttribute("/", "description");
    } else {
        msg << indent << "    /description\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "keywords")) {
        h5->readAttribute("/", "keywords", &_keywords);
    } else {
        msg << indent << "    /keywords\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "creator_name")) {
        _creatorName = h5->readAttribute("/", "creator_name");
    } else {
        msg << indent << "    /creator_name\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "creator_email")) {
        _creatorEmail = h5->readAttribute("/", "creator_email");
    } else {
        msg << indent << "    /creator_email\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "creator_institution")) {
        _creatorInstitution = h5->readAttribute("/", "creator_institution");
    } else {
        msg << indent << "    /creator_institution\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "acknowledgements")) {
        _acknowledgements = h5->readAttribute("/", "acknowledgements");
    } else {
        msg << indent << "    /acknowledgements\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "authors")) {
        h5->readAttribute("/", "authors", &_authors);
    } else {
        msg << indent << "    /authors\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "references")) {
        h5->readAttribute("/", "references", &_references);
    } else {
        msg << indent << "    /references\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "doi")) {
        _doi = h5->readAttribute("/", "doi");
    } else {
        msg << indent << "    /doi\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "version")) {
        _version = h5->readAttribute("/", "version");
    } else {
        msg << indent << "    /version\n";
        missingAttributes = true;
    } // if/else

    if (missingAttributes) { throw std::runtime_error(msg.str().c_str()); }
} // load


// End of file
