# Block Python Object 

**Full name**: geomodelgrids.create.core.model.Block

## Data Members

+ **name** *(str)* Name of block.
+ **model_metadata** *(ModelMetadata)* Metadata for model domain containing the block.
+ **x_resolution** *(float)* Resolution in x direction if uniform resolution, otherwise `None`.
+ **y_resolution** *(float)* Resolution in y direction if uniform resolution, otherwise `None`.
+ **z_resolution** *(float)* Resolution in z direction if uniform resolution, otherwise `None`.
+ **x_coordinates** *(tuple)* Coordinates in x direction if variable resolution, otherwise `None`.
+ **y_coordinates** *(tuple)* Coordinates in y direction if variable resolution, otherwise `None`.
+ **z_coordinates** *(tuple)* Coordinates in z direction if variable resolution, otherwise `None`.
+ **z_top** *(float)* Elevation of top of block.
+ **z_bot** *(float)* Elevation of bottom of block.
+ **z_top_offset** *(float)* Vertical offset of top slice of points below top of block.
+ **chunk_size** *(tuple)* Dimensions of dataset chunk.

## Methods

+ [Block(name, model_metadata, config)](py-api-create-core-block-constructor)
+ [get_dims()](py-api-create-core-block-get-dims)
+ [get_batches()](py-api-create-core-block-get-batches)
+ [generate_points(top_surface, batch)](py-api-create-core-block-generate-points)
+ [get_surface(surface, batch)](py-api-create-core-block-get-surface)
+ [get_attributes()](py-api-create-core-block-get-attributes)

(py-api-create-core-block-constructor)=
### Block(name, model_metadata, config)

+ **name[in]** *(str) Name of block.
+ **model_metadata[in]** *(ModelMetadata)* Metadata for model domain containing the block.
+ **config[in]** *(dict)* Block parameters as a dictionary.
  + `x_resolution` *(float)* Resolution in x direction (if uniform resolution).
  + `y_resolution` *(float)* Resolution in y direction (if uniform resolution).
  + `z_resolution` *(float)* Resolution in z direction (if uniform resolution).
  + `x_coordinates` *(float)* Coordinates in x direction (if variable resolution).
  + `y_coordinates` *(float)* Coordinates in y direction (if variable resolution).
  + `z_coordinates` *(float)* Coordinates in z direction (if variable resolution).
  + `z_top` *(float)* Elevation of top of block if uniform resolution in z direction.
  + `z_bot` *(float)* Elevation of bottom of block if uniform resolution in z direction.
  + `z_top_offset` *(float)* Vertical offset of top slice of points below top of block (used to avoid roundoff errors).
  + `chunk_size` *(tuple)* Dimensions of dataset chunk (should be about 10Kb - 1Mb).

(py-api-create-core-block-get-dims)=
### get_dims()

Get number of points in block along each dimension.

+ **returns** Tuple of number of points along each dimension (num_x, num_y, num_z).

(py-api-create-core-block-get-batches)=
### get_batches(batch_size)

Get batch generator for block.

+ **returns** BatchGenerator3D for block.

(py-api-create-core-block-generate-points)=
### generate_points(top_surface, batch)

Generate grid of points in block.

+ **top_surface[in]** *(Surface)* Elevation of top surface of model domain.
+ **batch[in]** *(BatchGenerator3D)* Current batch of points in block.
+ **returns** 3D array (Nx*Ny*Nz,3) of point locations in block.

(py-api-create-core-block-get-surface)=
### get_surface(surface, batch=None)

Get surface grid for block.

+ **surface** *(Surface)* Surface in model domain.
+ **batch** *(BatchGenerator3D)* Current batch of points in block.
+ **returns** *(numpy array)* [Nx,Ny] with elevation of surface for current batch in block.

(py-api-create-core-block-get-attributes)=
### get_attributes()

Get attributes associated with block.

+ **returns** Array of tuples with attributes for block.
