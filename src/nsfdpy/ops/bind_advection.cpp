/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

#include <nsfd/field.hpp>
#include <nsfd/ops/advection.hpp>
#include <nsfd/vector.hpp>

namespace nsfdpy {
namespace ops {
void bindAdvection(py::module_ &m) {
  py::class_<nsfd::ops::Advection>(m, "Advection")
      .def(py::init<nsfd::grid::StaggeredGrid &, double,
                    nsfd::Field<nsfd::Vector> &, nsfd::Field<nsfd::Vector> &>())
      .def("__call__", &nsfd::ops::Advection::operator());
}
}  // namespace ops
}  // namespace nsfdpy
