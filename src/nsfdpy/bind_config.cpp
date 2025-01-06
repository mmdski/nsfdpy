/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <nsfd/bcond/data.hpp>
#include <nsfd/config.hpp>

namespace py = pybind11;

namespace nsfdpy {
namespace config {
void bindConfig(py::module_ &m) {
  py::class_<nsfd::config::BoundaryCond>(m, "BoundaryCond")
      .def(py::init<nsfd::bcond::Data, nsfd::bcond::Data, nsfd::bcond::Data,
                    nsfd::bcond::Data>())
      .def_readonly("n", &nsfd::config::BoundaryCond::n)
      .def_readonly("s", &nsfd::config::BoundaryCond::s)
      .def_readonly("e", &nsfd::config::BoundaryCond::e)
      .def_readonly("w", &nsfd::config::BoundaryCond::w);

  py::class_<nsfd::config::Constants>(m, "Constants")
      .def(py::init<double, double, double>())
      .def_readonly("Re", &nsfd::config::Constants::Re)
      .def_readonly("gx", &nsfd::config::Constants::gx)
      .def_readonly("gy", &nsfd::config::Constants::gy);

  py::class_<nsfd::config::Geometry>(m, "Geometry")
      .def(py::init<size_t, size_t, double, double>())
      .def(py::init<size_t, size_t, double, double,
                    std::vector<std::pair<size_t, size_t>>>())
      .def_readonly("imax", &nsfd::config::Geometry::imax)
      .def_readonly("jmax", &nsfd::config::Geometry::jmax)
      .def_readonly("xlength", &nsfd::config::Geometry::xlength)
      .def_readonly("ylength", &nsfd::config::Geometry::ylength)
      .def_readonly("obstacles", &nsfd::config::Geometry::obstacles);

  py::class_<nsfd::config::InitialCond>(m, "InitialCond")
      .def(py::init<double, double, double>())
      .def("p", &nsfd::config::InitialCond::p)
      .def("u", &nsfd::config::InitialCond::u);

  py::class_<nsfd::config::Solver>(m, "Solver")
      .def(py::init<double, int, double, double>())
      .def_readonly("omg", &nsfd::config::Solver::omg)
      .def_readonly("itermax", &nsfd::config::Solver::itermax)
      .def_readonly("eps", &nsfd::config::Solver::eps)
      .def_readonly("gamma", &nsfd::config::Solver::gamma);

  py::class_<nsfd::config::Time>(m, "Time")
      .def(py::init<double>())
      .def(py::init<double, std::optional<double>>())
      .def_readonly("delt", &nsfd::config::Time::delt)
      .def_readonly("tau", &nsfd::config::Time::tau);
}
}  // namespace config
}  // namespace nsfdpy
