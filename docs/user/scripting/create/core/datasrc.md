# DataSrc Python Object

**Full name**: geomodelgrids.create.core.datasrc.DataSrc

Abstract base class defining the data source interface.

## Methods

+ [DataSrc()](py-api-create-core-constructor)
+ [initialize()](py-api-create-core-initialize)
+ [get_metadata()](py-api-create-core-get-metadata)
+ [get_top_surface(points)](py-api-create-core-get-top-surface)
+ [get_topography_bathymetry(points)](py-api-create-core-get-topography-bathymetry)
+ [get_values(block, top_surface, topo_bathy, batch)](py-api-create-core-get-values)

(py-api-create-core-constructor)=
### DataSrc()

Constructor.

(py-api-create-core-initialize)=
### initialize()

Initialize data source.

(py-api-create-core-get-metadata)=
### get_metadata()

Get any additional metadata provided by data source.

+ **returns** Dict with additional metadata.

(py-api-create-core-get-top-surface)=
### get_top_surface(points)

Query model for elevation of top surface at points.

+ **points[in]** *(numpy.array [Nx, Ny])* Coordinates of points in model coordinates.
+ **returns** Elevation of top surface at points.

(py-api-create-core-get-topography-bathymetry)=
### get_topography_bathymetry(points)

Query model for elevation of topography/bathymetry at points.

+ **points[in]** *(numpy.array [Nx, Ny])* Coordinates of points in model coordinates.
+ **returns** Elevation of topography/bathymetry surface at points.

(py-api-create-core-get-values)=
### get_values(block, top_surface, topo_bathy, batch=None)

Query model for values at points.

+ **block[in]** *(Block)* Block information.
+ **top_surface[in]** *(Surface)* Top surface.
+ **topo_bathy[in]** *(Surface)* Topography/bathymetry surface to define depth.
+ **batch[in]** *(BatchGenerator3D)* Current batch of points in block.
+ **returns** Values at points.
  