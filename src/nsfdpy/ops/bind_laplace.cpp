/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

#include <nsfd/field.hpp>
#include <nsfd/ops/laplace.hpp>
#include <nsfd/vector.hpp>

namespace nsfdpy {
namespace ops {
void bindLaplace(py::module_ &m) {
  py::class_<nsfd::ops::Laplace<nsfd::Vector>>(m, "VectorLaplace")
      .def(py::init<nsfd::grid::StaggeredGrid &, nsfd::Field<nsfd::Vector> &>())
      .def("__call__", &nsfd::ops::Laplace<nsfd::Vector>::operator());
}
}  // namespace ops
}  // namespace nsfdpy
