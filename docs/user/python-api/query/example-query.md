# Query Example

We provide a complete example Python script for querying the model with comments describing each step.
The code can be run in the `tests` directory of the GeoModelGrids source tree.

```{code-block} python
# We use the `numpy` and `geomodelgrids` Python modules, so we must import them before use.
import numpy
import geomodelgrids

# List of models to query (in order of precedence).
# Multiple models can be queried and they will be accessed in the order given.
# Once values are found in one of the models, the rest of the models are skipped.
MODELS = (
        "data/one-block-topo.h5",
        "data/three-blocks-flat.h5",
)

# Values we want returned in queries (in order).
VALUES = ("two", "one")

# Coordinate system that is used for the points passed to the query function.
#
# The coordinate system is specified using a string corresponding to any
# coordinate reference system (CRS) recognized by the [Proj library](https://proj.org).
# This includes EPGS codes, Proj parameters, and Well-Known Text (WKT).
# In this case, we specify the coordinates as longitude, latitude, elevation
# in the WGS horizontal datum, which corresponds to `EPSG:4326`.
# Elevation is meters above the WGS84 ellipsoid.
CRS = "EPSG:4326"

# We create the query object and initialize it with the query parameters.
query = geomodelgrids.Query()
query.initialize(MODELS, VALUES, CRS)

# Coordinates of points for query (latitude, longitude, elevation)
# as a NumPy array with shape [numPoints, 3].
# The coordinate system is the one specified by the `CRS` variable.
points = numpy.array([
        [37.455, -121.941, 0.0],
        [37.479, -121.734, -5.0e+3],
        [37.381, -121.581, -3.0e+3],
        [37.283, -121.959, -1.5e+3],
        [37.262, -121.684, -4.0e+3],
])

(values, status) = query.query(points)

# Check that we have valid status (=0) for all points.
assert numpy.sum(status) == 0
```
