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
z_\mathit{physical} &= z_\mathit{surf} + z_\mathit{squashed} \frac{z_\mathit{minsquash}-z_\mathit{surf}}{z_\mathit{minsquash}} \text{, and}\\
%
z_\mathit{squashed} &= \frac{z_\mathit{minsquash}}{z_\mathit{minsquash}-z_\mathit{surf}} (z_\mathit{physical} - z_\mathit{surf}).
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

## Algorithm for using squashing with topography/bathymetry

When using squashing with the topography/bathymetry surface, queries just below the surface may return `NODATA_VALUES` for Vs values.
This happens when the query point lies in a grid cell that has some values above the topography/bathymetry surface and some below; the interpolation algorithm will return `NODATA_VALUES` if any values at the vertices of a grid cell are `NODATA_VALUES`.
You can use the following algorithm to efficiently find the highest point that will return a valid value.

### Nomenclature

- `dimZ`: total height of the model
- `zPhysical`: elevation (m) in physical space
- `zSquashed`: elevation (m) in squashed space
- `zMinSquashed`: minimum elevation (m) of squashing (recommended value is `zMinSquashed=-dimZ`) 
- `zLogical`: elevation (m) in logical space (grid space)
- `zTop`: elevation (m) of the top of the model (topography and top of ocean)
- `zSurf`: elevation (m) of surface used in squashing (topography / bathymetry in this case)
- `dZ`: discretization size (m) in the vertical direction in the region being queried

### Relationship between `zSquashed` and `zLogical`

Using the relationship between zSquashed and zPhysical, 

```
zSquashed = zMinSquashed / (zMinSquashed – zSurf) * (zPhysical – zSurf)
```

and the relationship between zPhysical and zLogical,

```
zLogical = -dimZ * (zTop – zPhysical) / (zTop + dimZ)
```

we can derive relationships between zSquashed and zLogical:

```
zSquashed = zMinSquashed / (zMinSquashed – zSurf) * (zTop – zSurf + zLogical / dimZ * (zTop + dimZ))
zLogical = (zSquashed * (zMinSquashed - zSurf) / zMinSquashed + zSurf - zTop) * (dimZ / (zTop + dimZ))
```

:::{note}
If `zSurf == zTop` (top of the model) and `zMinSquashed == -dimZ`, then `zLogical == zSquashed`.
That is, the logical grid lines up with a squashed model.
However, if `zSurf` is the elevation of the topography/bathymetry surface, then `zSurf != zTop`, so `zLogical != zSquashed` even if `zMinSquashed == -dimZ`.
:::

### Pseudocode

1. Query for values using squashing and the desired elevation, for example, to get a value at the “surface”, you could use an elevation of -1.0 m (1 m below the topography/bathymetry surface). This is zSquashed.
2. If the Vs value returned is `NODATA_VALUE`, then adjust the elevation and query the model again.
    1. Compute `zLogical` from `zSquashed` using `zLogical = (zSquashed * (zMinSquashed - zSurf) / zMinSquashed + zSurf - zTop) * (dimZ / (zTop + dimZ))`
    2. Update `zLogical` to be 1 grid point lower using `zLogical = floor(zLogical/dZ)*dZ`
    3. Compute an updated `zSquashed` from `zLogical` using `zSquashed = zMinSquashed / (zMinSquashed – zSurf) * (zTop – zSurf + zLogical / dimZ * (zTop + dimZ))`
    4. Query the model using this updated `zSquashed`.
    5. While Vs is `NODATA_VALUE`, do
        1. Reduce `zLogical` by `dZ`, `zLogical -= dZ`
        2. Update `zSquashed` using `zSquashed = zMinSquashed / (zMinSquashed – zSurf) * (zTop – zSurf + zLogical / dimZ * (zTop + dimZ))`
        3. Query the mode using this update `zSquashed`
