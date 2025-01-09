/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_COMP_FG_HPP_
#define NSFD_COMP_FG_HPP_

#include <cstddef>
#include <tuple>
#include <vector>

#include "../bcond/apply.hpp"
#include "../config.hpp"
#include "../field.hpp"
#include "../grid/staggered_grid.hpp"
#include "../ops/advection.hpp"
#include "../ops/laplace.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace comp {
class FG {
 public:
  FG(nsfd::grid::StaggeredGrid &grid, nsfd::Vector g, double Re, double gamma,
     std::vector<std::pair<size_t, size_t>> &fluid_cells,
     nsfd::bcond::Apply &apply_bcond)
      : grid_{grid},
        g_{g},
        Re_{Re},
        gamma_{gamma},
        fluid_cells_(fluid_cells),
        apply_bcond_(apply_bcond) {
    (void)apply_bcond_;
  }

  FG(nsfd::grid::StaggeredGrid &grid, nsfd::config::Constants &constants,
     nsfd::config::Solver &solver,
     std::vector<std::pair<size_t, size_t>> &fluid_cells,
     nsfd::bcond::Apply &apply_bcond)
      : FG(grid, {constants.gx, constants.gy}, constants.Re, solver.gamma,
           fluid_cells, apply_bcond) {}

  void operator()(nsfd::Field<nsfd::Vector> &u, double delt,
                  nsfd::Field<nsfd::Vector> &fg) {
    nsfd::ops::Laplace<nsfd::Vector> lap(grid_, u);
    nsfd::ops::Advection adv(grid_, gamma_, u, u);

    for (const auto &[i, j] : fluid_cells_) {
      fg(i, j) = u(i, j) + delt * (g_ + 1.0 / Re_ * lap(i, j) - adv(i, j));
    }

    apply_bcond_.set_fg(u, fg);
  }

 private:
  nsfd::grid::StaggeredGrid &grid_;
  nsfd::Vector g_;
  double Re_;
  double gamma_;
  std::vector<std::pair<size_t, size_t>> &fluid_cells_;
  nsfd::bcond::Apply &apply_bcond_;
};
}  // namespace comp
}  // namespace nsfd

#endif
