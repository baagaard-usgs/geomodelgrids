# EarthVision DataSrc Python Object 

**Full name**: geomodelgrids.create.data_srcs.earthvision.datasrc.RulesDataSrc

EarthVision model constructed from rules applies to fault blocks and zones.

:::{important}
This data source requires the EarthVision software and a corresponding license.
:::

## Data Members

+ **config** *(dict)* Model parameters.
+ **model_dir** *(str)* Relative or absolute path to directory containing EarthVision model.
+ **api** *(api)* EarthVision API.
+ **faultblock_ids** *(dict)* Mapping of fault block names to ids.
+ **zone_ids** *(dict)* Mapping of zone names to ids.

## Methods

+ [RulesDataSrc(config)](py-api-create-data-srcs-earthvision-rulesdatasrc-constructor)
+ [initialize()](py-api-create-data-srcs-earthvision-rulesdatasrc-initialize)
+ [get_metadata()](py-api-create-data-srcs-earthvision-rulesdatasrc-get-metadata)
+ [get_top_surface(points)](py-api-create-data-srcs-earthvision-rulesdatasrc-get-top-surface)
+ [get_topography_bathymetry(points)](py-api-create-data-srcs-earthvision-rulesdatasrc-get-topography-bathymetry)
+ [get_values(block, top_surface, topo_bathy, batch)](py-api-create-data-srcs-earthvision-rulesdatasrc-get-values)

(py-api-create-data-srcs-earthvision-rulesdatasrc-constructor)=
### DataSrc(config)

Constructor.

+ **config** *(dict)* Model parameters.

(py-api-create-data-srcs-earthvision-rulesdatasrc-initialize)=
### initialize()

Initialize data source.

(py-api-create-data-srcs-earthvision-rulesdatasrc-get-metadata)=
### get_metadata()

Get any additional metadata provided by data source.

+ **returns** Dict with additional metadata.

(py-api-create-data-srcs-earthvision-rulesdatasrc-get-top-surface)=
### get_top_surface(points)

Query model for elevation of top surface at points.

+ **points[in]** *(numpy.array [Nx, Ny])* Coordinates of points in model coordinates.
+ **returns** `None`

(py-api-create-data-srcs-earthvision-rulesdatasrc-get-topography-bathymetry)=
### get_topography_bathymetry(points)

Query model for elevation of topography/bathymetry at points.

+ **points[in]** *(numpy.array [Nx, Ny])* Coordinates of points in model coordinates.
+ **returns** `None`

(py-api-create-data-srcs-earthvision-rulesdatasrc-get-values)=
### get_values(block, top_surface, topo_bathy, batch=None)

Query model for values at points.

+ **block[in]** *(Block)* Block information.
+ **top_surface[in]** *(Surface)* Top surface.
+ **topo_bathy[in]** *(Surface)* Topography/bathymetry surface to define depth.
+ **batch[in]** *(BatchGenerator3D)* Current batch of points in block.
+ **returns** Values at points.
  