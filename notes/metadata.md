# Domain
* num_blocks Number of grid blocks

# Coordinate system
* projection Proj.4 name for geographic projection.
* projection_parameters Proj.4 parameters for geographic projection.
* x_origin X coordinate of origin of local grid in projected coordinates.
* y_origin Y coordinate of origin of local grid in projected coordinates.
* y_azimuth Azimuth of y-axis in degrees.

# Block
* res_horiz Horizontal resolution within block.
* res_vert Vertical resolution within block.
* z_top Z coordinate of top of block.
* num_x Number of grid points along grid x-direction.
* num_y Number of grid points along grid y-direction.
* num_z Number of grid points along grid z-direction.

Within a block we generally work from shallow depths to greater
depths, so we employ a left-handed block coordinate system with the
z-axis pointed down.

## Transform from block coordinates to projected coordinates
```
x_proj = x_origin + x_grid*cos(y_azmith) + y_grid*sin(y_azimuth)
y_proj = y_origin - x_grid*sin(y_azmith) + y_grid*cos(y_azimuth)
z_proj = z_top - z_grid
```

## Transform from projected coordinates to block coordinates
```
x_grid = (x_proj-x_origin)*cos(y_azimuth) - (y_proj-y_origin)*sin(y_azimuth)
y_grid = (x_proj-x_origin)*sin(y_azimuth) + (y_proj-y_origin)*cos(y_azimuth)
z_grid = z_top - z_proj
```

## Block indices from grid coordiantes
```
index_x = x_grid / res_horiz
index_y = y_grid / res_horiz
index_z = z_grid / res_vert
```

Grids are stored in y, x, z order so consecutive points vary in z,
then x, and then y.
```
index = index_y*num_y*numZ + index_x*num_z + index_z
```
