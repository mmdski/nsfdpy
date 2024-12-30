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
#include "../grid/staggered_grid.hpp"
#include "bcond.hpp"
#include "data.hpp"

namespace nsfd {
namespace bcond {
class Apply {
 private:
  std::unique_ptr<nsfd::bcond::NBCond> n_bcond_;
  std::unique_ptr<nsfd::bcond::SBCond> s_bcond_;
  std::unique_ptr<nsfd::bcond::EBCond> e_bcond_;
  std::unique_ptr<nsfd::bcond::WBCond> w_bcond_;

 public:
  Apply(nsfd::grid::StaggeredGrid &grid, nsfd::bcond::Data n_bc,
        nsfd::bcond::Data s_bc, nsfd::bcond::Data e_bc,
        nsfd::bcond::Data w_bc) {
    switch (n_bc.type) {
      case nsfd::bcond::Type::NoSlip:
        n_bcond_ = std::make_unique<nsfd::bcond::NoSlipNBCond>(
            grid, n_bc.value.value_or(0));
        break;
      default:
        throw std::invalid_argument("Invalid north boundary condition type");
    }

    switch (s_bc.type) {
      case nsfd::bcond::Type::NoSlip:
        s_bcond_ = std::make_unique<nsfd::bcond::NoSlipSBCond>(
            grid, s_bc.value.value_or(0));
        break;
      default:
        throw std::invalid_argument("Invalid south boundary condition type");
    }

    switch (e_bc.type) {
      case nsfd::bcond::Type::NoSlip:
        e_bcond_ = std::make_unique<nsfd::bcond::NoSlipEBCond>(
            grid, e_bc.value.value_or(0));
        break;
      case nsfd::bcond::Type::Periodic:
        e_bcond_ = std::make_unique<nsfd::bcond::PeriodicEBCond>(grid);
        break;
      default:
        throw std::invalid_argument("Invalid east boundary condition type");
    }

    switch (w_bc.type) {
      case nsfd::bcond::Type::NoSlip:
        w_bcond_ = std::make_unique<nsfd::bcond::NoSlipWBCond>(
            grid, w_bc.value.value_or(0));
        break;
      case nsfd::bcond::Type::Periodic:
        w_bcond_ = std::make_unique<nsfd::bcond::PeriodicWBCond>(grid);
        break;
      default:
        throw std::invalid_argument("Invalid west boundary condition type");
    }
  }

  Apply(nsfd::grid::StaggeredGrid &grid, nsfd::config::BoundaryCond &bcond)
      : Apply(grid, bcond.n, bcond.s, bcond.e, bcond.w) {}

  void operator()(nsfd::Field<nsfd::Vector> &u, nsfd::Field<nsfd::Scalar> &p) {
    n_bcond_->operator()(u, p);
    s_bcond_->operator()(u, p);
    e_bcond_->operator()(u, p);
    w_bcond_->operator()(u, p);
  }
};
}  // namespace bcond
}  // namespace nsfd

#endif
