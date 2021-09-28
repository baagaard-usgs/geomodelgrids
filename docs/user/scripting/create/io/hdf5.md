# HDF5Storage Python Object 

**Full name**: geomodelgrids.create.io.hdf5.HDF5Storage

HDF5 file for storing gridded model.

## Data Members

+ **filename** *(str)* Name of HDF5 file.

## Methods

+ [HDF5Storage(filename)](py-api-create-io-hdf5storage-constructor)
+ [save_domain(domain)](py-api-create-io-hdf5storage-save-domain)
+ [create_surface(surface)](py-api-create-io-hdf5storage-create-surface)
+ [save_surface_metadata(surface)](py-api-create-io-hdf5storage-save-surface-metadata)
+ [save_surface(surface, elevation, batch)](py-api-create-io-hdf5storage-save-surface)
+ [load_surface(surface, batch)](py-api-create-io-hdf5storage-load-surface)
+ [create_block(block)](py-api-create-io-hdf5storage-create-block)
+ [save_block_metadata(block)](py-api-create-io-hdf5storage-save-block-metadata)
+ [save_block(block, data, batch)](py-api-create-io-hdf5storage-save-block)

(py-api-create-io-hdf5storage-constructor)=
### HDF5Storage(filename)

Constructor.

+ **filename[in]** *(str) Name for HDF5 file.

(py-api-create-io-hdf5storage-save-domain)=
### save_domain(domain)

Write domain attributes to HDF5 file.

+ **domain[in]** *(Model)* Model domain.

(py-api-create-io-hdf5storage-create-surface)=
### create_surface(surface)

Create surface in HDF5 file.

+ **surface[in]** *(Surface)* Model surface.

(py-api-create-io-hdf5storage-save-surface-metadata)=
### save_surface_metadata(surface)

Write surface metadata to HDF5 file.

+ **surface[in]** *(Surface)* Model surface

(py-api-create-io-hdf5storage-save-surface)=
### save_surface(surface, elevation, batch=None)

Write surface to HDF5 file.

+ **surface** *(Surface)* Model surface.
+ **elevation** *(numpy.array)* Elevation of surface.
+ **batch** *(BatchGenerator2D)* Current batch of surface points.

(py-api-create-io-hdf5storage-load-surface)=
### load_surface(surface, batch=None)

Load surface from HDF5 file.

+ **surface** *(Surface)* Model surface.
+ **batch** *(BatchGenerator2D)* Current batch of surface points.
+ **returns** numpy.array with elevation of surface.

(py-api-create-io-hdf5storage-create-block)=
### create_block(block)

Create block in HDF5 file.

+ **block** *(Block)* Block in model.

(py-api-create-io-hdf5storage-save-block-metadata)=
### save_block_metadata(block)

Write block metadata to HDF5 file.

+ **block[in]** *(Block)* Block associated with gridded data.

(py-api-create-io-hdf5storage-save-block)=
### save_block(block, data, batch=None)

Write block data to HDF5 file.

+ **block** *(Block)* Block in model.
+ **data** *(numpy.array) [Nx, Ny, Nz, Nv]* Array of gridded data.
+ **batch** *(BatchGenerator3D)* Current batch of block points.
