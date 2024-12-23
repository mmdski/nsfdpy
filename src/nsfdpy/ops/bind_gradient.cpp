/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/field/scalar.hpp>
#include <nsfd/grid/staggered_grid.hpp>
#include <nsfd/ops/gradient.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace ops {
void bindGradient(py::module_ &m) {
  py::class_<nsfd::ops::Gradient>(m, "Gradient")
      .def(py::init<nsfd::grid::StaggeredGrid &, nsfd::field::Scalar &>())
      .def("__call__", &nsfd::ops::Gradient::operator());
}

}  // namespace ops
}  // namespace nsfdpy
