(sec-user-topography)=
# Topography

In cases where you have your own topography model, which may or may not match the one provided in a GeoModelGrid model, we recommend transforming your vertical coordinates into the GeoModelGrids model space.
For example, to keep the same relative position with repsect to the model top and bottom, the $z$ coordinate passed to the GeoModelGrids query function, $z_query$, would be

```{math}
z_\mathit{query} = z_\mathit{bot} + \frac{z_\mathit{user}-z_\mathit{bot}}{z_\mathit{top\_user} - z_\mathit{bot}}(z_\mathit{top} - z_\mathit{bot}),
```

where $z_\mathit{top}$ is the elevation of the top of the GeoModelGrids model (computed using `queryTopElevation()`), $z_\mathit{bot}$ is the elevation of the bottom of the GeoModelGrids model, $z_\mathit{top\_user}$ is the elevation from your topography model, and $z_\mathit{user}$ is the elevation you want in your query.
