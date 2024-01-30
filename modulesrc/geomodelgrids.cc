#include "pybind11/pybind11.h"
namespace py = pybind11;

void init_model(py::module_ &);

void init_modelinfo(py::module_ &);

void init_query(py::module_ &);

void init_errorhandler(py::module_ &);

PYBIND11_MODULE(_geomodelgrids, m) {
    init_model(m);
    init_modelinfo(m);
    init_query(m);
    init_errorhandler(m);
}
