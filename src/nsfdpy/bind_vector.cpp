/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <format>

#include <pybind11/pybind11.h>

#include <nsfd/vector.hpp>

namespace py = pybind11;

namespace {

std::string __repr__(nsfd::Vector &self) {
  return "nsfdpy.Vector(" + std::format("{}", self.u) + ", " +
         std::format("{}", self.v) + ")";
}
} // namespace

namespace nsfdpy {

void bindVector(py::module_ &m) {
  py::class_<nsfd::Vector>(m, "Vector")
      .def(py::init<double, double>(), py::arg("u") = 0, py::arg("v") = 0)
      .def("__repr__", __repr__)
      .def_readwrite("u", &nsfd::Vector::u)
      .def_readwrite("v", &nsfd::Vector::v);
}
} // namespace nsfdpy
