/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <nsfd/field/vector.hpp>
#include <nsfd/scalar.hpp>

namespace py = pybind11;

namespace {}

namespace nsfdpy {
namespace field {
void bindVector(py::module_ &m) {
  py::class_<nsfd::field::Vector>(m, "VectorField")
      .def(py::init([](size_t imax, size_t jmax) {
        return new nsfd::field::Vector(imax, jmax);
      }))
      .def(py::init([](size_t imax, size_t jmax,
                       std::tuple<double, double> inital_value) {
        return new nsfd::field::Vector(imax, jmax, inital_value);
      }))
      .def(py::init([](size_t imax, size_t jmax, nsfd::Vector inital_value) {
        return new nsfd::field::Vector(imax, jmax, inital_value);
      }))
      .def("__add__",
           [](const nsfd::field::Vector &self, const nsfd::field::Vector &rhs)
               -> nsfd::field::Vector { return self + rhs; })
      .def("__radd__",
           [](const nsfd::field::Vector &self, double lhs)
               -> nsfd::field::Vector { return nsfd::Scalar(lhs) + self; })
      .def("__radd__",
           [](const nsfd::field::Vector &self, std::tuple<double, double> lhs)
               -> nsfd::field::Vector { return nsfd::Vector(lhs) + self; })
      .def("__sub__",
           [](const nsfd::field::Vector &self, const nsfd::field::Vector &rhs)
               -> nsfd::field::Vector { return self - rhs; })
      .def("__rmul__",
           [](const nsfd::field::Vector &self, double lhs)
               -> nsfd::field::Vector { return nsfd::Scalar(lhs) * self; })
      .def(
          "__getitem__",
          [](nsfd::field::Vector &self,
             std::tuple<size_t, size_t> idx) -> nsfd::Vector & {
            return self(std::get<0>(idx), std::get<1>(idx));
          },
          py::return_value_policy::reference_internal, py::arg("idx"))
      .def("__setitem__",
           [](nsfd::field::Vector &self, std::tuple<size_t, size_t> idx,
              nsfd::Vector u) { self(std::get<0>(idx), std::get<1>(idx)) = u; })
      .def("__setitem__",
           [](nsfd::field::Vector &self, std::tuple<size_t, size_t> idx,
              std::tuple<double, double> u) {
             self(std::get<0>(idx), std::get<1>(idx)) = u;
           })
      .def_property_readonly(
          "values",
          [](nsfd::field::Vector &self) {
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
      .def_property_readonly("u",
                             [](nsfd::field::Vector &self) {
                               auto [n_i, n_j] = self.shape();
                               py::array_t<double> u({n_i, n_j});
                               auto u_buf = u.mutable_unchecked<2>();

                               for (size_t i = 0; i < n_i; ++i) {
                                 for (size_t j = 0; j < n_j; ++j) {
                                   u_buf(i, j) = self(i, j).x;
                                 }
                               }
                               return u;
                             })
      .def_property_readonly("v",
                             [](nsfd::field::Vector &self) {
                               auto [n_i, n_j] = self.shape();
                               py::array_t<double> v({n_i, n_j});
                               auto v_buf = v.mutable_unchecked<2>();

                               for (size_t i = 0; i < n_i; ++i) {
                                 for (size_t j = 0; j < n_j; ++j) {
                                   v_buf(i, j) = self(i, j).y;
                                 }
                               }
                               return v;
                             })
      .def("max_abs_components", &nsfd::field::Vector::max_abs_components);
}
}  // namespace field
}  // namespace nsfdpy
