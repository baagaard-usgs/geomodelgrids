# Example using C++ API for querying models

See [examples/cxx-api/query.c](https://github.com/baagaard-usgs/geomodelgrids/blob/master/examples/c-api/query.c) for the complete source code associated with this example.

We first show a complete example in a single code block and then discuss the individual pieces.

## Source code

In this example, we hardwire the parameters for convenience. See the [C++ query application](https://github.com/baagaard-usgs/geomodelgrids/blob/master/libsrc/geomodelgrids/apps/Query.cc) for a more sophisticated interface.

```{code-block} c++
#include "geomodelgrids/serial/Query.hh"
#include "geomodelgrids/utils/ErrorHandler.hh"
#include "geomodelgrids/utils/constants.hh"

int main(int argc, char* argv[]) {
    // Models to query.
    static const std::vector<std::string>& filenames{
        "../../tests/data/one-block-topo.h5",
        "../../tests/data/three-blocks-flat.h5",
    };

    // Values and order to be returned in queries.
    static const std::vector<std::string>& valueNames{ "two", "one" };
    static const size_t numValues = valueNames.size();

    // Coordinate reference system of points passed to queries.
    //
    // The string can be in the form of EPSG:XXXX, WKT, or Proj
    // parameters. In this case, we will specify the coordinates in
    // latitude, longitude, elevation in the WGS84 horizontal
    // datum. The elevation is with respect to the WGS84 ellipsoid.
    static const char* const crs = "EPSG:4326";
    static const size_t spaceDim = 3;
	
    // Create and initialize serial query object using the parameters
    // stored in local variables.
    geomodelgrids::serial::Query query;
    query.initialize(filenames, valueNames, crs);

    // Log warnings and errors to "error.log".
    geomodelgrids::utils::ErrorHandler& errorHandler = query.getErrorHandler();
    errorHandler.setLogFilename("error.log");

    // Coordinates of points for query (latitude, longitude, elevation).
    static const size_t numPoints = 5;
    static const double points[5*3] = {
        37.455, -121.941, 0.0,
        37.479, -121.734, -5.0e+3,
        37.381, -121.581, -3.0e+3,
        37.283, -121.959, -1.5e+3,
        37.262, -121.684, -4.0e+3,
    };

    // Query for values at points. We must preallocate the array holding the values.
    double values[numValues];
    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double latitude = points[iPt*spaceDim+0];
        const double longitude = points[iPt*spaceDim+1];
        const double elevation = points[iPt*spaceDim+2];
        const int err = query.query(values, latitude, longitude, elevation);

        // Query for elevation of ground surface.
        const double groundElev = query.queryElevation(latitude, longitude);

        // Use the values returned in the query.
    }

    return 0;
}
```

### Header files

We include the header files for the query and error handler interfaces as well as defined constants.

```{code-block} c++
#include "geomodelgrids/serial/Query.hh"
#include "geomodelgrids/utils/ErrorHandler.hh"
#include "geomodelgrids/utils/constants.hh"
```

### Set query parameters

As mentioned earlier, in this example we hardwire all of the query parameters using local variables.

Set the parameters indicating which model to query
Multiple models can be queried and they will be accessed in the order given.
Once values are found in one of the models, the rest of the models are skipped.

```{code-block} c++
static const std::vector<std::string>& filenames{
   "../../tests/data/one-block-topo.h5",
   "../../tests/data/three-blocks-flat.h5",
};
```

Set the parameters for the values to be returned in queries. The order specified is the order in which they will be returned in queries.

```{code-block} c++
static const std::vector<std::string>& valueNames{ "two", "one" };
static const size_t numValues = valueNames.size();
```

Set the coordinate system that is used for the points passed to the query function.
The coordinate system is specified using a string corresponding to any coordinate reference system (CRS) recognized by the [Proj library](https://proj.org).
This includes EPGS codes, Proj parameters, and Well-Known Text (WKT).
In this case, we specify the coordinates as longitude, latitude, elevation in the WGS horizontal datum, which corresponds to `EPSG:4326`.
Elevation is meters above the WGS84 ellipsoid.

```{code-block} c++
static const char* const crs = "EPSG:4326";
static const size_t spaceDim = 3;
```

### Create and initialize the query

We create the query object and initialize it with the query parameters.

```{code-block} c++
geomodelgrids::serial::Query query;
query.initialize(filenames, valueNames, crs);
```

### Setup error handler

We get the error handler from the query object and set the name of the log file where warnings and errors will be written.

```{code-block} c++
geomodelgrids::utils::ErrorHandler& errorHandler = query.getErrorHandler();
errorHandler.setLogFilename("error.log");
```

### Set points for query

In this example, we hardwire the query points using a local variable.
The coordinate system is the one specified by the `crs` variable.

```{code-block} c
static const size_t numPoints = 5;
static const double points[5*3] = {
   37.455, -121.941, 0.0,
   37.479, -121.734, -5.0e+3,
   37.381, -121.581, -3.0e+3,
   37.283, -121.959, -1.5e+3,
   37.262, -121.684, -4.0e+3,
};
```

### Query model

We query the model looping over the points.
We must preallocate the array holding the values returned in the queries.
We do not do anything with the values returned as indicated by the comment at the end of the `for` loop.

```{code-block} c++
double values[numValues];
for (size_t iPt = 0; iPt < numPoints; ++iPt) {
    const double latitude = points[iPt*spaceDim+0];
    const double longitude = points[iPt*spaceDim+1];
    const double elevation = points[iPt*spaceDim+2];
    const int err = query.query(values, latitude, longitude, elevation);

    // Query for elevation of ground surface.
    const double groundElev = query.queryElevation(latitude, longitude);

    // Use the values returned in the query.
}
```
