/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>

#include "bindings.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_nsfd, m) {
  nsfdpy::bindScalar(m);
  nsfdpy::bindVector(m);

  nsfdpy::bindIterPressure(m);

  auto m_bcond = m.def_submodule("bcond");
  nsfdpy::bcond::bindApply(m_bcond);
  nsfdpy::bcond::bindData(m_bcond);

  auto m_comp = m.def_submodule("comp");

  nsfdpy::comp::bindFG(m_comp);
  nsfdpy::comp::bindRHS(m_comp);
  nsfdpy::comp::bindTimeStep(m_comp);
  nsfdpy::comp::bindUNext(m_comp);

  auto m_config = m.def_submodule("config");

  nsfdpy::config::bindConfig(m_config);

  auto m_field = m.def_submodule("field");

  nsfdpy::field::bindScalar(m_field);
  nsfdpy::field::bindVector(m_field);

  auto m_grid = m.def_submodule("grid");

  nsfdpy::grid::bindAxis(m_grid);
  nsfdpy::grid::bindGrid(m_grid);
  nsfdpy::grid::bindStaggeredGrid(m_grid);

  auto m_ops = m.def_submodule("ops");

  nsfdpy::ops::bindAdvection(m_ops);
  nsfdpy::ops::bindGradient(m_ops);
  nsfdpy::ops::bindLaplace(m_ops);
}
