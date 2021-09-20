# Serial ModelInfo Object 

**Full name**: geomodelgrids::serial::ModelInfo

## Methods

+ [ModelInfo()](cxx-api-serial-modelinfo-ModelInfo)
+ [getTitle()](cxx-api-serial-modelinfo-getTitle)
+ [getId()](cxx-api-serial-modelinfo-getId)
+ [getDescription()](cxx-api-serial-modelinfo-getDescription)
+ [getKeywords()](cxx-api-serial-modelinfo-getKeywords)
+ [getHistory()](cxx-api-serial-modelinfo-getHistory)
+ [getComment()](cxx-api-serial-modelinfo-getComment)
+ [getCreatorName()](cxx-api-serial-modelinfo-getCreatorName)
+ [getCreatorInstitution()](cxx-api-serial-modelinfo-getCreatorInstitution)
+ [getCreatorEmail()](cxx-api-serial-modelinfo-getCreatorEmail)
+ [getAckowledgment()](cxx-api-serial-modelinfo-getAcknowledgement)
+ [getAuthors()](cxx-api-serial-modelinfo-getAuthors)
+ [getReferences()](cxx-api-serial-modelinfo-getReferences)
+ [getRepositoryName()](cxx-api-serial-modelinfo-getRepositoryName)
+ [getRepositoryURL()](cxx-api-serial-modelinfo-getRepositoryURL)
+ [getRepositoryDOI()](cxx-api-serial-modelinfo-getRepositoryDOI)
+ [getVersion()](cxx-api-serial-modelinfo-getVersion)
+ [getLicense()](cxx-api-serial-modelinfo-getLicense)
+ [getAuxiliary()](cxx-api-serial-modelinfo-getAuxiliary)
+ [load(geomodelgrids::serial::HDF5* const h5)](cxx-api-serial-modelinfo-load)

(cxx-api-serial-modelinfo-ModelInfo)=
### ModelInfo()

Constructor.

(cxx-api-serial-modelinfo-getTitle)=
### const std::string& getTitle()

Get title.

* **returns** Title of model.

(cxx-api-serial-modelinfo-getId)=
### const std::string& getId()

Get identifier.

* **returns** Model identifier.

(cxx-api-serial-modelinfo-getDescription)=
### const std::string& getDescription()

Get description.

* **returns** Model description.

(cxx-api-serial-modelinfo-getKeywords)=
### const std::vector\<std::string\>& getKeywords()

Get keywords describing model.

* **returns** Array of keywords.

(cxx-api-serial-modelinfo-getHistory)=
### const std::string& getHistory()

Get model history.

* **returns** Model history.

(cxx-api-serial-modelinfo-getComment)=
### const std::string& getComment()

Get comment.

* **returns** Comment about model.

(cxx-api-serial-modelinfo-getCreatorName)=
### const std::string& getCreatorName()

Get name of creator.

* **returns** Name of creator.

(cxx-api-serial-modelinfo-getCreatorInstitution)=
### const std::string& getCreatorInstitution()

Get institution of creator.

* **returns** Institution of creator.

(cxx-api-serial-modelinfo-getCreatorEmail)=
### const std::string& getCreatorEmail()

Get email of creator.

* **returns** Email of creator.

(cxx-api-serial-modelinfo-getAcknowledgement)=
### const std::string& getAcknowledgement()

Get acknowledgements for model.

* **returns** acknowledgements for model.

(cxx-api-serial-modelinfo-getAuthors)=
### const std::vector\<std::string\>& getAuthors()

Get authors of model.

* **returns** Array of author names.

(cxx-api-serial-modelinfo-getReferences)=
### const std::vector\<std::string\>& getReferences()

Get references associated with model.

* **returns** Array of references.

(cxx-api-serial-modelinfo-getRepositoryName)=
### const std::string& getRepositoryName()

Get name of repository holding model.

* **returns** Name of repository.

(cxx-api-serial-modelinfo-getRepositoryURL)=
### const std::string& getRepositoryURL()

Get URL of repository holding model.

* **returns** URL for repository.

(cxx-api-serial-modelinfo-getRepositoryDOI)=
### const std::string& getRepositoryDOI()

Get DOI for model.

* **returns** Digital Object Identifier.

(cxx-api-serial-modelinfo-getVersion)=
### const std::string& getVersion()

Get model version.

* **returns** Model version.

(cxx-api-serial-modelinfo-getLicense)=
### const std::string& getLicense()

Get model license.

* **returns** Name of license for model.

(cxx-api-serial-modelinfo-getAuxiliary)=
### const std::string& getAuxiliary()

Get auxiliary information.

* **returns** Auxiliary information as string.

(cxx-api-serial-modelinfo-load)=
### load(geomodelgrids::serial::HDF5* const h5)

Load model information.
