(cxx-api-serial-modelinfo)=
# ModelInfo

**Full name**: geomodelgrids::serial::ModelInfo

## Methods

### ModelInfo()

Constructor.

### const std::string& getTitle()

Get title.

- **returns** Title of model.

### const std::string& getId()

Get identifier.

- **returns** Model identifier.

### const std::string& getDescription()

Get description.

- **returns** Model description.

### const std::vector\<std::string\>& getKeywords()

Get keywords describing model.

- **returns** Array of keywords.

### const std::string& getHistory()

Get model history.

- **returns** Model history.

### const std::string& getComment()

Get comment.

- **returns** Comment about model.

### const std::string& getCreatorName()

Get name of creator.

- **returns** Name of creator.

### const std::string& getCreatorInstitution()

Get institution of creator.

- **returns** Institution of creator.

### const std::string& getCreatorEmail()

Get email of creator.

- **returns** Email of creator.

### const std::string& getAcknowledgement()

Get acknowledgements for model.

- **returns** acknowledgements for model.

### const std::vector\<std::string\>& getAuthors()

Get authors of model.

- **returns** Array of author names.

### const std::vector\<std::string\>& getReferences()

Get references associated with model.

- **returns** Array of references.

### const std::string& getRepositoryName()

Get name of repository holding model.

- **returns** Name of repository.

### const std::string& getRepositoryURL()

Get URL of repository holding model.

- **returns** URL for repository.

### const std::string& getRepositoryDOI()

Get DOI for model.

- **returns** Digital Object Identifier.

### const std::string& getVersion()

Get model version.

- **returns** Model version.

### const std::string& getLicense()

Get model license.

- **returns** Name of license for model.

### const std::string& getAuxiliary()

Get auxiliary information.

- **returns** Auxiliary information as string.

### load(geomodelgrids::serial::HDF5* const h5)

Load model information.
