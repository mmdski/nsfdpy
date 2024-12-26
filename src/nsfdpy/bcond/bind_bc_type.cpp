/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include <nsfd/bcond.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace bcond {
void bindBCType(py::module_ &m) {
  py::enum_<nsfd::BCType>(m, "Type").value("NoSlip", nsfd::BCType::NoSlip);
}
}  // namespace bcond
}  // namespace nsfdpy
