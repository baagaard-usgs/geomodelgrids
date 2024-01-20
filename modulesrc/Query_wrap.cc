#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
namespace py = pybind11;

#include "geomodelgrids/serial/Query.hh"
#include "geomodelgrids/utils/ErrorHandler.hh"
#include "geomodelgrids/utils/constants.hh"

namespace geomodelgrids {
    class PyQuery;
}

class geomodelgrids::PyQuery : public geomodelgrids::serial::Query {
public:

    inline
    PyQuery(void) {}


    inline
    ~PyQuery(void) {}


    inline
    py::array_t<double> query_top_elevation(py::array_t<double, py::array::c_style | py::array::forcecast> pointsArray) {
        py::buffer_info pointsInfo = pointsArray.request();
        const double* const points = static_cast<const double*>(pointsInfo.ptr);

        if ((pointsInfo.ndim != 2) || (pointsInfo.shape[1] != 2)) {
            throw std::runtime_error("Points must be an array with shape [numPoints, 2].");
        }
        const size_t numPoints = pointsInfo.shape[0];
        const size_t spaceDim = pointsInfo.shape[1];

        const size_t resultSize = pointsInfo.shape[0];
        py::array_t<double> resultArray(resultSize);
        py::buffer_info resultInfo = resultArray.request();

        double* result = static_cast<double*>(resultInfo.ptr);
        for (size_t iPoint = 0; iPoint < numPoints; ++iPoint) {
            const double* x = &points[iPoint*spaceDim];
            result[iPoint] = geomodelgrids::serial::Query::queryTopElevation(x[0], x[1]);
        }

        return resultArray;
    }

    inline
    py::array_t<double> query_topobathy_elevation(py::array_t<double, py::array::c_style | py::array::forcecast> pointsArray) {
        py::buffer_info pointsInfo = pointsArray.request();
        const double* const points = static_cast<const double*>(pointsInfo.ptr);

        if ((pointsInfo.ndim != 2) || (pointsInfo.shape[1] != 2)) {
            throw std::runtime_error("Points must be an array with shape [numPoints, 2].");
        }
        const size_t numPoints = pointsInfo.shape[0];
        const size_t spaceDim = pointsInfo.shape[1];

        const size_t resultSize = pointsInfo.shape[0];
        py::array_t<double> resultArray(resultSize);
        py::buffer_info resultInfo = resultArray.request();

        double* result = static_cast<double*>(resultInfo.ptr);
        for (size_t iPoint = 0; iPoint < numPoints; ++iPoint) {
            const double* x = &points[iPoint*spaceDim];
            result[iPoint] = geomodelgrids::serial::Query::queryTopoBathyElevation(x[0], x[1]);
        }

        return resultArray;
    }

    inline
    std::tuple < py::array_t<double>, py::array_t<int> > query(py::array_t<double, py::array::c_style | py::array::forcecast> pointsArray) {
        py::buffer_info pointsInfo = pointsArray.request();
        const double* const points = static_cast<const double*>(pointsInfo.ptr);

        if ((pointsInfo.ndim != 2) || (pointsInfo.shape[1] != 3)) {
            throw std::runtime_error("Points must be an array with shape [numPoints, 3].");
        }
        const size_t numPoints = pointsInfo.shape[0];
        const size_t spaceDim = pointsInfo.shape[1];
        const size_t numValues = geomodelgrids::serial::Query::getValueNames().size();

        py::array_t<double> resultArray({numPoints, numValues});
        py::buffer_info resultInfo = resultArray.request();
        double* result = static_cast<double*>(resultInfo.ptr);

        py::array_t<int> errorArray(numPoints);
        py::buffer_info errorInfo = errorArray.request();
        int* error = static_cast<int*>(errorInfo.ptr);

        for (size_t iPoint = 0; iPoint < numPoints; ++iPoint) {
            const double* x = &points[iPoint*spaceDim];
            const int errorCode = geomodelgrids::serial::Query::query(&result[iPoint*numValues], x[0], x[1], x[2]);
            error[iPoint] = errorCode;
            if (errorCode == geomodelgrids::utils::ErrorHandler::ERROR) {
                throw std::runtime_error(geomodelgrids::serial::Query::getErrorHandler()->getMessage());
            }
        }

        return std::make_tuple(resultArray, errorArray);
    }

};

void
init_query(py::module_& m) {
    py::class_<geomodelgrids::PyQuery> query(m, "Query");

    py::enum_<geomodelgrids::PyQuery::SquashingEnum>(query, "SquashingEnum")
    .value("SQUASH_NONE", geomodelgrids::PyQuery::SQUASH_NONE)
    .value("SQUASH_TOP_SURFACE", geomodelgrids::PyQuery::SQUASH_TOP_SURFACE)
    .value("SQUASH_TOPOGRAPHY_BATHYMETRY", geomodelgrids::PyQuery::SQUASH_TOPOGRAPHY_BATHYMETRY)
    .export_values();

    query
    .def(py::init<>())

    .def_readonly_static("NODATA_VALUE", &geomodelgrids::NODATA_VALUE)

    .def("get_error_handler", &geomodelgrids::PyQuery::getErrorHandler)

    .def("initialize", &geomodelgrids::PyQuery::initialize,
         "Perform initialization required to query the models.",
         py::arg("models"),
         py::arg("values"),
         py::arg("input_crs"))

    .def("finalize", &geomodelgrids::PyQuery::finalize,
         "Clean up after querying the models.")

    .def("set_squash_min_elev", &geomodelgrids::PyQuery::setSquashMinElev,
         "Turn on squashing using the top surface and set the minimum elevation for squashing.",
         py::arg("min_elev"))

    .def("set_squashing", &geomodelgrids::PyQuery::setSquashing,
         "Set type of squashing.",
         py::arg("squash_type"))

    .def("query_top_elevation", &geomodelgrids::PyQuery::query_top_elevation,
         "Query for elevation (m) of top of model at points using bilinear interpolation.",
         py::arg("points")
         )

    .def("query_topobathy_elevation", &geomodelgrids::PyQuery::query_topobathy_elevation,
         "Query for elevation (m) of topography/bathymetry of model at points using bilinear interpolation.",
         py::arg("points")
         )

    .def("query", &geomodelgrids::PyQuery::query,
         "Query for model values at points using bilinear interpolation.",
         py::arg("points"))

    ;
}
