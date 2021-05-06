# Objective

The GeoModelGrids project focuses on storing and querying
georeferenced raster-based models. The models are composed of blocks,
in which each block is a uniform grid with a potentially different
resolution.

# Motivation

* Store models using a self-describing, portable, widely-used storage
  scheme, [HDF5](https://www.hdfgroup.org/).
* Support queries on a wide range of platforms, from laptops and
  desktops to large clusters.
* Support variable resolution models that include topography.

# Use Cases

There are many possible use cases.
Our first target use case is storing and querying 3D seismic velocity models for simulation and analysis of earthquake ground motions.

