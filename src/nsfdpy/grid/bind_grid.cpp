/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <nsfd/grid/grid.hpp>

namespace py = pybind11;

namespace {

nsfd::grid::Grid *__init__(const py::dict &geom_data, double xfrac = 1,
                           double yfrac = 1) {
  return new nsfd::grid::Grid(geom_data["xlength"].cast<double>(),
                              geom_data["imax"].cast<size_t>(),
                              geom_data["ylength"].cast<double>(),
                              geom_data["jmax"].cast<size_t>(), xfrac, yfrac);
}

py::tuple meshgrid(const nsfd::grid::Grid &self) {
  py::array_t<double> X({self.x.max + 2, self.y.max + 2});
  py::array_t<double> Y({self.x.max + 2, self.y.max + 2});
  auto X_buf = X.mutable_unchecked<2>();
  auto Y_buf = Y.mutable_unchecked<2>();
  for (size_t i = 0; i < self.x.max + 2; ++i) {
    for (size_t j = 0; j < self.y.max + 2; ++j) {
      X_buf(i, j) = static_cast<double>(self.x[i]);
      Y_buf(i, j) = static_cast<double>(self.y[j]);
    }
  }
  return py::make_tuple(X, Y);
}
}  // namespace

namespace nsfdpy {
namespace grid {

void bindGrid(py::module_ &m) {
  py::class_<nsfd::grid::Grid>(m, "Grid")
      .def(py::init(&__init__), py::arg("geom_data"), py::arg("xfrac") = 1,
           py::arg("yfrac") = 1)
      .def_readonly("x", &nsfd::grid::Grid::x)
      .def_readonly("y", &nsfd::grid::Grid::y)
      .def("meshgrid", meshgrid);
}
}  // namespace grid
}  // namespace nsfdpy
