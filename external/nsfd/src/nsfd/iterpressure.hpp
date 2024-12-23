/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_ITERPRESSURE_HPP_
#define NSFD_ITERPRESSURE_HPP_

#include <cmath>
#include <tuple>

#include "field.hpp"
#include "grid/staggered_grid.hpp"
#include "ops/laplace.hpp"
#include "scalar.hpp"

namespace nsfd {
class IterPressure {
 private:
  nsfd::grid::StaggeredGrid &grid_;
  double omg_;
  int itermax_;
  double eps_;
  nsfd::Field<nsfd::Scalar> rit_;

  void calc_rit(nsfd::Field<nsfd::Scalar> &pit,
                const nsfd::Field<nsfd::Scalar> &rhs) {
    auto lap_p = nsfd::ops::Laplace<nsfd::Scalar>(grid_, pit);
    for (size_t i = 1; i <= grid_.imax(); ++i) {
      for (size_t j = 1; j <= grid_.jmax(); ++j) {
        rit_(i, j) = lap_p(i, j) - rhs(i, j);
      }
    }
  }

  double calc_norm(nsfd::Field<nsfd::Scalar> &pit,
                   const nsfd::Field<nsfd::Scalar> &rhs) {
    calc_rit(pit, rhs);

    double s = 0;

    for (size_t i = 1; i <= grid_.imax(); ++i) {
      for (size_t j = 1; j <= grid_.jmax(); ++j) {
        s += rit_(i, j) * rit_(i, j);
      }
    }

    return std::sqrt(s);
  }

  void set_bc(nsfd::Field<nsfd::Scalar> &pit) {
    for (size_t i = 1; i <= grid_.imax(); ++i) {
      pit(i, 0) = pit(i, 1);
      pit(i, grid_.jmax() + 1) = pit(i, grid_.jmax());
    }
    for (size_t j = 1; j <= grid_.jmax(); ++j) {
      pit(0, j) = pit(1, j);
      pit(grid_.imax() + 1, j) = pit(grid_.imax(), j);
    }
  }

 public:
  IterPressure(nsfd::grid::StaggeredGrid &grid, double omg, int itermax,
               double eps)
      : grid_{grid}, omg_{omg}, itermax_{itermax}, eps_{eps}, rit_(grid) {}

  std::tuple<int, double> operator()(nsfd::Field<nsfd::Scalar> &pit,
                                     const nsfd::Field<nsfd::Scalar> &rhs) {
    int it = 1;
    double norm = INFINITY;
    for (; it <= itermax_; ++it) {
      for (size_t i = 1; i <= grid_.imax(); ++i) {
        for (size_t j = 1; j <= grid_.jmax(); ++j) {
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
      }

      norm = calc_norm(pit, rhs);
      if (norm < eps_) {
        break;
      }
    }

    return {it, norm};
  }
};
}  // namespace nsfd

#endif
