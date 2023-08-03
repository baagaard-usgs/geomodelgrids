/**
 * C++ unit testing of geomodelgrids::serial::ModelInfo.
 */

#include <portinfo>

#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include "catch2/catch_test_macros.hpp"

namespace geomodelgrids {
    namespace serial {
        class TestModelInfo;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestModelInfo {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Test constructor.
    static
    void testConstructor(void);

    /// Test getters.
    static
    void testAccessors(void);

    /// Test load().
    static
    void testLoad(void);

}; // class TestModelInfo

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestModelInfo::testConstructor", "[TestModelInfo]") {
    geomodelgrids::serial::TestModelInfo::testConstructor();
}
TEST_CASE("TestModelInfo::testAccessors", "[TestModelInfo]") {
    geomodelgrids::serial::TestModelInfo::testAccessors();
}
TEST_CASE("TestModelInfo::testLoad", "[TestModelInfo]") {
    geomodelgrids::serial::TestModelInfo::testLoad();
}

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestModelInfo::testConstructor(void) {
    ModelInfo info;

    CHECK(std::string("") == info._title);
    CHECK(std::string("") == info._id);
    CHECK(std::string("") == info._description);
    CHECK(info._keywords.empty());
    CHECK(std::string("") == info._history);
    CHECK(std::string("") == info._comment);

    CHECK(std::string("") == info._creatorName);
    CHECK(std::string("") == info._creatorInstitution);
    CHECK(std::string("") == info._creatorEmail);
    CHECK(std::string("") == info._acknowledgement);
    CHECK(info._authors.empty());
    CHECK(info._references.empty());

    CHECK(std::string("") == info._repositoryName);
    CHECK(std::string("") == info._repositoryURL);
    CHECK(std::string("") == info._repositoryDOI);
    CHECK(std::string("") == info._version);
    CHECK(std::string("") == info._license);
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test getters.
void
geomodelgrids::serial::TestModelInfo::testAccessors(void) {
    ModelInfo info;

    const std::string title("Title");info._title = title;
    const std::string id("Id");info._id = id;
    const std::string description("Description");info._description = description;

    const size_t numKeywords = 3;
    const char* keywordsPtr[numKeywords] = {"key one", "key two", "key three"};
    const std::vector<std::string> keywords(keywordsPtr, keywordsPtr+numKeywords);info._keywords = keywords;

    const std::string history("History");info._history = history;
    const std::string comment("Comment");info._comment = comment;

    const std::string creatorName("Name");info._creatorName = creatorName;
    const std::string creatorInstitution("Institution");info._creatorInstitution = creatorInstitution;
    const std::string creatorEmail("email@organization.org");info._creatorEmail = creatorEmail;
    const std::string acknowledgement("Acknowledgement");info._acknowledgement = acknowledgement;

    const size_t numAuthors = 4;
    const char* authorsPtr[numAuthors] = {"Name 0", "Name 1", "Name 2", "Name 3"};
    const std::vector<std::string> authors(authorsPtr, authorsPtr+numAuthors);info._authors = authors;

    const size_t numReferences = 2;
    const char* referencesPtr[numReferences] = {"Reference 0", "Reference 1"};
    const std::vector<std::string> references(referencesPtr, referencesPtr+numReferences);info._references = references;

    const std::string repositoryName("Repository");info._repositoryName = repositoryName;
    const std::string repositoryURL("URL");info._repositoryURL = repositoryURL;
    const std::string repositoryDOI("doi:1.2.3");info._repositoryDOI = repositoryDOI;
    const std::string version("1.2.3");info._version = version;
    const std::string license("1.2.3");info._license = license;
    const std::string auxiliary("{'a': 1}");info._auxiliary = auxiliary;

    CHECK(title == info.getTitle());
    CHECK(id == info.getId());
    CHECK(description == info.getDescription());

    const std::vector<std::string>& keywordsT = info.getKeywords();
    REQUIRE(keywords.size() == keywordsT.size());
    for (size_t i = 0; i < keywords.size(); ++i) {
        CHECK(keywords[i] == keywordsT[i]);
    } // for

    CHECK(history == info.getHistory());
    CHECK(comment == info.getComment());

    CHECK(creatorName == info.getCreatorName());
    CHECK(creatorInstitution == info.getCreatorInstitution());
    CHECK(creatorEmail == info.getCreatorEmail());
    CHECK(acknowledgement == info.getAcknowledgement());

    const std::vector<std::string>& authorsT = info.getAuthors();
    REQUIRE(authors.size() == authorsT.size());
    for (size_t i = 0; i < authors.size(); ++i) {
        CHECK(authors[i] == authorsT[i]);
    } // for

    const std::vector<std::string>& referencesT = info.getReferences();
    REQUIRE(references.size() == referencesT.size());
    for (size_t i = 0; i < references.size(); ++i) {
        CHECK(references[i] == referencesT[i]);
    } // for

    CHECK(repositoryName == info.getRepositoryName());
    CHECK(repositoryURL == info.getRepositoryURL());
    CHECK(repositoryDOI == info.getRepositoryDOI());
    CHECK(version == info.getVersion());
    CHECK(license == info.getLicense());

    CHECK(auxiliary == info.getAuxiliary());
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test load().
void
geomodelgrids::serial::TestModelInfo::testLoad(void) {
    geomodelgrids::serial::HDF5 h5;
    h5.open("../../data/one-block-flat.h5", H5F_ACC_RDONLY);

    ModelInfo info;
    info.load(&h5);

    const std::string title("One Block Flat");
    const std::string id("one-block-flat");
    const std::string description("Model with one block and no topography.");

    const size_t numKeywords = 3;
    const char* keywordsPtr[numKeywords] = {"key one", "key two", "key three"};
    const std::vector<std::string> keywords(keywordsPtr, keywordsPtr+numKeywords);info._keywords = keywords;

    const std::string history("First version");
    const std::string comment("One comment");

    const std::string creatorName("John Doe");
    const std::string creatorInstitution("Agency");
    const std::string creatorEmail("johndoe@agency.org");
    const std::string acknowledgement("Thank you!");

    const size_t numAuthors = 3;
    const char* authorsPtr[numAuthors] = {"Smith, Jim", "Doe, John", "Doyle, Sarah"};
    const std::vector<std::string> authors(authorsPtr, authorsPtr+numAuthors);info._authors = authors;

    const size_t numReferences = 2;
    const char* referencesPtr[numReferences] = {"Reference 1", "Reference 2"};
    const std::vector<std::string> references(referencesPtr, referencesPtr+numReferences);info._references = references;

    const std::string repositoryName("Some repository");
    const std::string repositoryURL("http://somewhere.org");
    const std::string repositoryDOI("this.is.a.doi");
    const std::string version("1.0.0");
    const std::string license("CC0");

    CHECK(title == info.getTitle());
    CHECK(id == info.getId());
    CHECK(description == info.getDescription());

    const std::vector<std::string>& keywordsT = info.getKeywords();
    REQUIRE(keywords.size() == keywordsT.size());
    for (size_t i = 0; i < keywords.size(); ++i) {
        CHECK(keywords[i] == keywordsT[i]);
    } // for

    CHECK(history == info.getHistory());
    CHECK(comment == info.getComment());

    CHECK(creatorName == info.getCreatorName());
    CHECK(creatorInstitution == info.getCreatorInstitution());
    CHECK(creatorEmail == info.getCreatorEmail());
    CHECK(acknowledgement == info.getAcknowledgement());

    const std::vector<std::string>& authorsT = info.getAuthors();
    REQUIRE(authors.size() == authorsT.size());
    for (size_t i = 0; i < authors.size(); ++i) {
        CHECK(authors[i] == authorsT[i]);
    } // for

    const std::vector<std::string>& referencesT = info.getReferences();
    REQUIRE(references.size() == referencesT.size());
    for (size_t i = 0; i < references.size(); ++i) {
        CHECK(references[i] == referencesT[i]);
    } // for

    CHECK(repositoryName == info.getRepositoryName());
    CHECK(repositoryURL == info.getRepositoryURL());
    CHECK(repositoryDOI == info.getRepositoryDOI());
    CHECK(version == info.getVersion());
    CHECK(license == info.getLicense());

    h5.close();

    h5.open("../../data/three-blocks-topo-missing-metadata.h5", H5F_ACC_RDONLY);
    CHECK_THROWS_AS(info.load(&h5), std::runtime_error);
} // testLoad


// End of file
