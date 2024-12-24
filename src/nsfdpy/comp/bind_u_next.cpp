/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/comp/u_next.hpp>
#include <nsfd/grid/staggered_grid.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace comp {
void bindUNext(py::module_ &m) {
  py::class_<nsfd::comp::UNext>(m, "UNext")
      .def(py::init<nsfd::grid::StaggeredGrid &>())
      .def("__call__", &nsfd::comp::UNext::operator());
}
}  // namespace comp
}  // namespace nsfdpy
