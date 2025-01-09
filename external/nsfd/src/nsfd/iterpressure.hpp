/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_ITERPRESSURE_HPP_
#define NSFD_ITERPRESSURE_HPP_

#include <cmath>
#include <tuple>

#include "bcond/apply.hpp"
#include "config.hpp"
#include "field.hpp"
#include "grid/staggered_grid.hpp"
#include "ops/laplace.hpp"
#include "scalar.hpp"

namespace nsfd {
class IterPressure {
 public:
  IterPressure(nsfd::grid::StaggeredGrid &grid, nsfd::bcond::Apply &apply_bcond,
               std::vector<std::pair<size_t, size_t>> &fluid_cells, double omg,
               int itermax, double eps)
      : grid_{grid},
        omg_{omg},
        itermax_{itermax},
        eps_{eps},
        rit_(grid),
        fluid_cells_{fluid_cells},
        apply_bcond_{apply_bcond} {}
  IterPressure(nsfd::grid::StaggeredGrid &grid, nsfd::config::Solver &solver,
               nsfd::bcond::Apply &apply_bcond,
               std::vector<std::pair<size_t, size_t>> &fluid_cells)
      : IterPressure(grid, apply_bcond, fluid_cells, solver.omg, solver.itermax,
                     solver.eps) {}

  std::tuple<int, double> operator()(nsfd::Field<nsfd::Scalar> &pit,
                                     const nsfd::Field<nsfd::Scalar> &rhs) {
    int it = 1;
    double norm = INFINITY;
    for (; it <= itermax_; ++it) {
      for (auto &[i, j] : fluid_cells_) {
        pit(i, j) = (1.0 - omg_) * pit(i, j) +
                    omg_ /
                        (2.0 / (grid_.delx() * grid_.delx()) +
                         2.0 / (grid_.dely() * grid_.dely())) *
                        ((pit(i + 1, j) + pit(i - 1, j)) /
                             (grid_.delx() * grid_.delx()) +
                         (pit(i, j + 1) + pit(i, j - 1)) /
                             (grid_.dely() * grid_.dely()) -
                         rhs(i, j));
      }

      apply_bcond_.set_p(pit);

      norm = calc_norm(pit, rhs);
      if (norm < eps_) {
        break;
      }
    }

    return {it, norm};
  }

 private:
  nsfd::grid::StaggeredGrid &grid_;
  double omg_;
  int itermax_;
  double eps_;
  nsfd::Field<nsfd::Scalar> rit_;
  std::vector<std::pair<size_t, size_t>> &fluid_cells_;
  nsfd::bcond::Apply &apply_bcond_;

  void calc_rit(nsfd::Field<nsfd::Scalar> &pit,
                const nsfd::Field<nsfd::Scalar> &rhs) {
    auto lap_p = nsfd::ops::Laplace<nsfd::Scalar>(grid_, pit);
    for (auto &[i, j] : fluid_cells_) {
      rit_(i, j) = lap_p(i, j) - rhs(i, j);
    }
  }

  double calc_norm(nsfd::Field<nsfd::Scalar> &pit,
                   const nsfd::Field<nsfd::Scalar> &rhs) {
    calc_rit(pit, rhs);

    double s = 0;
    double n = 0;

    for (auto &[i, j] : fluid_cells_) {
      s += rit_(i, j) * rit_(i, j);
      n += 1;
    }

    return std::sqrt(s / n);
  }
};
}  // namespace nsfd

#endif
