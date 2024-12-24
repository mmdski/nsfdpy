/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_COMP_U_NEXT_HPP_
#define NSFD_COMP_U_NEXT_HPP_

#include "../field.hpp"
#include "../grid/staggered_grid.hpp"
#include "../ops/gradient.hpp"
#include "../scalar.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace comp {
class UNext {
 private:
  nsfd::grid::StaggeredGrid &grid_;

 public:
  UNext(nsfd::grid::StaggeredGrid &grid) : grid_{grid} {}

  void operator()(nsfd::Field<nsfd::Vector> &fg, nsfd::Field<nsfd::Scalar> &p,
                  double delt, nsfd::Field<nsfd::Vector> &u_next) {
    nsfd::ops::Gradient grad_p(grid_, p);

    for (size_t i = 1; i <= grid_.imax(); ++i) {
      for (size_t j = 1; j <= grid_.jmax(); ++j) {
        u_next(i, j) = fg(i, j) - delt * grad_p(i, j);
      }
    }
  }
};
}  // namespace comp
}  // namespace nsfd

#endif
