// Application illustrating how to use the C++ API to query models.

#include "geomodelgrids/serial/Query.hh"
#include "geomodelgrids/utils/ErrorHandler.hh"
#include "geomodelgrids/utils/constants.hh"

#include <stddef.h>
#include <iostream>
#include <cmath>
#include <cassert>

int
main(int argc,
     char* argv[]) {
    // In this example, we hardwire the parameters for
    // convenience. See the C++ query application (bin/query.cc) for a
    // more sophisticated interface.

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
    static const size_t numPoints = 16;
    static const double points[16*3] = {
        // Points in one-block-topo model
        37.455, -121.941, 0.0,
        37.479, -121.734, -5.0e+3,
        37.381, -121.581, -3.0e+3,
        37.283, -121.959, -1.5e+3,
        37.262, -121.684, -4.0e+3,

        // Points in three-blocks-flat model
        35.3, -118.2, 0.0,
        35.5, -117.9, -45.0e+3,
        35.0, -118.1, -3.0e+3,
        35.1, -117.7, -15.0e+3,
        34.7, -117.9, -25.0e+3,
        34.7, -117.5, -8.4e+3,

        // Points not in either model domain
        34.7, -117.8, 1.0e+4,
        35.0, -117.6, -45.1e+3,
        34.3, -117.8, -3.0e+3,
        35.0, -113.0, -15.0e+3,
        42.0, -117.8, -25.0e+3,
    };

    // Expected return values and query values.
    static const int errE[16] = {
        // Points in one-block-topography model
        0, 0, 0, 0, 0,

        // Points in three-blocks-flat model
        0, 0, 0, 0, 0, 0,

        /* Points not in either model domain */
        1, 1, 1, 1, 1,
    };
    static const double valuesE[16*2] = {
        // Points in one-block-topo model
        4121.65, 7662.52,
        -19846.6, 14510.1,
        -9158.54, 29696.3,
        45038.7, 26938.5,
        26595.3, 39942.5,

        // Points in three-blocks-flat model
        -56937.1, 62087.1,
        -3564.86, 121682,
        -44676.9, 42041.4,
        37087.2, 82235.8,
        -17131.2, 37823.2,
        71156.9, 54662,

        // Points not in either model domain
        geomodelgrids::NODATA_VALUE, geomodelgrids::NODATA_VALUE,
        geomodelgrids::NODATA_VALUE, geomodelgrids::NODATA_VALUE,
        geomodelgrids::NODATA_VALUE, geomodelgrids::NODATA_VALUE,
        geomodelgrids::NODATA_VALUE, geomodelgrids::NODATA_VALUE,
        geomodelgrids::NODATA_VALUE, geomodelgrids::NODATA_VALUE,
    };
    static const double surfaceElevE[16] = {
        // Elevations for one-block-topo points
        150.046,
        149.775,
        150.023,
        150.463614,
        150.521716,

        // Elevations for three-blocks-flat model
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,

        // Elevations for points outside model domain
        0.0, // latitude/longitude is in model domain
        0.0, // latitude/longitude is in model domain
        geomodelgrids::NODATA_VALUE,
        geomodelgrids::NODATA_VALUE,
        geomodelgrids::NODATA_VALUE,
    };

    // Query for values at points. We must preallocate the array holding the values.
    double values[numValues];
    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double latitude = points[iPt*spaceDim+0];
        const double longitude = points[iPt*spaceDim+1];
        const double elevation = points[iPt*spaceDim+2];
        const int err = query.query(values, latitude, longitude, elevation);

        // Query for elevation of top surface.
        const double surfaceElev = query.queryTopElevation(latitude, longitude);

        // Use the values returned in the query. In this case we check the values against the expected ones.
        //
        // We first check the return value and then the query values.

        if (errE[iPt] != err) {
            printf("Expected a return value of %d for point (%g, %g, %g) but got a value of %d.\n",
                   errE[iPt], latitude, longitude, elevation, err);
        } // if

        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            const double tolerance = 1.0e-4;
            const double relativeDiff = (values[iValue] - valuesE[iPt*numValues+iValue]) / valuesE[iPt*numValues+iValue];
            if (fabs(relativeDiff) > tolerance) {
                std::cout << "Expected value of " << valuesE[iPt*numValues+iValue] << " for '" << valueNames[iValue]
                          << "' at point (" << latitude << ", " << longitude << ", " << elevation
                          << ") but got a value of " << values[iValue] << "." << std::endl;
            } else {
                if (values[iValue] != geomodelgrids::NODATA_VALUE) {
                    std::cout << "Point (" << latitude << ", " << longitude << ", " << elevation << "), value '"
                              << valueNames[iValue] << "': " << values[iValue] << "." << std::endl;
                } else {
                    std::cout << "No data for point (" << latitude << ", " << longitude << ", " << elevation << ")."
                              << std::endl;
                } // if/else
            } // if/else
        } // for

        // Check ground surface elevation values.
        const double tolerance = 1.0e-06 * fabs(surfaceElevE[iPt]);
        if (fabs(surfaceElev-surfaceElevE[iPt]) > tolerance) {
            std::cout << "Expected ground surface elevation of " << surfaceElevE[iPt] << " at point (" << latitude
                      << ", " << longitude << ") but got a value of " << surfaceElev << "." << std::endl;
        } else {
            if (surfaceElev != geomodelgrids::NODATA_VALUE) {
                std::cout << "Point (" << latitude << ", " << longitude << "), ground surface elevation: " << surfaceElev
                          << "." << std::endl;
            } else {
                std::cout << "No ground surface elevation for point (" << latitude << ", " << longitude << ")."
                          << std::endl;
            } // if/else

        } // if/else
    } // for

    return 0;
} // main


// End of file
