/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/comp/fg.hpp>
#include <nsfd/grid/staggered_grid.hpp>
#include <nsfd/vector.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace comp {
void bindFG(py::module_ &m) {
  py::class_<nsfd::comp::FG>(m, "FG")
      .def(
          py::init<nsfd::grid::StaggeredGrid &, nsfd::Vector, double, double>())
      .def(py::init<nsfd::grid::StaggeredGrid &, nsfd::config::Constants &,
                    nsfd::config::Solver &>())
      .def("__call__", &nsfd::comp::FG::operator());
}
}  // namespace comp
}  // namespace nsfdpy
