/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_COMP_RHS_HPP_
#define NSFD_COMP_RHS_HPP_

#include "../field.hpp"
#include "../grid/staggered_grid.hpp"
#include "../ops/divergence.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace comp {
class RHS {
 private:
  nsfd::grid::StaggeredGrid &grid_;

 public:
  RHS(nsfd::grid::StaggeredGrid &grid) : grid_{grid} {}

  void operator()(nsfd::Field<nsfd::Vector> &fg, double delt,
                  nsfd::Field<nsfd::Scalar> &rhs) {
    nsfd::ops::Divergence div(grid_, fg);
    for (size_t i = 1; i <= grid_.imax(); ++i) {
      for (size_t j = 1; j <= grid_.jmax(); ++j) {
        rhs(i, j) = 1.0 / delt * div(i, j);
      }
    }
  }
};
}  // namespace comp
}  // namespace nsfd

#endif
