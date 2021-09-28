# Overview

The GeoModelGrids project focuses on storing and querying georeferenced raster-based models with an emphasis on regional models.
The models are composed of blocks, in which each block is a grid with a potentially different resolution that vary along the x, y, or z axis.


## Motivation

* Support variable resolution models that include topography and are in any standard geographic projection.
* Support querying for model values on a wide range of platforms, from laptops and desktops to large clusters.

## Features

* Store models using a self-describing, portable, widely-used storage scheme, [HDF5](https://www.hdfgroup.org/).
* Models are composed of an arbitrary number of blocks of uniform or variable resolution.
* Models domains may be in wide variety of georeferenced coordinate systems and need not be aligned with the x and y axes of the geographic projection.
* If the model includes topography, the blocks will be warped to conform to the ground surface. Queries for applications requiring a flat top surface can use "squashing" relative to either the top surface of the of the model or topography/bathymetry (if provided).
* In serial queries (intended for use on laptops and desktops), only a portion of the model is loaded into memory, thereby allowing queries of models that are much larger than the available memory.
* In addition to discretization information, the self-describing format includes description of the names of the values and their units, and coordinate system information.

## Use Cases

There are many possible use cases.
Our first target use case is storing and querying 3D seismic velocity models for simulation and analysis of earthquake ground motions.
