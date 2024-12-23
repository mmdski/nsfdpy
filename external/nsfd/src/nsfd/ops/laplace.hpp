/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_OPS_LAPLACE_HPP_
#define NSFD_OPS_LAPLACE_HPP_

#include "../field/vector.hpp"
#include "../grid/staggered_grid.hpp"

namespace nsfd {
namespace ops {
class Laplace {
 private:
  nsfd::grid::StaggeredGrid &grid_;
  nsfd::field::Vector &field_;

 public:
  Laplace(nsfd::grid::StaggeredGrid &grid, nsfd::field::Vector &field)
      : grid_{grid}, field_{field} {}

  nsfd::Vector operator()(size_t i, size_t j) {
    auto dx2 = (field_(i + 1, j) - 2 * field_(i, j) + field_(i - 1, j)) /
               (grid_.delx() * grid_.delx());
    auto dy2 = (field_(i, j + 1) - 2 * field_(i, j) + field_(i, j - 1)) /
               (grid_.dely() * grid_.dely());
    return dx2 + dy2;
  }
};
}  // namespace ops
}  // namespace nsfd

#endif
