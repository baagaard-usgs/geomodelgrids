#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
namespace py = pybind11;

#include "geomodelgrids/serial/ModelInfo.hh"

void
init_modelinfo(py::module_& m) {
    py::class_<geomodelgrids::serial::ModelInfo, std::shared_ptr<geomodelgrids::serial::ModelInfo> >(m, "ModelInfo")
    .def(py::init<>())
    .def_property_readonly("title", &geomodelgrids::serial::ModelInfo::getTitle)
    .def_property_readonly("id", &geomodelgrids::serial::ModelInfo::getId)
    .def_property_readonly("description", &geomodelgrids::serial::ModelInfo::getDescription)
    .def_property_readonly("keywords", &geomodelgrids::serial::ModelInfo::getKeywords)
    .def_property_readonly("history", &geomodelgrids::serial::ModelInfo::getHistory)
    .def_property_readonly("comment", &geomodelgrids::serial::ModelInfo::getComment)
    .def_property_readonly("creator_name", &geomodelgrids::serial::ModelInfo::getCreatorName)
    .def_property_readonly("creator_institution", &geomodelgrids::serial::ModelInfo::getCreatorInstitution)
    .def_property_readonly("creator_email", &geomodelgrids::serial::ModelInfo::getCreatorEmail)
    .def_property_readonly("acknowledgement", &geomodelgrids::serial::ModelInfo::getAcknowledgement)
    .def_property_readonly("authors", &geomodelgrids::serial::ModelInfo::getAuthors)
    .def_property_readonly("references", &geomodelgrids::serial::ModelInfo::getReferences)
    .def_property_readonly("repository_name", &geomodelgrids::serial::ModelInfo::getRepositoryName)
    .def_property_readonly("repository_url", &geomodelgrids::serial::ModelInfo::getRepositoryURL)
    .def_property_readonly("repository_doi", &geomodelgrids::serial::ModelInfo::getRepositoryDOI)
    .def_property_readonly("version", &geomodelgrids::serial::ModelInfo::getVersion)
    .def_property_readonly("license", &geomodelgrids::serial::ModelInfo::getLicense)
    .def_property_readonly("auxiliary", &geomodelgrids::serial::ModelInfo::getAuxiliary)

    ;
}
