# Storage Layout

## Model Representation

We map the physical space of the model domain bounded by on the top by
topography (or a flat surface) into topological space that has uniform
regular grids.

<figure>
  <img src="figs/gridmapping.png" alt="Diagram of physical and topological grids">
</figure>


## HDF5 Storage Scheme

The model is stored in an HDF5 file. The elevation of the ground
surface (topography) is stored in the `topography` dataset if it is not
a flat surface at sea level. The grids are stored in the `blocks`
group. Attributes are included at the root level and for each data set
in order to yield a self-describing model. That is, no additional
metadata is needed to define the model.

<figure>
  <img src="figs/hdf5layout.png" alt="Diagram of HDF5 layout scheme">
</figure>
