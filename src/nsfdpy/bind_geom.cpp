/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <nsfd/geometry.hpp>
#include <nsfd/grid/staggered_grid.hpp>

namespace py = pybind11;

namespace nsfdpy {

void bindGeometry(py::module_ &m) {
  py::class_<nsfd::Geometry>(m, "Geometry")
      .def(py::init<size_t, size_t>())
      .def(py::init<nsfd::grid::StaggeredGrid &>())
      .def(py::init<nsfd::grid::StaggeredGrid &,
                    std::vector<std::pair<size_t, size_t>>>())
      .def("boundary_cells", &nsfd::Geometry::boundary_cells)
      .def("fluid_cells", &nsfd::Geometry::fluid_cells)
      .def("obstacle_cells", &nsfd::Geometry::obstacle_cells);
}
}  // namespace nsfdpy
