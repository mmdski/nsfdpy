/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <format>

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <nsfd/scalar.hpp>

namespace py = pybind11;

namespace {

double __float__(nsfd::Scalar &self) { return static_cast<double>(self); }

std::string __repr__(nsfd::Scalar &self) {
  return "nsfdpy.Scalar(" + std::format("{}", static_cast<double>(self)) + ")";
}

} // namespace

namespace nsfdpy {
void bindScalar(py::module_ &m) {
  py::class_<nsfd::Scalar>(m, "Scalar")
      .def(py::init<double>(), py::arg("s") = 0)
      .def("__float__", &__float__)
      .def("__repr__", &__repr__)
      .def(py::self + py::self)
      .def(py::self - py::self)
      .def(py::self - double())
      .def(double() - py::self)
      .def(py::self * py::self)
      .def(double() * py::self)
      .def(py::self * double())
      .def(py::self / double())
      .def(double() / py::self);
}

} // namespace nsfdpy
