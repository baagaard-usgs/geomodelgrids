# Objective

The GeoModelGrids project focuses on storing and querying
georeferenced grid-based models. The models are composed of blocks,
where each block is a uniform grid with potentially different
resolutions.

# Motivation

* Store models using a self-describing, portable, widely-used storage
  scheme, [HDF5](https://www.hdfgroup.org/).
* Support queries on a wide range of platforms, from laptops and
  desktops to large clusters.
* Support variable resolution models that include topography.

# Target Use Cases

* Storing and querying 3-D seismic velocity models in running computer
simulations of earthquake ground motions.

