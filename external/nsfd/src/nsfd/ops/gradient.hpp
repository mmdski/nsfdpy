/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_OPS_GRADIENT_HPP_
#define NSFD_OPS_GRADIENT_HPP_

#include "../field.hpp"
#include "../grid/staggered_grid.hpp"
#include "../scalar.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace ops {

class Gradient {
 private:
  nsfd::grid::StaggeredGrid &grid_;
  nsfd::Field<nsfd::Scalar> &field_;

 public:
  Gradient(nsfd::grid::StaggeredGrid &grid, nsfd::Field<nsfd::Scalar> &field)
      : grid_{grid}, field_{field} {}

  nsfd::Vector operator()(size_t i, size_t j) {
    return {(field_(i + 1, j) - field_(i, j)) / grid_.delx(),
            (field_(i, j + 1) - field_(i, j)) / grid_.dely()};
  }
};

}  // namespace ops
}  // namespace nsfd

#endif
