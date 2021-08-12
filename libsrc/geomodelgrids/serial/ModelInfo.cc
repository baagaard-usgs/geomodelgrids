#include <portinfo>

#include "ModelInfo.hh" // implementation of class methods

#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cstring> // USES strlen()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::serial::ModelInfo::ModelInfo(void) :
    _title(""),
    _id(""),
    _description(""),
    _history(""),
    _comment(""),
    _creatorName(""),
    _creatorInstitution(""),
    _creatorEmail(""),
    _acknowledgement(""),
    _repositoryName(""),
    _repositoryURL(""),
    _repositoryDOI(""),
    _version(""),
    _license(""),
    _auxiliary("")
{} // constructor


// ------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::serial::ModelInfo::~ModelInfo(void) {}


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getTitle(void) const {
    return _title;
} // getTitle


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getId(void) const {
    return _id;
} // getId


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getDescription(void) const {
    return _description;
} // getDescription


// ------------------------------------------------------------------------------------------------
const std::vector<std::string>&
geomodelgrids::serial::ModelInfo::getKeywords(void) const {
    return _keywords;
} // getKeywords


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getHistory(void) const {
    return _history;
} // getHistory


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getComment(void) const {
    return _comment;
} // getComment


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getCreatorName(void) const {
    return _creatorName;
} // getCreatorName


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getCreatorInstitution(void) const {
    return _creatorInstitution;
} // getCreatorInstitution


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getCreatorEmail(void) const {
    return _creatorEmail;
} // getCreatorEmail


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getAcknowledgement(void) const {
    return _acknowledgement;
} // getAcknowledgment


// ------------------------------------------------------------------------------------------------
const std::vector<std::string>&
geomodelgrids::serial::ModelInfo::getAuthors(void) const {
    return _authors;
} // getAuthors


// ------------------------------------------------------------------------------------------------
const std::vector<std::string>&
geomodelgrids::serial::ModelInfo::getReferences(void) const {
    return _references;
} // getReferences


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getRepositoryName(void) const {
    return _repositoryName;
} // getRepositoryName


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getRepositoryURL(void) const {
    return _repositoryURL;
} // getRepositoryURL


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getRepositoryDOI(void) const {
    return _repositoryDOI;
} // getRepositoryDOI


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getVersion(void) const {
    return _version;
} // getVersion


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getLicense(void) const {
    return _license;
} // getLicense


// ------------------------------------------------------------------------------------------------
const std::string&
geomodelgrids::serial::ModelInfo::getAuxiliary(void) const {
    return _auxiliary;
}


// ------------------------------------------------------------------------------------------------
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

    if (h5->hasAttribute("/", "history")) {
        _history = h5->readAttribute("/", "history");
    } else {
        msg << indent << "    /history\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "comment")) {
        _comment = h5->readAttribute("/", "comment");
    } else {
        msg << indent << "    /comment\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "creator_name")) {
        _creatorName = h5->readAttribute("/", "creator_name");
    } else {
        msg << indent << "    /creator_name\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "creator_institution")) {
        _creatorInstitution = h5->readAttribute("/", "creator_institution");
    } else {
        msg << indent << "    /creator_institution\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "creator_email")) {
        _creatorEmail = h5->readAttribute("/", "creator_email");
    } else {
        msg << indent << "    /creator_email\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "acknowledgement")) {
        _acknowledgement = h5->readAttribute("/", "acknowledgement");
    } else {
        msg << indent << "    /acknowledgement\n";
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

    if (h5->hasAttribute("/", "repository_name")) {
        _repositoryName = h5->readAttribute("/", "repository_name");
    } else {
        msg << indent << "    /repository_name\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "repository_url")) {
        _repositoryURL = h5->readAttribute("/", "repository_url");
    } else {
        msg << indent << "    /repository_url\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "repository_doi")) {
        _repositoryDOI = h5->readAttribute("/", "repository_doi");
    } else {
        msg << indent << "    /repository_doi\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "version")) {
        _version = h5->readAttribute("/", "version");
    } else {
        msg << indent << "    /version\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "license")) {
        _license = h5->readAttribute("/", "license");
    } else {
        msg << indent << "    /license\n";
        missingAttributes = true;
    } // if/else

    if (h5->hasAttribute("/", "auxiliary")) {
        _auxiliary = h5->readAttribute("/", "auxiliary");
    } // if

    if (missingAttributes) { throw std::runtime_error(msg.str().c_str()); }
} // load


// End of file
