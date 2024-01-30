(sec-user-squashing)=
# Squashing

Some applications ignore topography and assume a flat Earth model.
In such cases, you will likely want to query the model at elevations relative to the topographic surface.
You can query for elevation and adjust your input values accordingly, or you can use squashing to have the top surface automatically squashed and stretched so that it is at z=0.
The model is squashed and stretched over the elevation range from the minimum squashing elevation up to the top surface.
Either the top surface or the topography/bathymetry surface can be used as the reference surface for squashing.

:::{important}
Squashing distorts the geometry of the Earth structure above the minimum squashing elevation.
The amount of distortion depends on the elevation of the top surface and the minimum squashing elevation.
The distortion in the vertical direction is uniform over the entire depth range of a single vertical profile.
The distortion in the horizontal direction depends on the gradient in the elevation of the reference surface with a larger gradient leading to larger distortion in the horizontal direction.
:::

:::{tip}
To minimize distortion in the horizontal and vertical directions, you will want to use a minimum squashing elevation that is equal to the bottom elevation of the model.
:::



:::{figure-md} fig-squashing
<img src="figs/squashing.*" alt="Diagram of physical model and squashed model." width="100%"/>

Diagram showing the model in physical space (left) and squashed (right).
Above the minimum squashing elevation of -15, we squash and stretch the model so that the top surface is at z=0.
Below the minimum squashing elevation, the model is kept fixed.
:::

## Mapping between physical space and squashed model

The mappings between the physical space and squashed model are given by:

```{math}
\begin{align}
z_\mathit{physical} &= z_\mathit{top} + z_\mathit{squashed} \frac{z_\mathit{minsquash}-z_\mathit{top}}{z_\mathit{minsquash}} \text{, and}\\
%
z_\mathit{squashed} &= \frac{z_\mathit{minsquash}}{z_\mathit{minsquash}-z_\mathit{top}} (z_\mathit{physical} - z_\mathit{top}).
\end{align}
```

## Example

For an elevation of the top surface of 100 m and a minimum squashing elevation of -10 km, the following two queries are equivalent.

```{code-block} bash
---
caption: Query of model in physical space
---
geomodelgrids_query \
--models=my_model_with_topography.h5 \
--points=points_topo.in \
--output=points_topo.out

# Input: points_topo.in
37.455  -121.941   100.0
37.455  -121.941  -910.0
37.455  -121.941   -10.0e+3
37.455  -121.941   -20.0e+3
```

```{code-block} bash
---
caption: Query of model using squashing using the default surface (top surface) as the reference.
---
geomodelgrids_query \
--models=my_model_with_topography.h5 \
--points=points_flat.in \
--output=points_flat.out \
--min-squash-elev=-10.0e+3

# Input: points_flat.in
37.455  -121.941      0.0
37.455  -121.941     -1.0e+3
37.455  -121.941    -10.0e+3
37.455  -121.941    -20.0e+3
```
