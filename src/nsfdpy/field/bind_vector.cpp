/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <nsfd/field.hpp>
#include <nsfd/scalar.hpp>
#include <nsfd/vector.hpp>

namespace py = pybind11;

namespace {}

namespace nsfdpy {
namespace field {
void bindVector(py::module_ &m) {
  py::class_<nsfd::Field<nsfd::Vector>>(m, "VectorField")
      .def(py::init<nsfd::Field<nsfd::Vector>>())
      .def(py::init<size_t, size_t>())
      .def(py::init([](size_t imax, size_t jmax,
                       std::tuple<double, double> inital_value) {
        return new nsfd::Field<nsfd::Vector>(imax, jmax, inital_value);
      }))
      .def(py::init([](size_t imax, size_t jmax, nsfd::Vector inital_value) {
        return new nsfd::Field<nsfd::Vector>(imax, jmax, inital_value);
      }))
      .def(py::init<nsfd::grid::StaggeredGrid &, nsfd::Vector>())
      .def(
          "__getitem__",
          [](nsfd::Field<nsfd::Vector> &self,
             std::tuple<size_t, size_t> idx) -> nsfd::Vector & {
            return self(std::get<0>(idx), std::get<1>(idx));
          },
          py::return_value_policy::reference_internal, py::arg("idx"))
      .def("__setitem__",
           [](nsfd::Field<nsfd::Vector> &self, std::tuple<size_t, size_t> idx,
              nsfd::Vector u) { self(std::get<0>(idx), std::get<1>(idx)) = u; })
      .def("__setitem__",
           [](nsfd::Field<nsfd::Vector> &self, std::tuple<size_t, size_t> idx,
              std::tuple<double, double> u) {
             self(std::get<0>(idx), std::get<1>(idx)) = u;
           })
      .def("copy", &nsfd::Field<nsfd::Vector>::copy)
      .def("new_like",
           [](nsfd::Field<nsfd::Vector> &self) {
             return new nsfd::Field<nsfd::Vector>(self.n_interior());
           })
      .def("resid", &nsfd::Field<nsfd::Vector>::resid)
      .def_property_readonly(
          "values",
          [](nsfd::Field<nsfd::Vector> &self) {
            auto [n_i, n_j] = self.shape();
            py::array_t<double> s({n_i, n_j, static_cast<size_t>(2)});
            auto s_buf = s.mutable_unchecked<3>();

            for (size_t i = 0; i < n_i; ++i) {
              for (size_t j = 0; j < n_j; ++j) {
                s_buf(i, j, 0) = self(i, j).x;
                s_buf(i, j, 1) = self(i, j).y;
              }
            }
            return s;
          })
      .def_property_readonly("x",
                             [](nsfd::Field<nsfd::Vector> &self) {
                               auto [n_i, n_j] = self.shape();
                               py::array_t<double> x({n_i, n_j});
                               auto x_buf = x.mutable_unchecked<2>();

                               for (size_t i = 0; i < n_i; ++i) {
                                 for (size_t j = 0; j < n_j; ++j) {
                                   x_buf(i, j) = self(i, j).x;
                                 }
                               }
                               return x;
                             })
      .def_property_readonly("y", [](nsfd::Field<nsfd::Vector> &self) {
        auto [n_i, n_j] = self.shape();
        py::array_t<double> y({n_i, n_j});
        auto y_buf = y.mutable_unchecked<2>();

        for (size_t i = 0; i < n_i; ++i) {
          for (size_t j = 0; j < n_j; ++j) {
            y_buf(i, j) = self(i, j).y;
          }
        }
        return y;
      });
}
}  // namespace field
}  // namespace nsfdpy
