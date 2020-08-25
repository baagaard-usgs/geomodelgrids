# Features

* Store georeferenced models composed of an arbitrary number of blocks
  of uniform resolution.
  
* Model domains need not be aligned with the x and y axes of the
  geographic projection.

* If the model includes topography, the blocks will be warped to
  conform to the ground surface.

* In serial queries (intended for use on laptops and desktops), only a
  portion of the model is loaded into memory, thereby allowing queries
  of models only limited by the total storage size.

* In addition to discretization information, the self-describing
  format includes description of the names of the values and their
  units, coordinate information.
