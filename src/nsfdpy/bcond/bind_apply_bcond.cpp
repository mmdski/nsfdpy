/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/bcond.hpp>
#include <nsfd/grid/staggered_grid.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace bcond {
void bindApplyBCond(py::module_ &m) {
  py::class_<nsfd::ApplyBCond>(m, "ApplyBCond")
      .def(py::init<nsfd::grid::StaggeredGrid &, nsfd::BCData, nsfd::BCData,
                    nsfd::BCData, nsfd::BCData>())
      .def("__call__", &nsfd::ApplyBCond::operator());
}
}  // namespace bcond
}  // namespace nsfdpy
