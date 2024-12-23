/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_OPS_LAPLACE_HPP_
#define NSFD_OPS_LAPLACE_HPP_

#include "../field.hpp"
#include "../grid/staggered_grid.hpp"
#include "../scalar.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace ops {
template <typename T>
class Laplace {
 private:
  nsfd::grid::StaggeredGrid &grid_;
  nsfd::Field<T> &field_;

 public:
  Laplace(nsfd::grid::StaggeredGrid &grid, nsfd::Field<T> &field)
      : grid_{grid}, field_{field} {}

  T operator()(size_t i, size_t j) {
    auto dx2 = (field_(i + 1, j) - 2 * field_(i, j) + field_(i - 1, j)) /
               (grid_.delx() * grid_.delx());
    auto dy2 = (field_(i, j + 1) - 2 * field_(i, j) + field_(i, j - 1)) /
               (grid_.dely() * grid_.dely());
    return dx2 + dy2;
  }
};

Laplace<nsfd::Scalar> lap(nsfd::grid::StaggeredGrid &grid,
                          nsfd::Field<nsfd::Scalar> &field) {
  return Laplace<nsfd::Scalar>(grid, field);
}

Laplace<nsfd::Vector> lap(nsfd::grid::StaggeredGrid &grid,
                          nsfd::Field<nsfd::Vector> &field) {
  return Laplace<nsfd::Vector>(grid, field);
}
}  // namespace ops
}  // namespace nsfd

#endif
