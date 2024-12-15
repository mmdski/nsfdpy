/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_GRID_GRID_HPP_
#define NSFD_GRID_GRID_HPP_

#include <cstddef>
#include <tuple>

#include "axis.hpp"
#include "geom_data.hpp"

namespace nsfd {
namespace grid {
class Grid {
 public:
  Axis x;
  Axis y;
  Grid() : x(), y() {}
  Grid(GeometryData data, double xfrac, double yfrac)
      : x(data.xlength, data.imax, xfrac), y(data.ylength, data.jmax, yfrac) {}
  Grid(double xlength, size_t imax, double ylength, size_t jmax, double xfrac,
       double yfrac)
      : x(xlength, imax, xfrac), y(ylength, jmax, yfrac) {}

  std::tuple<double, double> operator()(size_t i, size_t j) {
    return {x[i], y[j]};
  }
};
}  // namespace grid
}  // namespace nsfd

#endif
