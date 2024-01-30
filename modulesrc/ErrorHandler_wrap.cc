#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
namespace py = pybind11;

#include "geomodelgrids/utils/ErrorHandler.hh"

void
init_errorhandler(py::module_& m) {
    py::class_<geomodelgrids::utils::ErrorHandler, std::shared_ptr<geomodelgrids::utils::ErrorHandler> > errorHandler(m, "ErrorHandler");

    py::enum_<geomodelgrids::utils::ErrorHandler::StatusEnum>(errorHandler, "StatusEnum")
    .value("OK", geomodelgrids::utils::ErrorHandler::OK)
    .value("WARNING", geomodelgrids::utils::ErrorHandler::WARNING)
    .value("ERROR", geomodelgrids::utils::ErrorHandler::ERROR)
    .export_values();

    errorHandler
    .def(py::init<>())

    .def("reset_status", &geomodelgrids::utils::ErrorHandler::resetStatus,
         "Reset error status and clear any error message.")

    .def("get_status", &geomodelgrids::utils::ErrorHandler::getStatus,
         "Get error status.")

    ;
}
