/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/config.hpp>
#include <nsfd/field.hpp>
#include <nsfd/grid/staggered_grid.hpp>
#include <nsfd/iterpressure.hpp>
#include <nsfd/scalar.hpp>

namespace py = pybind11;

namespace nsfdpy {
void bindIterPressure(py::module_ &m) {
  py::class_<nsfd::IterPressure>(m, "IterPressure")
      .def(py::init<nsfd::grid::StaggeredGrid &, double, int, double>())
      .def(py::init<nsfd::grid::StaggeredGrid &, nsfd::config::Solver &>())
      .def("__call__", &nsfd::IterPressure::operator());
}
}  // namespace nsfdpy
