#include <pybind11/pybind11.h>

#include <nsfd/vector.hpp>

namespace py = pybind11;

PYBIND11_MODULE(_nsfd, m) {
  py::class_<nsfd::Vector>(m, "Vector")
      .def(py::init<double, double>(), py::arg("x") = 0, py::arg("y") = 0)
      .def_readwrite("x", &nsfd::Vector::x)
      .def_readwrite("y", &nsfd::Vector::y);
}
