# Serial Topography Object 

**Full name**: geomodelgrids::serial::Topography

## Methods

* [Topography()](#topography)
* [loadMetadata()](#loadmetadata)
* [getResolutionHoriz()](#double-getresolutionhoriz)
* [query(const double x, const double y)](#double-queryconst-double-x-const-double-y)


## Topography()

Constructor.

## loadMetadata()

Load metadata from the model file.


## double getResolutionHoriz()

Get horizontal resolution of topography grid.

* **returns** Horizontal resolution (m).


## double query(const double x, const double y)

Query for topography at a point using bilinear interpolation. 

* **x[in]** X coordinate of point in model coordinate system.
* **y[in]** Y coordinate of point in model coordinate system.

* **returns** Eelvation (m) of ground surface at point.
