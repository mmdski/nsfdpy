/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/config.hpp>
#include <nsfd/grid/staggered_grid.hpp>

namespace py = pybind11;

namespace {
nsfd::grid::StaggeredGrid *__init__(const py::dict &geom_data) {
  return new nsfd::grid::StaggeredGrid(
      geom_data["xlength"].cast<double>(), geom_data["imax"].cast<size_t>(),
      geom_data["ylength"].cast<double>(), geom_data["jmax"].cast<size_t>());
}

py::dict geom_data(const nsfd::grid::StaggeredGrid &self) {
  nsfd::grid::GeometryData geom_data = self.geom_data();
  py::dict py_geom_data;
  py_geom_data["xlength"] = geom_data.xlength;
  py_geom_data["ylength"] = geom_data.ylength;
  py_geom_data["imax"] = geom_data.imax;
  py_geom_data["jmax"] = geom_data.jmax;
  return py_geom_data;
}

}  // namespace

namespace nsfdpy {
namespace grid {

void bindStaggeredGrid(py::module_ &m) {
  py::class_<nsfd::grid::StaggeredGrid>(m, "StaggeredGrid")
      .def(py::init(&__init__))
      .def(py::init<nsfd::config::Geometry &>())
      .def_readonly("p", &nsfd::grid::StaggeredGrid::p)
      .def_readonly("u", &nsfd::grid::StaggeredGrid::u)
      .def_readonly("v", &nsfd::grid::StaggeredGrid::v)
      .def_property_readonly("delx", &nsfd::grid::StaggeredGrid::delx)
      .def_property_readonly("dely", &nsfd::grid::StaggeredGrid::dely)
      .def_property_readonly("imax", &nsfd::grid::StaggeredGrid::imax)
      .def_property_readonly("jmax", &nsfd::grid::StaggeredGrid::jmax)
      .def_property_readonly("geom_data", &geom_data);
}
}  // namespace grid
}  // namespace nsfdpy
