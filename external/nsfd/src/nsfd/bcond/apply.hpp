/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_BCOND_APPLY_HPP_
#define NSFD_BCOND_APPLY_HPP_

#include <memory>
#include <stdexcept>

#include "../config.hpp"
#include "../geometry.hpp"
#include "../grid/staggered_grid.hpp"
#include "bcond.hpp"
#include "cell.hpp"
#include "data.hpp"

namespace nsfd {
namespace bcond {
class Apply {
 public:
  Apply(nsfd::grid::StaggeredGrid &grid, nsfd::bcond::Data n_bc,
        nsfd::bcond::Data s_bc, nsfd::bcond::Data e_bc, nsfd::bcond::Data w_bc)
      : n_bcond_{nsfd::bcond::NorthDomain(grid, n_bc)},
        s_bcond_{nsfd::bcond::SouthDomain(grid, s_bc)},
        e_bcond_{nsfd::bcond::EastDomain(grid, e_bc)},
        w_bcond_{nsfd::bcond::WestDomain(grid, w_bc)},
        interior_() {}

  Apply(nsfd::grid::StaggeredGrid &grid, nsfd::config::BoundaryCond &bcond)
      : Apply(grid, bcond.n, bcond.s, bcond.e, bcond.w) {}

  Apply(nsfd::grid::StaggeredGrid &grid, nsfd::config::BoundaryCond &bcond,
        nsfd::Geometry &geom)
      : Apply(grid, bcond.n, bcond.s, bcond.e, bcond.w) {
    for (auto const &[i, j, direction] : geom.boundary_cond()) {
      interior_.emplace_back(Cell(i, j, direction));
    }
  }

  void set_fg(nsfd::Field<nsfd::Vector> &u, nsfd::Field<nsfd::Vector> &fg) {
    n_bcond_.set_fg(u, fg);
    s_bcond_.set_fg(u, fg);
    e_bcond_.set_fg(u, fg);
    w_bcond_.set_fg(u, fg);

    for (auto &c : interior_) c.set_fg(u, fg);
  }

  void set_p(nsfd::Field<nsfd::Scalar> &p) {
    n_bcond_.set_p(p);
    s_bcond_.set_p(p);
    e_bcond_.set_p(p);
    w_bcond_.set_p(p);

    for (auto &c : interior_) c.set_p(p);
  }

  void set_u(nsfd::Field<nsfd::Vector> &u) {
    n_bcond_.set_u(u);
    s_bcond_.set_u(u);
    e_bcond_.set_u(u);
    w_bcond_.set_u(u);

    for (auto &c : interior_) c.set_u(u);
  }

 private:
  nsfd::bcond::NorthDomain n_bcond_;
  nsfd::bcond::SouthDomain s_bcond_;
  nsfd::bcond::EastDomain e_bcond_;
  nsfd::bcond::WestDomain w_bcond_;

  std::vector<Cell> interior_;
};
}  // namespace bcond
}  // namespace nsfd

#endif
