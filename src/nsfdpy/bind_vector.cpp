/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <format>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <tuple>

#include <nsfd/vector.hpp>

namespace py = pybind11;

namespace {

std::string __repr__(nsfd::Vector &self) {
  return "nsfdpy.Vector(" + std::format("{}", self.x) + ", " +
         std::format("{}", self.y) + ")";
}
}  // namespace

namespace nsfdpy {

void bindVector(py::module_ &m) {
  py::class_<nsfd::Vector>(m, "Vector")
      .def(py::init<double, double>(), py::arg("x") = 0, py::arg("y") = 0)
      .def(py::init<std::tuple<double, double>>())
      .def(py::init<nsfd::Vector>())
      .def("__repr__", __repr__)
      .def(py::self + py::self)
      .def(py::self - py::self)
      .def(double() * py::self)
      .def_readwrite("x", &nsfd::Vector::x)
      .def_readwrite("y", &nsfd::Vector::y);
}
}  // namespace nsfdpy
