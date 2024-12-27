/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/bcond/data.hpp>
#include <nsfd/config.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace config {
void bindConfig(py::module_ &m) {
  py::class_<nsfd::config::BoundaryCond>(m, "BoundaryCond")
      .def(py::init<nsfd::bcond::Data, nsfd::bcond::Data, nsfd::bcond::Data,
                    nsfd::bcond::Data>());

  py::class_<nsfd::config::Constants>(m, "Constants")
      .def(py::init<double, double, double>());

  py::class_<nsfd::config::Geometry>(m, "Geometry")
      .def(py::init<size_t, size_t, double, double>());

  py::class_<nsfd::config::InitialCond>(m, "InitialCond")
      .def(py::init<double, double, double>());

  py::class_<nsfd::config::Solver>(m, "Solver")
      .def(py::init<double, int, double, double>());

  py::class_<nsfd::config::Time>(m, "Time").def(py::init<double>());
}
}  // namespace config
}  // namespace nsfdpy
