/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFDPY_BINDINGS_HPP_
#define NSFDPY_BINDINGS_HPP_

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace nsfdpy {
void bindScalar(py::module_ &m);
void bindVector(py::module_ &m);

void bindIterPressure(py::module_ &m);

namespace bcond {
void bindApplyBCond(py::module_ &m);
void bindBCType(py::module_ &m);
void bindBCData(py::module_ &m);
}  // namespace bcond

namespace comp {
void bindFG(py::module_ &m);
void bindRHS(py::module_ &m);
void bindUNext(py::module_ &m);
}  // namespace comp

namespace field {
void bindScalar(py::module_ &m);
void bindVector(py::module_ &m);
}  // namespace field

namespace grid {
void bindAxis(py::module_ &m);
void bindGrid(py::module_ &m);
void bindStaggeredGrid(py::module_ &m);
}  // namespace grid

namespace ops {
void bindAdvection(py::module_ &m);
void bindGradient(py::module_ &m);
void bindLaplace(py::module_ &m);
}  // namespace ops
}  // namespace nsfdpy

#endif
