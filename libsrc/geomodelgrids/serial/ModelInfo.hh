/** Model stored as HDF5 file.
 */

#if !defined(geomodelgrids_serial_modelinfo_hh)
#define geomodelgrids_serial_modelinfo_hh

// Include directives -----------------------------------------------------------------------------
#include "serialfwd.hh" // forward declarations

#include <vector> // HASA std::std::vector
#include <string> // HASA std::string

class geomodelgrids::serial::ModelInfo {
    friend class TestModelInfo; // Unit testing

    // PUBLIC METHODS -----------------------------------------------------------------------------
public:

    /// Default constructor.
    ModelInfo(void);

    /// Destructor
    ~ModelInfo(void);

    /** Get title.
     *
     * @returns Title of model.
     */
    const std::string& getTitle(void) const;

    /** Get identifier.
     *
     * @returns Model identifier.
     */
    const std::string& getId(void) const;

    /** Get description.
     *
     * @returns Model description.
     */
    const std::string& getDescription(void) const;

    /** Get keywords describing model.
     *
     * @returns Array of keywords.
     */
    const std::vector<std::string>& getKeywords(void) const;

    /** Get name of creator.
     *
     * @returns Name of creator.
     */
    const std::string& getCreatorName(void) const;

    /** Get email of creator.
     *
     * @returns Email of creator.
     */
    const std::string& getCreatorEmail(void) const;

    /** Get institution of creator.
     *
     * @returns Institution of creator.
     */
    const std::string& getCreatorInstitution(void) const;

    /** Get acknowledgments.
     *
     * @returns Acknowledgments for model.
     */
    const std::string& getAcknowledgements(void) const;

    /** Get authors of model.
     *
     * @returns Array of author names.
     */
    const std::vector<std::string>& getAuthors(void) const;

    /** Get references associaed with model.
     *
     * @returns Array of references.
     */
    const std::vector<std::string>& getReferences(void) const;

    /** Get DOI for model.
     *
     * @returns Digital Object Identifier.
     */
    const std::string& getDOI(void) const;

    /** Get model version.
     *
     * @returns ModelInfo version.
     */
    const std::string& getVersion(void) const;

    /** Get auxiliary information.
     *
     * @returns Json as string.
     */
    const std::string& getAuxiliary(void) const;

    /** Load metadata.
     */
    void load(geomodelgrids::serial::HDF5* const h5);

    // PRIVATE MEMBERS ----------------------------------------------------------------------------
private:

    std::string _title; ///< Title of model.
    std::string _id; ///< ModelInfo identifier.
    std::string _description; ///< ModelInfo description.
    std::vector<std::string> _keywords; ///< Keywords describing model.
    std::string _creatorName; ///< Name of person creating model.
    std::string _creatorEmail; ///< Email of creator.
    std::string _creatorInstitution; ///< Institution of creator.
    std::string _acknowledgements; ///< Acknowledgments for model.
    std::vector<std::string> _authors; ///< Name of authors.
    std::vector<std::string> _references; ///< References for model.
    std::string _doi; ///< Digital Object Identifier for model.
    std::string _version; ///< ModelInfo version.
    std::string _auxiliary; ///< Auxiliary information (optional).

    // NOT IMPLEMENTED ----------------------------------------------------------------------------
private:

    ModelInfo(const ModelInfo&); ///< Not implemented
    const ModelInfo& operator=(const ModelInfo&); ///< Not implemented

}; // ModelInfo

#endif // geomodelgrids_serial_modelinfo_hh

// End of file
