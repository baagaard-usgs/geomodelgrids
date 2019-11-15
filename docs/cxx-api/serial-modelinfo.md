# Serial ModelInfo Object 

**Full name**: geomodelgrids::serial::ModelInfo

## Methods

* [ModelInfo()](#modelinfo)
* [getTitle()](#const-stdstring-gettitle)
* [getId()](#const-stdstring-getid)
* [getDescription()](#const-stdstring-getdescription)
* [getKeywords()](#const-stdvectorstdstring-getkeywords)
* [getCreatorName()](#const-stdstring-getcreatorname)
* [getCreatorEmail()](#const-stdstring-getcreatoremail)
* [getCreatorInstitution()](#const-stdstring-getcreatorinstitution)
* [getAckowledgments()](#const-stdstring-getacknowledgments)
* [getAuthors()](#const-stdvectorstdstring-getauthors)
* [getReferences()](#const-stdvectorstdstring-getreferences)
* [getDOI()](#const-stdstring-getdoi)
* [getVersion()](#const-stdstring-getversion)
* [load(geomodelgrids::serial::HDF5* const h5)](#loadgeomodelgridsserialhdf5-const-h5)


## ModelInfo()

Constructor.

## const std::string& getTitle()

Get title.

* **returns** Title of model.


## const std::string& getId()

Get identifier.

* **returns** Model identifier.


## const std::string& getDescription()

Get description.

* **returns** Model description.


## const std::vector\<std::string\>& getKeywords()

Get keywords describing model.

* **returns** Array of keywords.


## const std::string& getCreatorName()

Get name of creator.

* **returns** Name of creator.


## const std::string& getCreatorEmail()

Get email of creator.

* **returns** Email of creator.


## const std::string& getCreatorInstitution()

Get institution of creator.

* **returns** Institution of creator.


## const std::string& getAcknowledgments()

Get acknowledgments for model.

* **returns** Acknowledgments for model.


## const std::vector\<std::string\>& getAuthors()

Get authors of model.

* **returns** Array of author names.


## const std::vector\<std::string\>& getReferences()

Get references associated with model.

* **returns** Array of references.


## const std::string& getDOI()

Get DOI for model.

* **returns** Digital Object Identifier.


## const std::string& getVersion()

Get model version.

* **returns** Model version.


## load(geomodelgrids::serial::HDF5* const h5)

Load model information.


