#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
namespace py = pybind11;

#include "geomodelgrids/serial/Model.hh"
#include "geomodelgrids/serial/ModelInfo.hh"

namespace geomodelgrids {
    class PyModel;
}

class geomodelgrids::PyModel : public geomodelgrids::serial::Model {
public:

    inline
    PyModel(void) {}


    inline
    ~PyModel(void) {}


    inline
    std::vector<double> get_dims(void) {
        const double* dims = geomodelgrids::serial::Model::getDims();
        return std::vector<double>(dims, dims+3);
    }

    inline
    std::vector<double> get_origin(void) {
        const double* origin = geomodelgrids::serial::Model::getOrigin();
        return std::vector<double>(origin, origin+2);
    }

    inline
    py::array_t<double> contains(py::array_t<double, py::array::c_style | py::array::forcecast> pointsArray) {
        py::buffer_info pointsInfo = pointsArray.request();
        const double* const points = static_cast<const double*>(pointsInfo.ptr);

        if ((pointsInfo.ndim != 2) || (pointsInfo.shape[1] != 3)) {
            throw std::runtime_error("Points must be an array with shape [numPoints, 3].");
        }
        const size_t numPoints = pointsInfo.shape[0];
        const size_t spaceDim = pointsInfo.shape[1];

        const size_t resultSize = pointsInfo.shape[0];
        py::array_t<bool> resultArray(resultSize);
        py::buffer_info resultInfo = resultArray.request();

        bool* result = static_cast<bool*>(resultInfo.ptr);
        for (size_t iPoint = 0; iPoint < numPoints; ++iPoint) {
            const double* x = &points[iPoint*spaceDim];
            result[iPoint] = geomodelgrids::serial::Model::contains(x[0], x[1], x[2]);
        }

        return resultArray;
    }

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
            result[iPoint] = geomodelgrids::serial::Model::queryTopElevation(x[0], x[1]);
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
            result[iPoint] = geomodelgrids::serial::Model::queryTopoBathyElevation(x[0], x[1]);
        }

        return resultArray;
    }

    inline
    py::array_t<double> query(py::array_t<double, py::array::c_style | py::array::forcecast> pointsArray) {
        py::buffer_info pointsInfo = pointsArray.request();
        const double* const points = static_cast<const double*>(pointsInfo.ptr);

        if ((pointsInfo.ndim != 2) || (pointsInfo.shape[1] != 3)) {
            throw std::runtime_error("Points must be an array with shape [numPoints, 3].");
        }
        const size_t numPoints = pointsInfo.shape[0];
        const size_t spaceDim = pointsInfo.shape[1];
        const size_t numValues = Model::getValueNames().size();

        py::array_t<double> resultArray({numPoints, numValues});
        py::buffer_info resultInfo = resultArray.request();

        double* result = static_cast<double*>(resultInfo.ptr);
        for (size_t iPoint = 0; iPoint < numPoints; ++iPoint) {
            const double* x = &points[iPoint*spaceDim];
            const double* values = geomodelgrids::serial::Model::query(x[0], x[1], x[2]);
            for (size_t iValue = 0; iValue < numValues; ++iValue) {
                result[iPoint*numValues+iValue] = values[iValue];
            }
        }

        return resultArray;
    }

};

void
init_model(py::module_& m) {
    py::class_<geomodelgrids::PyModel> model(m, "Model");

    py::enum_<geomodelgrids::PyModel::ModelMode>(model, "ModelMode")
    .value("READ", geomodelgrids::PyModel::READ)
    .value("READ_WRITE", geomodelgrids::PyModel::READ_WRITE)
    .value("READ_WRITE_TRUNCATE", geomodelgrids::PyModel::READ_WRITE_TRUNCATE)
    .export_values();

    py::enum_<geomodelgrids::PyModel::DataLayout>(model, "DataLayout")
    .value("VERTEX", geomodelgrids::PyModel::VERTEX)
    .value("CELL", geomodelgrids::PyModel::CELL)
    .export_values();

    model
    .def(py::init<>())

    .def("set_input_crs", &geomodelgrids::PyModel::setInputCRS)

    .def("open", &geomodelgrids::PyModel::open,
         "Open model file.",
         py::arg("filename"),
         py::arg("mode"))

    .def("close", &geomodelgrids::PyModel::close,
         "Close model file.")

    .def("load_metadata", &geomodelgrids::PyModel::loadMetadata,
         "Load metadata.")

    .def("initialize", &geomodelgrids::PyModel::initialize,
         "Initialize.")

    .def_property_readonly("value_names", &geomodelgrids::PyModel::getValueNames,
                           "Get names of values in model.")

    .def_property_readonly("value_units", &geomodelgrids::PyModel::getValueUnits,
                           "Get names of units for values in model.")

    .def_property_readonly("data_layout", &geomodelgrids::PyModel::getDataLayout,
                           "Get data layout.")

    .def_property_readonly("dims", &geomodelgrids::PyModel::get_dims,
                           "Get model dimensions in meters.")

    .def_property_readonly("origin", &geomodelgrids::PyModel::get_origin,
                           "Get coordinates of model origin in the model CRS.")

    .def_property_readonly("y_azimuth", &geomodelgrids::PyModel::getYAzimuth,
                           "Get azimuth (degrees) of y coordinate axis.")

    .def_property_readonly("crs", &geomodelgrids::PyModel::getCRSString,
                           "Get coordinate system of model as string.")

    .def("get_info", &geomodelgrids::PyModel::getInfo,
         "Get information describing model.")

    .def("contains", &geomodelgrids::PyModel::contains,
         "Does model contain given point?",
         py::arg("points")
         )

    .def("query_top_elevation", &geomodelgrids::PyModel::query_top_elevation,
         "Query for elevation (m) of top of model at points using bilinear interpolation.",
         py::arg("points")
         )

    .def("query_topobathy_elevation", &geomodelgrids::PyModel::query_topobathy_elevation,
         "Query for elevation (m) of topography/bathymetry of model at points using bilinear interpolation.",
         py::arg("points")
         )

    .def("query", &geomodelgrids::PyModel::query,
         "Query for model values at points using bilinear interpolation.",
         py::arg("points"))

    ;
}


/* Not implemented
 *
 * const geomodelgrids::serial::Surface* getTopSurface(void) const;
 * const geomodelgrids::serial::Surface* getTopoBathy(void) const;
 * const std::vector<geomodelgrids::serial::Block*>& getBlocks(void) const;
 */
