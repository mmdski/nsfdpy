/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_COMP_FG_HPP_
#define NSFD_COMP_FG_HPP_

#include "../field.hpp"
#include "../grid/staggered_grid.hpp"
#include "../ops/advection.hpp"
#include "../ops/laplace.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace comp {
class FG {
 private:
  nsfd::grid::StaggeredGrid &grid_;
  nsfd::Vector g_;
  double Re_;
  double gamma_;

 public:
  FG(nsfd::grid::StaggeredGrid &grid, nsfd::Vector g, double Re, double gamma)
      : grid_{grid}, g_{g}, Re_{Re}, gamma_{gamma} {}

  void operator()(nsfd::Field<nsfd::Vector> &u, double delt,
                  nsfd::Field<nsfd::Vector> &fg) {
    nsfd::ops::Laplace<nsfd::Vector> lap(grid_, u);
    nsfd::ops::Advection adv(grid_, gamma_, u, u);

    for (size_t i = 1; i <= grid_.imax(); ++i) {
      for (size_t j = 1; j <= grid_.jmax(); ++j) {
        fg(i, j) = u(i, j) + delt * (g_ + 1.0 / Re_ * (lap(i, j) - adv(i, j)));
      }
    }

    for (size_t j = 1; j <= grid_.jmax(); ++j) {
      fg(0, j).x = u(0, j).x;
      fg(grid_.imax(), j).x = u(grid_.imax(), j).x;
    }

    for (size_t i = 1; i <= grid_.imax(); ++i) {
      fg(i, 0).y = u(i, 0).y;
      fg(i, grid_.jmax()).y = u(i, grid_.jmax()).y;
    }
  }
};
}  // namespace comp
}  // namespace nsfd

#endif
