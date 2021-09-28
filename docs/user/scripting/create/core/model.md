# Model Python Object 

**Full name**: geomodelgrids.create.core.model.Model

## Data Members

+ **top_surface** *(Surface)* Surface for top of model.
+ **topo_bathy** *(Surface)* Surface for top of solid material.
+ **blocks** *(list of Block)* Blocks in model.
+ **config** *(dict)* Model parameters.
+ **storage** *(HDF5)* Interface for storing model.
+ **metadata** *(ModelMetadata)* Model metadata.

## Methods

+ [Model(config)](py-api-create-core-model-constructor)
+ [save_domain()](py-api-create-core-model-save-domain)
+ [init_top_surface()](py-api-create-core-model-init-top-surface)
+ [save_top_surface(elevation, batch)](py-api-create-core-model-save-top-surface)
+ [init_topography_bathymetry()](py-api-create-core-model-init-topography-bathymetry)
+ [save_topography_bathymetry(elevation, batch)](py-api-create-core-model-save-topography-bathymetry)
+ [init_block(block)](py-api-create-core-model-init-block)
+ [save_block(block, values, batch)](py-api-create-core-model-save-block)
+ [update_metadata()](py-api-create-core-model-update-metadata)
+ [get_attributes()](py-api-create-core-model-get-attributes)


(py-api-create-core-model-constructor)=
### Model(config)

Constructor.

+ **config[in]** *(dict)* Model parameters.

(py-api-create-core-model-save-domain)=
### save_domain()

Write domain information to storage.

(py-api-create-core-model-init-top-surface)=
### init_top_surface()

Create `top_surface` in storage

(py-api-create-core-model-save-top-surface)=
### save_top_surface(elevation, batch=None)

Write `top_surface` information to storage.

+ **elevation[in]** *(numpy.array [Nx, Ny])* Elevation of top surface.
+ **batch[in]** *(BatchGenerator2D)* Current batch of surface points.

(py-api-create-core-model-init-topography-bathymetry)=
### init_topography_bathymetry()

Create topography/bathymetry in storage.

(py-api-create-core-model-save-topography-bathymetry)=
### save_topography_bathymetry(elevation, batch=None)

Write `topography_bathymetry` information to storage.

+ **elevation[in]** *(numpy.array [Nx, Ny])* Elevation of topography/bathymetry surface.
+ **batch[in]** *(BatchGenerator2D)* Current batch of surface points.

(py-api-create-core-model-init-block)=
### init_block(block)

Create block in storage.

+ **block[in]** *(Block)* Block information.

(py-api-create-core-model-save-block)=
### save_block(block, values, batch=None)

Write block information to storage.

+ **block[in]** *(Block)* Block information.
+ **values[in]** *(numpy.array [Nx, Ny, Nz, Nv])* Gridded data associated with block.
+ **batch[in]** *(BatchGenerator3D)* Current batch of points in domain.

(py-api-create-core-model-update-metadata)=
### update_metadata()

Update all metadata for model using current model configuration.

(py-api-create-core-model-get-attributes)=
### get_attributes()

Get attributes for model.

+ **returns** Tuple of model attributes.
