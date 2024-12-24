/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_OPS_DIVERGENCE_HPP_
#define NSFD_OPS_DIVERGENCE_HPP_

#include "../field.hpp"
#include "../grid/staggered_grid.hpp"
#include "../scalar.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace ops {
class Divergence {
 private:
  nsfd::grid::StaggeredGrid &grid_;
  nsfd::Field<nsfd::Vector> &field_;

 public:
  Divergence(nsfd::grid::StaggeredGrid &grid, nsfd::Field<nsfd::Vector> &field)
      : grid_{grid}, field_{field} {}

  nsfd::Scalar operator()(size_t i, size_t j) {
    return (field_(i, j).x - field_(i - 1, j).x) / grid_.delx() +
           (field_(i, j).y - field_(i, j - 1).y) / grid_.dely();
  }
};
}  // namespace ops
}  // namespace nsfd

#endif
