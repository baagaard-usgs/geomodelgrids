# App Python Object 

**Full name**: geomodelgrids.create.apps.create_model.App

## Data Members

+ **config** *(dict)* Configuration information.
+ **model** *([core.model.Model]())* Model information.
+ **show_progress** *(bool)* If True, print progress to stdout.

## Methods

+ [App()](py-api-create-apps-create-constructor)
+ [main()](py-api-create-apps-create-main)
+ [initialize()](py-api-create-apps-create-initialize)

(py-api-create-apps-create-constructor)=
### App

Constructor.

(py-api-create-apps-create-main)=
### main(**kwargs)

Application driver.

Keyword arguments:

+ **config[in]** *(str)* Name of configuration file.
+ **show_parameters[in]** *(bool)*, If True, print parameters to stdout (default: False)
+ **import_domain[in]** *(bool)*, If True, write domain information to model (default: False)
+ **import_surfaces[in]** *(bool)* If True, write surfaces information to model (default: False)
+ **import_blocks[in]** *(bool)* If True, write block information to model (default: False)
+ **all[in]** *(bool)* If True, equivalent to import_domain=True, import_surfaces=True, import_blocks=True (default: False)
+ **show_progress[in]** *(bool)* If True, print progress to stdout (default: True)
+ **log_filename[in]** *(str)*, Name of log file (default: create_model.log)
+ **debug[in]** *(bool)* Print additional debugging information to log file (default: False)

(py-api-create-apps-create-initialize)=
### initialize(config_filenames)

Set parameters from config file.

+ **config_filenames[in]** *(list of str)* Name of configuration file(s) with parameters.
