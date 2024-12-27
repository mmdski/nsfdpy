/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/comp/time_step.hpp>
#include <nsfd/config.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace comp {
void bindTimeStep(py::module_ &m) {
  py::class_<nsfd::comp::TimeStep>(m, "TimeStep")
      .def(py::init<nsfd::config::Geometry &, nsfd::config::BoundaryCond &,
                    nsfd::config::Constants &, nsfd::config::Solver &,
                    nsfd::config::Time &>())
      .def("__call__", &nsfd::comp::TimeStep::operator());
}
}  // namespace comp
}  // namespace nsfdpy
