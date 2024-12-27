/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <nsfd/field.hpp>
#include <nsfd/scalar.hpp>

namespace py = pybind11;

namespace {

nsfd::Scalar __getitem__(nsfd::Field<nsfd::Scalar> &self,
                         std::tuple<size_t, size_t> idx) {
  return self(std::get<0>(idx), std::get<1>(idx));
}

void __setitem__(nsfd::Field<nsfd::Scalar> &self,
                 std::tuple<size_t, size_t> idx, double s) {
  self(std::get<0>(idx), std::get<1>(idx)) = nsfd::Scalar(s);
}

py::array_t<double> values(nsfd::Field<nsfd::Scalar> &self) {
  auto [n_i, n_j] = self.shape();
  py::array_t<double> s({n_i, n_j});
  auto s_buf = s.mutable_unchecked<2>();

  for (size_t i = 0; i < n_i; ++i) {
    for (size_t j = 0; j < n_j; ++j) {
      s_buf(i, j) = static_cast<double>(self(i, j));
    }
  }
  return s;
}
}  // namespace

namespace nsfdpy {
namespace field {
void bindScalar(py::module_ &m) {
  py::class_<nsfd::Field<nsfd::Scalar>>(m, "ScalarField")
      .def(py::init<nsfd::Field<nsfd::Scalar>>())
      .def(py::init<size_t, size_t>())
      .def(py::init<size_t, size_t, double>())
      .def(py::init<size_t, size_t, nsfd::Scalar>())
      .def(py::init<nsfd::grid::StaggeredGrid &, nsfd::Scalar>())
      .def("__getitem__", &__getitem__)
      .def("__setitem__", &__setitem__)
      .def_property_readonly("values", &values);
}
}  // namespace field
}  // namespace nsfdpy
