/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <iostream>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <nsfd/field/scalar.hpp>
#include <nsfd/field/vector.hpp>
#include <nsfd/grid/axis.hpp>
#include <nsfd/grid/geom_data.hpp>
#include <nsfd/grid/grid.hpp>
#include <nsfd/grid/staggered_grid.hpp>
#include <nsfd/scalar.hpp>
#include <nsfd/vector.hpp>

namespace py = pybind11;

py::module np = py::module::import("numpy");

PYBIND11_MODULE(_nsfd, m) {

  py::class_<nsfd::Vector>(m, "Vector")
      .def(py::init<double, double>(), py::arg("u") = 0, py::arg("v") = 0)
      .def("__repr__",
           [](const nsfd::Vector &self) {
             return "nsfdpy.Vector(" + std::to_string(self.u) + ", " +
                    std::to_string(self.v) + ")";
           })
      .def_readwrite("u", &nsfd::Vector::u)
      .def_readwrite("v", &nsfd::Vector::v);

  auto m_grid = m.def_submodule("grid");

  py::class_<nsfd::grid::Axis>(m_grid, "Axis")
      .def(py::init([](double length, size_t max, double frac) {
             return new nsfd::grid::Axis(length, max, frac);
           }),
           py::arg("length"), py::arg("max"), py::arg("frac") = 1)
      .def("__getitem__",
           [](nsfd::grid::Axis &self, size_t i) { return self[i]; })
      .def("del", &nsfd::grid::Axis::del)
      .def_readonly("max", &nsfd::grid::Axis::max);

  py::class_<nsfd::grid::Grid>(m_grid, "Grid")
      .def(py::init([](const py::dict &geom_data, double xfrac = 1,
                       double yfrac = 1) {
             return new nsfd::grid::Grid(geom_data["xlength"].cast<double>(),
                                         geom_data["imax"].cast<size_t>(),
                                         geom_data["ylength"].cast<double>(),
                                         geom_data["jmax"].cast<size_t>(),
                                         xfrac, yfrac);
           }),
           py::arg("geom_data"), py::arg("xfrac") = 1, py::arg("yfrac") = 1)
      .def_readonly("x", &nsfd::grid::Grid::x)
      .def_readonly("y", &nsfd::grid::Grid::y)
      .def("meshgrid", [](const nsfd::grid::Grid &self) {
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
      });

  py::class_<nsfd::grid::StaggeredGrid>(m_grid, "StaggeredGrid")
      .def(py::init([](const py::dict &geom_data) {
        return new nsfd::grid::StaggeredGrid(
            geom_data["xlength"].cast<double>(),
            geom_data["imax"].cast<size_t>(),
            geom_data["ylength"].cast<double>(),
            geom_data["jmax"].cast<size_t>());
      }))
      .def_readonly("p", &nsfd::grid::StaggeredGrid::p)
      .def_readonly("u", &nsfd::grid::StaggeredGrid::u)
      .def_readonly("v", &nsfd::grid::StaggeredGrid::v)
      .def_property_readonly("delx", &nsfd::grid::StaggeredGrid::delx)
      .def_property_readonly("dely", &nsfd::grid::StaggeredGrid::dely)
      .def_property_readonly("imax", &nsfd::grid::StaggeredGrid::imax)
      .def_property_readonly("jmax", &nsfd::grid::StaggeredGrid::jmax)
      .def_property_readonly(
          "geom_data", [](const nsfd::grid::StaggeredGrid &self) {
            nsfd::grid::GeometryData geom_data = self.geom_data();
            py::dict py_geom_data;
            py_geom_data["xlength"] = geom_data.xlength;
            py_geom_data["ylength"] = geom_data.ylength;
            py_geom_data["imax"] = geom_data.imax;
            py_geom_data["jmax"] = geom_data.jmax;
            return py_geom_data;
          });

  auto m_field = m.def_submodule("field");

  py::class_<nsfd::field::Scalar>(m_field, "ScalarField")
      .def(py::init([](size_t imax, size_t jmax) {
        return new nsfd::field::Scalar(imax, jmax);
      }))
      .def(py::init([](size_t imax, size_t jmax, double initial_value) {
        nsfd::Scalar s = nsfd::Scalar(initial_value);
        return new nsfd::field::Scalar(imax, jmax, s);
      }))
      .def("__getitem__",
           [](nsfd::field::Scalar &self, std::tuple<size_t, size_t> idx) {
             return static_cast<double>(
                 self(std::get<0>(idx), std::get<1>(idx)));
           })
      .def("__setitem__",
           [](nsfd::field::Scalar &self, std::tuple<size_t, size_t> idx,
              double s) {
             self(std::get<0>(idx), std::get<1>(idx)) = nsfd::Scalar(s);
           })
      .def_static("loadtxt",
                  [](const std::string &filename) {
                    py::object dtype = np.attr("float64");
                    py::array s = np.attr("loadtxt")(py::str(filename),
                                                     py::arg("dtype") = dtype,
                                                     py::arg("ndmin") = 2);
                    const ssize_t *shape = s.shape();
                    if (shape[0] < 3)
                      throw std::runtime_error(
                          "shape[0] must be greater than or equal to 3");
                    if (shape[1] < 3)
                      throw std::runtime_error(
                          "shape[1] must be greater than or equal to 3");

                    size_t n_i = static_cast<size_t>(shape[0]);
                    size_t n_j = static_cast<size_t>(shape[1]);

                    nsfd::field::Scalar sf(n_i - 2, n_j - 2);
                    const double *s_data =
                        static_cast<const double *>(s.data());

                    for (size_t i = 0; i < n_i; ++i) {
                      for (size_t j = 0; j < n_j; ++j) {
                        sf(i, j) = s_data[i * n_j + j];
                      }
                    }

                    return sf;
                  })
      .def("max_abs", &nsfd::field::Scalar::max_abs)
      .def("savetxt",
           [](nsfd::field::Scalar &self, const std::string &filename) {
             auto [n_i, n_j] = self.shape();
             py::array_t<double> s({n_i, n_j});
             auto s_buf = s.mutable_unchecked<2>();

             for (size_t i = 0; i < n_i; ++i) {
               for (size_t j = 0; j < n_j; ++j) {
                 s_buf(i, j) = static_cast<double>(self(i, j));
               }
             }

             np.attr("savetxt")(filename, s);
           })
      .def("shape", &nsfd::field::Scalar::shape)
      .def_property_readonly("values", [](nsfd::field::Scalar &self) {
        auto [n_i, n_j] = self.shape();
        py::array_t<double> s({n_i, n_j});
        auto s_buf = s.mutable_unchecked<2>();

        for (size_t i = 0; i < n_i; ++i) {
          for (size_t j = 0; j < n_j; ++j) {
            s_buf(i, j) = static_cast<double>(self(i, j));
          }
        }
        return s;
      });

  py::class_<nsfd::field::Vector>(m_field, "VectorField")
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
      .def_property_readonly("values",
                             [](nsfd::field::Vector &self) {
                               auto [n_i, n_j] = self.shape();
                               py::array_t<double> s(
                                   {n_i, n_j, static_cast<size_t>(2)});
                               auto s_buf = s.mutable_unchecked<3>();

                               for (size_t i = 0; i < n_i; ++i) {
                                 for (size_t j = 0; j < n_j; ++j) {
                                   s_buf(i, j, 0) = self(i, j).u;
                                   s_buf(i, j, 1) = self(i, j).v;
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
                                   u_buf(i, j) = self(i, j).u;
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
                                   v_buf(i, j) = self(i, j).v;
                                 }
                               }
                               return v;
                             })
      .def("max_abs_components", &nsfd::field::Vector::max_abs_components);
}
