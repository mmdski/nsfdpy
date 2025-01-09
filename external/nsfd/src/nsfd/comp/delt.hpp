/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_COMP_DELT_HPP_
#define NSFD_COMP_DELT_HPP_

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "../config.hpp"
#include "../field.hpp"
#include "../geometry.hpp"
#include "../grid/staggered_grid.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace comp {
class DelT {
 public:
  DelT(nsfd::grid::StaggeredGrid &grid, nsfd::config::Constants &constants,
       nsfd::config::Time &time,
       std::vector<std::pair<size_t, size_t>> &fluid_cells)
      : grid_{grid},
        delt_{time.delt},
        Re_{constants.Re},
        tau_{time.tau},
        fluid_cells_(fluid_cells) {}

  double operator()(nsfd::Field<nsfd::Vector> &u) {
    if (!tau_.has_value()) return delt_;

    double u_max_abs = -INFINITY;
    double v_max_abs = -INFINITY;

    double u_abs = 0, v_abs = 0;

    for (const auto &[i, j] : fluid_cells_) {
      u_abs = std::abs(u(i, j).x);
      v_abs = std::abs(u(i, j).y);
      if (u_abs > u_max_abs) u_max_abs = u_abs;
      if (v_abs > v_max_abs) v_max_abs = v_abs;
    }

    return tau_.value() *
           std::min({Re_ / 2 /
                         (1 / (grid_.delx() * grid_.delx()) +
                          1 / (grid_.dely() * grid_.dely())),
                     grid_.delx() / u_max_abs, grid_.dely() / v_max_abs});
  }

 private:
  nsfd::grid::StaggeredGrid &grid_;
  double delt_;
  double Re_;
  std::optional<double> tau_;
  std::vector<std::pair<size_t, size_t>> &fluid_cells_;
};
}  // namespace comp
}  // namespace nsfd

#endif
