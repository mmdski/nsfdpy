/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/grid/axis.hpp>

namespace py = pybind11;

namespace {
double __getitem__(nsfd::grid::Axis &self, size_t i) { return self[i]; }
} // namespace

namespace nsfdpy {
namespace grid {
void bindAxis(py::module_ &m) {
  py::class_<nsfd::grid::Axis>(m, "Axis")
      .def(py::init<double, size_t, double>())
      .def("__getitem__", __getitem__)
      .def("del", &nsfd::grid::Axis::del)
      .def_readonly("max", &nsfd::grid::Axis::max);
}
} // namespace grid
} // namespace nsfdpy
