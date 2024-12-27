/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <format>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <nsfd/bcond/data.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace bcond {
void bindData(py::module_ &m) {
  py::enum_<nsfd::bcond::Type>(m, "Type").value("NoSlip",
                                                nsfd::bcond::Type::NoSlip);

  py::class_<nsfd::bcond::Data>(m, "Data")
      .def(py::init<nsfd::bcond::Type>())
      .def(py::init<nsfd::bcond::Type, double>())
      .def_readonly("type", &nsfd::bcond::Data::type)
      .def_readonly("value", &nsfd::bcond::Data::value);
}
}  // namespace bcond
}  // namespace nsfdpy
