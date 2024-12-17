/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <iostream>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <nsfd/field/scalar.hpp>
#include <nsfd/field/vector.hpp>
#include <nsfd/grid/axis.hpp>
#include <nsfd/grid/geom_data.hpp>
#include <nsfd/grid/grid.hpp>
#include <nsfd/grid/staggered_grid.hpp>
#include <nsfd/scalar.hpp>
#include <nsfd/vector.hpp>

#include "bindings.hpp"

namespace py = pybind11;

py::module np = py::module::import("numpy");

PYBIND11_MODULE(_nsfd, m) {
  nsfdpy::bindScalar(m);
  nsfdpy::bindVector(m);

  auto m_grid = m.def_submodule("grid");

  nsfdpy::grid::bindAxis(m_grid);
  nsfdpy::grid::bindGrid(m_grid);
  nsfdpy::grid::bindStaggeredGrid(m_grid);

  auto m_field = m.def_submodule("field");

  nsfdpy::field::bindScalar(m_field);
  nsfdpy::field::bindVector(m_field);
}
