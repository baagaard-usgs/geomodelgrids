# Storage Layout

## Model Representation

We map the physical space of the model domain bounded on top by topography (or a flat surface) into logical space that has uniform regular grids.

<figure>
  <img src="figs/gridmapping.png" alt="Diagram of physical and logical grids">
</figure>

The mapping from the physical space to topological space is:
```
z_logical = -dim_z * (z_topography - z_physical) / (z_topography + dim_z)
```

## Data layout

We use a Cartesian coordinate system for the model logical space with the origin at the top southwest corner (when the y axis is aligned with north) as shown in the figure. We measure the rotation of the coordinate system for the model logical space using the azimuth of the y direction relative to north (`y_azimuth` in the diagram).

<figure>
  <img src="figs/coordsys.png" alt="Diagram of Cartesian coordinate
  system of the model logical space">
</figure>

For the logical space 4D arrays the x, y, and z indices are zero at the origin. The x index increases in the +x direction, the y index increases in the +y direction, and the z index increases in the -z direction.


## HDF5 Storage Scheme

The model is stored in an HDF5 file. If the top surface of the model is not a flat surface at sea level, then a `surfaces/top_surface` dataset provides the elevation of the top surface. Models may also include a `surfaces/topography_bathymetry` dataset to define topography and bathymetry, which corresponds to the top of the solid surface. The grids are stored in the `blocks` group. Attributes are included at the root level as well as for each dataset to yield a self-describing model. That is, no additional metadata is needed to define the model.

<figure>
  <img src="figs/hdf5layout.png" alt="Diagram of HDF5 layout scheme">
</figure>

## Model Metadata

### Description

* **title** *(string)* Title of the model.
* **id** *(string)* Model identifier.
* **description** *(string)* Description of the model.
* **keywords** *(array of strings)* Keywords describing the model.
* **creator_name** *(string)* Name of person who created the HDF5 version of the model.
* **creator_institution** *(string)* Institution/organization of model creator.
* **creator_email** *(string)* Email of model creator.
* **acknowledgements** *(string)* acknowledgements for the model.
* **authors** *(array of strings)* Model authors.
* **references** *(array of strings)* Publications describing the model.
* **doi** *(string)* Digital Object Identifier for model.
* **version** *(string)* Version of model.

### Model Data

* **data_values** *(array of strings)* Names of values in model grids.
* **data_units** *(array of strings)* Units of values in model grids.

### Model Storage

* **crs** *(string)* Coordinate reference system (CRS) in Proj.4 format, WKT, or EPSG code.
* **origin_x** *(float)* X coordinate of origin of model in CRS coordinates.
* **origin_y** *(float)* Y coordinate of origin of model in CRS coordinates.
* **y_azimuth** *(float)* Azimuth (degrees clockwise from north) of y axis.
* **dim_x** *(float)* Dimension of model in local (rotated) x direction in units of CRS coordinates.
* **dim_y** *(float)* Dimension of model in local (rotated) y direction in units of CRS coordinates.
* **dim_z** *(float)* Dimension of model in local z direction in units of CRS coordinates.

## Surface Metadata

* **resolution_horiz** *(float)* Horizontal resolution in units of CRS coordinates.

## Block Metadata

* **resolution_horiz** *(float)* Horizontal resolution in units of CRS coordinates.
* **resolution_vert** *(float)* Vertical resolution in units of CRS coordinates.
* **z_top** *(float)* Z coordinate of top of block in topological space.
