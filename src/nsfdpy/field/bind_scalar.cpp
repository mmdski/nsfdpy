/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <nsfd/field/scalar.hpp>

namespace py = pybind11;

namespace {
py::module np = py::module::import("numpy");

nsfd::Scalar __getitem__(nsfd::field::Scalar &self,
                         std::tuple<size_t, size_t> idx) {
  return self(std::get<0>(idx), std::get<1>(idx));
}

void __setitem__(nsfd::field::Scalar &self, std::tuple<size_t, size_t> idx,
                 double s) {
  self(std::get<0>(idx), std::get<1>(idx)) = nsfd::Scalar(s);
}

nsfd::field::Scalar *loadtxt(const std::string &filename) {
  py::object dtype = np.attr("float64");
  py::array s = np.attr("loadtxt")(py::str(filename), py::arg("dtype") = dtype,
                                   py::arg("ndmin") = 2);
  const ssize_t *shape = s.shape();
  if (shape[0] < 3)
    throw std::runtime_error("shape[0] must be greater than or equal to 3");
  if (shape[1] < 3)
    throw std::runtime_error("shape[1] must be greater than or equal to 3");

  size_t n_i = static_cast<size_t>(shape[0]);
  size_t n_j = static_cast<size_t>(shape[1]);

  nsfd::field::Scalar *sf = new nsfd::field::Scalar(n_i - 2, n_j - 2);
  const double *s_data = static_cast<const double *>(s.data());

  for (size_t i = 0; i < n_i; ++i) {
    for (size_t j = 0; j < n_j; ++j) {
      sf->operator()(i, j) = s_data[i * n_j + j];
    }
  }

  return sf;
}

nsfd::field::Scalar *new_like(nsfd::field::Scalar &self) {
  return new nsfd::field::Scalar(self.n_interior());
}

void savetxt(nsfd::field::Scalar &self, const std::string &filename) {
  auto [n_i, n_j] = self.shape();
  py::array_t<double> s({n_i, n_j});
  auto s_buf = s.mutable_unchecked<2>();

  for (size_t i = 0; i < n_i; ++i) {
    for (size_t j = 0; j < n_j; ++j) {
      s_buf(i, j) = static_cast<double>(self(i, j));
    }
  }

  np.attr("savetxt")(filename, s);
}

py::array_t<double> values(nsfd::field::Scalar &self) {
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
  py::class_<nsfd::field::Scalar>(m, "ScalarField")
      .def(py::init<size_t, size_t>())
      .def(py::init<size_t, size_t, double>())
      .def(py::init<size_t, size_t, nsfd::Scalar>())
      .def("__getitem__", &__getitem__)
      .def("__setitem__", &__setitem__)
      .def_static("loadtxt", &loadtxt)
      .def("max_abs", &nsfd::field::Scalar::max_abs)
      .def("new_like", &new_like)
      .def("savetxt", &savetxt)
      .def("shape", &nsfd::field::Scalar::shape)
      .def_property_readonly("values", &values);
}
}  // namespace field
}  // namespace nsfdpy
