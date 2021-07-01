## Version 1.0.0rc1

### Known issues

* The interpolation algorithm does not respect fault block and zone boundaries and the boundary between water and solid materials. This means fault block and zone ids will be interpolated across block and zone boundaries, and Vp and density will be interpolated Vp and density across the boundary between water and solid materials. Vs is not interpolated across the boundary between water and solid material, because it is not defined in water.
* GeoModelGrids does not support unit conversions. All values returned in queries are in the units of the underlying models. We do check that all models have consistent units for values contained in multiple models. Queries for the elevations of the top surface or topography/bathymetry will be returned in the units of the input Coordinate Reference System.
* The model storage has been optimized for faster successive queries in the vertical direction. That is, querying for points on a vertical slice will generally be faster than querying the same number of points on a horizontal slice.

