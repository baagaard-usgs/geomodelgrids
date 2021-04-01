/**
 * C++ unit testing of geomodelgrids::serial::ModelInfo.
 */

#include <portinfo>

#include "geomodelgrids/serial/ModelInfo.hh" // USES ModelInfo
#include "geomodelgrids/serial/HDF5.hh" // USES HDF5

#include <cppunit/extensions/HelperMacros.h>

namespace geomodelgrids {
    namespace serial {
        class TestModelInfo;
    } // serial
} // geomodelgrids

class geomodelgrids::serial::TestModelInfo : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestModelInfo);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testLoad);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Test constructor.
    void testConstructor(void);

    /// Test getters.
    void testAccessors(void);

    /// Test load().
    void testLoad(void);

}; // class TestModelInfo
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::serial::TestModelInfo);

// ----------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::serial::TestModelInfo::testConstructor(void) {
    ModelInfo info;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expect empty title", std::string(""), info._title);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expect empty id", std::string(""), info._id);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expect empty description", std::string(""), info._description);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expect empty creator name", std::string(""), info._creatorName);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expect empty creator email", std::string(""), info._creatorEmail);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expect empty creator institution", std::string(""), info._creatorInstitution);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expect empty acknowledgements", std::string(""), info._acknowledgements);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expect empty DOI", std::string(""), info._doi);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expect empty version", std::string(""), info._version);
    CPPUNIT_ASSERT_MESSAGE("Expect empty keywords array", info._keywords.empty());
    CPPUNIT_ASSERT_MESSAGE("Expect empty authors array", info._authors.empty());
    CPPUNIT_ASSERT_MESSAGE("Expect empty references array", info._references.empty());
} // testConstructor


// ----------------------------------------------------------------------
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

    const std::string creatorName("Name");info._creatorName = creatorName;
    const std::string creatorInstitution("Institution");info._creatorInstitution = creatorInstitution;
    const std::string creatorEmail("email@organization.org");info._creatorEmail = creatorEmail;
    const std::string acknowledgements("Acknowledgements");info._acknowledgements = acknowledgements;

    const size_t numAuthors = 4;
    const char* authorsPtr[numAuthors] = {"Name 0", "Name 1", "Name 2", "Name 3"};
    const std::vector<std::string> authors(authorsPtr, authorsPtr+numAuthors);info._authors = authors;

    const size_t numReferences = 2;
    const char* referencesPtr[numReferences] = {"Reference 0", "Reference 1"};
    const std::vector<std::string> references(referencesPtr, referencesPtr+numReferences);info._references = references;

    const std::string doi("XXXX.XXX/XXX");info._doi = doi;
    const std::string version("1.2.3");info._version = version;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking title", title, info.getTitle());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking id", id, info.getId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking description", description, info.getDescription());

    const std::vector<std::string>& keywordsT = info.getKeywords();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking keywords size", keywords.size(), keywordsT.size());
    for (size_t i = 0; i < keywords.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking keywords", keywords[i], keywordsT[i]);
    } // for

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking creatorName", creatorName, info.getCreatorName());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking creatorInstitution", creatorInstitution, info.getCreatorInstitution());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking creatorEmail", creatorEmail, info.getCreatorEmail());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking acknowledgements", acknowledgements, info.getAcknowledgements());

    const std::vector<std::string>& authorsT = info.getAuthors();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking authors size", authors.size(), authorsT.size());
    for (size_t i = 0; i < authors.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking authors", authors[i], authorsT[i]);
    } // for

    const std::vector<std::string>& referencesT = info.getReferences();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking references size", references.size(), referencesT.size());
    for (size_t i = 0; i < references.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking references", references[i], referencesT[i]);
    } // for

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking doi", doi, info.getDOI());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking version", version, info.getVersion());
} // testAccessors


// ----------------------------------------------------------------------
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

    const std::string creatorName("John Doe");
    const std::string creatorInstitution("Agency");
    const std::string creatorEmail("johndoe@agency.org");
    const std::string acknowledgements("Thank you!");

    const size_t numAuthors = 3;
    const char* authorsPtr[numAuthors] = {"Smith, Jim", "Doe, John", "Doyle, Sarah"};
    const std::vector<std::string> authors(authorsPtr, authorsPtr+numAuthors);info._authors = authors;

    const size_t numReferences = 2;
    const char* referencesPtr[numReferences] = {"Reference 1", "Reference 2"};
    const std::vector<std::string> references(referencesPtr, referencesPtr+numReferences);info._references = references;

    const std::string doi("this.is.a.doi");
    const std::string version("1.0.0");

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking title", title, info.getTitle());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking id", id, info.getId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking description", description, info.getDescription());

    const std::vector<std::string>& keywordsT = info.getKeywords();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking keywords size", keywords.size(), keywordsT.size());
    for (size_t i = 0; i < keywords.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking keywords", keywords[i], keywordsT[i]);
    } // for

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking creatorName", creatorName, info.getCreatorName());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking creatorInstitution", creatorInstitution, info.getCreatorInstitution());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking creatorEmail", creatorEmail, info.getCreatorEmail());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking acknowledgements", acknowledgements, info.getAcknowledgements());

    const std::vector<std::string>& authorsT = info.getAuthors();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking authors size", authors.size(), authorsT.size());
    for (size_t i = 0; i < authors.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking authors", authors[i], authorsT[i]);
    } // for

    const std::vector<std::string>& referencesT = info.getReferences();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking references size", references.size(), referencesT.size());
    for (size_t i = 0; i < references.size(); ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking references", references[i], referencesT[i]);
    } // for

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking doi", doi, info.getDOI());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Checking version", version, info.getVersion());
} // testLoad


// End of file
