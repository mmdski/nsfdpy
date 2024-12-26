/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/bcond/apply.hpp>
#include <nsfd/bcond/data.hpp>
#include <nsfd/grid/staggered_grid.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace bcond {
void bindApply(py::module_ &m) {
  py::class_<nsfd::bcond::Apply>(m, "Apply")
      .def(py::init<nsfd::grid::StaggeredGrid &, nsfd::bcond::Data,
                    nsfd::bcond::Data, nsfd::bcond::Data, nsfd::bcond::Data>())
      .def("__call__", &nsfd::bcond::Apply::operator());
}
}  // namespace bcond
}  // namespace nsfdpy
