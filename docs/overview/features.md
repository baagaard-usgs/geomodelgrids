# Features

* Store models in any georeferenced coordinate system composed of an arbitrary number of blocks of uniform resolution.

* Model domains need not be aligned with the x and y axes of the geographic projection.

* If the model includes topography, the blocks will be warped to conform to the ground surface. Queries for applications requiring a flat top surface can use "squashing" relative to either the top surface of the of the model or topography/bathymetry (if provided).

* In serial queries (intended for use on laptops and desktops), only a portion of the model is loaded into memory, thereby allowing queries of models that are much larger than the available memory.

* In addition to discretization information, the self-describing format includes description of the names of the values and their units, and coordinate system information.
