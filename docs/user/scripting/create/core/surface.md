# Surface Python Object 

**Full name**: geomodelgrids.create.core.model.Surface

## Data Members

+ **name** *(str)* Name of surface.
+ **model_metadata** *(ModelMetadata)* Metadata for model domain containing the block.
+ **x_resolution** *(float)* Resolution in x direction if uniform resolution, otherwise `None`.
+ **y_resolution** *(float)* Resolution in y direction if uniform resolution, otherwise `None`.
+ **x_coordinates** *(tuple)* Coordinates in x direction if variable resolution, otherwise `None`.
+ **y_coordinates** *(tuple)* Coordinates in y direction if variable resolution, otherwise `None`.
+ **chunk_size** *(tuple)* Dimensions of dataset chunk.
+ **storage** *(HDF5)* Storage for surface.

## Methods

+ [Surface(name, model_metadata, config, storage)](py-api-create-core-surface-constructor)
+ [get_dims()](py-api-create-core-surface-get-dims)
+ [get_batches(batch_size)](py-api-create-core-surface-get-batches)
+ [generate_points(batch)](py-api-create-core-surface-generate-points)
+ [get_attributes()](py-api-create-core-surface-get-attributes)
  
(py-api-create-core-surface-constructor)=
### Surface(name, model_metadata, config, storage)

Constructor.

+ **name[in]** *(str) Name of block.
+ **model_metadata[in]** *(ModelMetadata)* Metadata for model domain containing the block.
+ **config[in]** *(dict)* Block parameters as a dictionary.
  + `x_resolution` *(float)* Resolution in x direction (if uniform resolution).
  + `y_resolution` *(float)* Resolution in y direction (if uniform resolution).
  + `x_coordinates` *(float)* Coordinates in x direction (if variable resolution).
  + `y_coordinates` *(float)* Coordinates in y direction (if variable resolution).
  + `chunk_size` *(tuple)* Dimensions of dataset chunk (should be about 10Kb - 1Mb).
+ **storage[in]** *(HDF5)* Storage for surface.

(py-api-create-core-surface-get-dims)=
### get_dims()

Get number of points in surface along each dimension.

+ **returns** Tuple of number of points along each dimension (num_x, num_y).

(py-api-create-core-surface-get-batches)=
### get_batches(batch_size)

Get generator for batches of points.

+ **returns** Current batch of points.

(py-api-create-core-surface-generate-points)=
### generate_points(batch=None)

Generate points for surface.

+ **batch** *(BatchGenerator2D)* Current batch of points for elevation data.
+ **returns** *(numpy.array [Nx*Ny, 2])* 2D array of point locations for ground surface.

(py-api-create-core-surface-get-attributes)=
### get_attributes()

Get attributes for surface.

+ **returns** Tuple of surface attributes.
