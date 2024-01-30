# EarthVision API Python Object 

**Full name**: geomodelgrids.create.data_srcs.earthvision.api.EarthVisionAPI

API for running some specific EarthVision programs.

## Data Members

+ **model_dir** *(str)* Relative or absolute path of directory containing EarthVision model.
+ **env** *(dict) Environment variables for accessing EarthVision executables and libraries.

## Methods

+ [EarthVisionAPI(model_dir, env)](py-api-create-data-srcs-earthvision-api-constructor)
+ [ev_facedump(filename_faces)](py-api-create-data-srcs-earthvision-api-ev-facedump)
+ [ev_label(filename_values, filename_points, filename_model, dtype, converters)](py-api-create-data-srcs-earthvision-api-ev-label)
+ [ev_fp(formula, filename_out)](py-api-create-data-srcs-earthvision-api-ev-fp)

(py-api-create-data-srcs-earthvision-api-constructor)=
### EarthVisionAPI(model_dir, env)

Constructor.

+ **model_dir[in]** *(str)* Relative or absolute path of directory containing EarthVision model.
+ **env[in]** *(dict) Environment variables for accessing EarthVision executables and libraries.

(py-api-create-data-srcs-earthvision-api-ev-facedump)=
### ev_facedump(filename_faces)

Run 'ev_facedump {filename_faces}'.

+ **filename_faces[in]** *(str)* Name of faces file.
+ **returns** Output of `ev_facedump` as list of lines.

(py-api-create-data-srcs-earthvision-api-ev-label)=
### ev_label(filename_values, filename_points, filename_model, dtype, converters)

Run 'ev_label -m FILENAME_MODEL -o FILENAME_VALUES FILE'.

+ **filename_values** *(str)* Name of file for output values.
+ **filename_points** *(str)* Name of file with input points.
+ **filename_model** *(str)* Name of EarthVision model (.seq) file.
+ **dtype** *(dict)* Mapping of output columns to numpy type.
+ **converters** *(dict)* Mapping of output columns to function to convert to dtype.
+ **returns** numpy.array with output.

(py-api-create-data-srcs-earthvision-api-ev-fp)=
### ev_fp(formula, filename_out)

Run 'ev_fp < {formula}'.

+ **formula** *(str)* Formula for EarthVision formula processor.
+ **filename_out** *(str)* Name of output file in formula.
+ **returns** numpy.array with output.