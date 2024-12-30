/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_COMP_DELT_HPP_
#define NSFD_COMP_DELT_HPP_

#include <algorithm>
#include <cmath>
#include <optional>

#include "../config.hpp"
#include "../field.hpp"
#include "../grid/staggered_grid.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace comp {
class DelT {
 private:
  nsfd::grid::StaggeredGrid &grid_;
  double delt_;
  double Re_;
  std::optional<double> tau_;

 public:
  DelT(nsfd::grid::StaggeredGrid &grid, nsfd::config::Constants &constants,
       nsfd::config::Time &time)
      : grid_{grid}, delt_{time.delt}, Re_{constants.Re}, tau_{time.tau} {}

  double operator()(nsfd::Field<nsfd::Vector> &u) {
    if (!tau_.has_value()) return delt_;

    double u_max_abs = -INFINITY;
    double v_max_abs = -INFINITY;

    double u_abs = 0, v_abs = 0;

    auto [imax, jmax] = u.n_interior();

    for (size_t i = 1; i <= imax; ++i) {
      for (size_t j = 1; j <= jmax; ++j) {
        u_abs = std::abs(u(i, j).x);
        v_abs = std::abs(u(i, j).y);
        if (u_abs > u_max_abs) u_max_abs = u_abs;
        if (v_abs > v_max_abs) v_max_abs = v_abs;
      }
    }

    return tau_.value() *
           std::min({Re_ / 2 /
                         (1 / (grid_.delx() * grid_.delx()) +
                          1 / (grid_.dely() * grid_.dely())),
                     grid_.delx() / u_max_abs, grid_.dely() / v_max_abs});
  }
};
}  // namespace comp
}  // namespace nsfd

#endif
