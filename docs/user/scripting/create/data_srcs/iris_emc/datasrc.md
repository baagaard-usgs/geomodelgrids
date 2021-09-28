# IRIS EMC DataSrc Python Object 

**Full name**: geomodelgrids.create.data_srcs.iris_emc.datasrc.EMCNetCDF

IRIS EMC data source using NetCDF file.

:::{important}
This data source requires the netCDF4 Python module.
:::

## Data Members

+ **config** *(dict)* Model parameters.

## Methods

+ [EMCNetCDF(config)](py-api-create-data-srcs-iris-emc-netcdf-constructor)
+ [initialize()](py-api-create-data-srcs-iris-emc-netcdf-initialize)
+ [get_metadata()](py-api-create-data-srcs-iris-emc-netcdf-get-metadata)
+ [get_top_surface(points)](py-api-create-data-srcs-iris-emc-netcdf-get-top-surface)
+ [get_topography_bathymetry(points)](py-api-create-data-srcs-iris-emc-netcdf-get-topography-bathymetry)
+ [get_values(block, top_surface, topo_bathy, batch)](py-api-create-data-srcs-iris-emc-netcdf-get-values)

(py-api-create-data-srcs-iris-emc-netcdf-constructor)=
### EMCNetCDF(config)

Constructor.

+ **config** *(dict)* Model parameters.

(py-api-create-data-srcs-iris-emc-netcdf-initialize)=
### initialize()

Initialize data source.

(py-api-create-data-srcs-iris-emc-netcdf-get-metadata)=
### get_metadata()

Get any additional metadata provided by data source.

+ **returns** Dict with additional metadata.

(py-api-create-data-srcs-iris-emc-netcdf-get-top-surface)=
### get_top_surface(points)

Query model for elevation of top surface at points.

:::{note}
We assume a flat top surface at an elevation of 0, so there is not top surface.
:::

+ **points[in]** *(numpy.array [Nx, Ny])* Coordinates of points in model coordinates.
+ **returns** `None`

(py-api-create-data-srcs-iris-emc-netcdf-get-topography-bathymetry)=
### get_topography_bathymetry(points)

Query model for elevation of topography/bathymetry at points.

:::{note}
We assume a flat top surface at an elevation of 0, so there is not top surface.
:::

+ **points[in]** *(numpy.array [Nx, Ny])* Coordinates of points in model coordinates.
+ **returns** `None`

(py-api-create-data-srcs-iris-emc-netcdf-get-values)=
### get_values(block, top_surface, topo_bathy, batch=None)

Query model for values at points.

+ **block[in]** *(Block)* Block information.
+ **top_surface[in]** *(Surface)* Top surface.
+ **topo_bathy[in]** *(Surface)* Topography/bathymetry surface to define depth.
+ **batch[in]** *(BatchGenerator3D)* Current batch of points in block.
+ **returns** Values at points.
  