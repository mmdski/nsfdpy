/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_COMP_RHS_HPP_
#define NSFD_COMP_RHS_HPP_

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "../field.hpp"
#include "../grid/staggered_grid.hpp"
#include "../ops/divergence.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace comp {
class RHS {
 public:
  RHS(nsfd::grid::StaggeredGrid &grid,
      std::vector<std::pair<size_t, size_t>> &fluid_cells)
      : grid_{grid}, fluid_cells_(fluid_cells) {}

  void operator()(nsfd::Field<nsfd::Vector> &fg, double delt,
                  nsfd::Field<nsfd::Scalar> &rhs) {
    nsfd::ops::Divergence div(grid_, fg);
    for (const auto &[i, j] : fluid_cells_) {
      rhs(i, j) = 1.0 / delt * div(i, j);
    }
  }

 private:
  nsfd::grid::StaggeredGrid &grid_;
  std::vector<std::pair<size_t, size_t>> &fluid_cells_;
};
}  // namespace comp
}  // namespace nsfd

#endif
