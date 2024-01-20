#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
namespace py = pybind11;

#include "geomodelgrids/serial/ModelInfo.hh"

void
init_modelinfo(py::module_& m) {
    py::class_<geomodelgrids::serial::ModelInfo, std::shared_ptr<geomodelgrids::serial::ModelInfo> >(m, "ModelInfo")
    .def(py::init<>())
    .def("get_title", &geomodelgrids::serial::ModelInfo::getTitle)
    .def("get_id", &geomodelgrids::serial::ModelInfo::getId)
    .def("get_description", &geomodelgrids::serial::ModelInfo::getDescription)
    .def("get_keywords", &geomodelgrids::serial::ModelInfo::getKeywords)
    .def("get_history", &geomodelgrids::serial::ModelInfo::getHistory)
    .def("get_comment", &geomodelgrids::serial::ModelInfo::getComment)
    .def("get_creator_name", &geomodelgrids::serial::ModelInfo::getCreatorName)
    .def("get_creator_institution", &geomodelgrids::serial::ModelInfo::getCreatorInstitution)
    .def("get_creator_email", &geomodelgrids::serial::ModelInfo::getCreatorEmail)
    .def("get_acknowledgement", &geomodelgrids::serial::ModelInfo::getAcknowledgement)
    .def("get_authors", &geomodelgrids::serial::ModelInfo::getAuthors)
    .def("get_references", &geomodelgrids::serial::ModelInfo::getReferences)
    .def("get_repository_name", &geomodelgrids::serial::ModelInfo::getRepositoryName)
    .def("get_repository_url", &geomodelgrids::serial::ModelInfo::getRepositoryURL)
    .def("get_repository_doi", &geomodelgrids::serial::ModelInfo::getRepositoryDOI)
    .def("get_version", &geomodelgrids::serial::ModelInfo::getVersion)
    .def("get_license", &geomodelgrids::serial::ModelInfo::getLicense)
    .def("get_auxiliary", &geomodelgrids::serial::ModelInfo::getAuxiliary)

    ;
}
