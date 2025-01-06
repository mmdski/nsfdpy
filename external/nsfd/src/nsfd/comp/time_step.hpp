/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_COMP_TIME_STEP_HPP_
#define NSFD_COMP_TIME_STEP_HPP_

#include <optional>
#include <tuple>

#include "../bcond/apply.hpp"
#include "../bcond/data.hpp"
#include "../config.hpp"
#include "../field.hpp"
#include "../geometry.hpp"
#include "../grid/staggered_grid.hpp"
#include "../iterpressure.hpp"
#include "../scalar.hpp"
#include "../vector.hpp"
#include "delt.hpp"
#include "fg.hpp"
#include "rhs.hpp"
#include "u_next.hpp"

namespace nsfd {
namespace comp {
class TimeStep {
 public:
  TimeStep(nsfd::config::Geometry &geometry, nsfd::config::BoundaryCond &bcond,
           nsfd::config::Constants &constants, nsfd::config::Solver &solver,
           nsfd::config::Time &time) {
    grid_ = std::make_unique<nsfd::grid::StaggeredGrid>(geometry);

    nsfd::Geometry geom =
        geometry.obstacles.has_value()
            ? nsfd::Geometry(*grid_, geometry.obstacles.value())
            : nsfd::Geometry(*grid_);

    apply_bc_ = std::make_unique<nsfd::bcond::Apply>(*grid_, bcond, geom);
    comp_delt_ = std::make_unique<nsfd::comp::DelT>(*grid_, constants, time);
    comp_fg_ = std::make_unique<nsfd::comp::FG>(*grid_, constants, solver);
    comp_rhs_ = std::make_unique<nsfd::comp::RHS>(*grid_);
    iter_p_ = std::make_unique<nsfd::IterPressure>(*grid_, solver);
    comp_u_next_ = std::make_unique<nsfd::comp::UNext>(*grid_);
    fg_ = std::make_unique<nsfd::Field<nsfd::Vector>>(*grid_);
    rhs_ = std::make_unique<nsfd::Field<nsfd::Scalar>>(*grid_);
  }

  std::tuple<double, std::tuple<int, double>> operator()(
      nsfd::Field<nsfd::Vector> &u, nsfd::Field<nsfd::Scalar> &p) {
    apply_bc_->operator()(u, p);
    delt_ = comp_delt_->operator()(u);
    comp_fg_->operator()(u, delt_, *fg_);
    comp_rhs_->operator()(*fg_, delt_, *rhs_);
    std::tuple<int, double> p_it = iter_p_->operator()(p, *rhs_);
    comp_u_next_->operator()(*fg_, p, delt_, u);
    return {delt_, p_it};
  }

 private:
  double delt_;
  std::optional<double> tau_;
  std::unique_ptr<nsfd::grid::StaggeredGrid> grid_;
  std::unique_ptr<nsfd::bcond::Apply> apply_bc_;
  std::unique_ptr<nsfd::comp::DelT> comp_delt_;
  std::unique_ptr<nsfd::comp::FG> comp_fg_;
  std::unique_ptr<nsfd::comp::RHS> comp_rhs_;
  std::unique_ptr<nsfd::IterPressure> iter_p_;
  std::unique_ptr<nsfd::comp::UNext> comp_u_next_;
  std::unique_ptr<nsfd::Field<nsfd::Vector>> fg_;
  std::unique_ptr<nsfd::Field<nsfd::Scalar>> rhs_;
};
}  // namespace comp
}  // namespace nsfd

#endif
