/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_GRID_STAGGERED_GRID_H_
#define NSFD_GRID_STAGGERED_GRID_H_

#include <cstddef>

#include "geom_data.hpp"
#include "grid.hpp"

namespace nsfd {
namespace grid {
class StaggeredGrid {
 private:
  double delx_;
  double dely_;
  GeometryData geom_data_;
  size_t imax_;
  size_t jmax_;

 public:
  Grid p;
  Grid u;
  Grid v;

  StaggeredGrid()
      : delx_{0},
        dely_{0},
        geom_data_{0, 0, 0, 0},
        imax_{0},
        jmax_{0},
        p(),
        u(),
        v() {}
  StaggeredGrid(double xlength, size_t imax, double ylength, size_t jmax)
      : delx_{xlength / imax},
        dely_{ylength / jmax},
        geom_data_{xlength, ylength, imax, jmax},
        imax_{imax},
        jmax_{jmax},
        p(xlength, imax, ylength, jmax, 0.5, 0.5),
        u(xlength, imax, ylength, jmax, 0, 0.5),
        v(xlength, imax, ylength, jmax, 0.5, 0) {}
  double delx() { return delx_; }
  double dely() { return dely_; }
  GeometryData geom_data() const { return geom_data_; }
  size_t imax() { return imax_; }
  size_t jmax() { return jmax_; }
};

}  // namespace grid
}  // namespace nsfd
#endif
