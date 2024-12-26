/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_BCOND_BCOND_HPP_
#define NSFD_BCOND_BCOND_HPP_

#include <memory>
#include <optional>

#include "field.hpp"
#include "grid/staggered_grid.hpp"
#include "scalar.hpp"
#include "vector.hpp"

namespace nsfd {

enum class BCType { NoSlip };

struct BCData {
 private:
  BCType type_;
  std::optional<double> data_;

 public:
  BCData(BCType type) : type_{type}, data_{std::nullopt} {}
  BCData(BCType type, double value) : type_{type}, data_{value} {}

  std::optional<double> data() { return data_; }
  BCType type() { return type_; }
};

class BCond {
 public:
  virtual ~BCond() = default;
  virtual void operator()(nsfd::Field<nsfd::Vector> &u,
                          [[maybe_unused]] nsfd::Field<nsfd::Scalar> &p) = 0;
};

/*
 * North boundary conditions
 */
class NBCond : public BCond {
 public:
  virtual void operator()(nsfd::Field<nsfd::Vector> &u,
                          [[maybe_unused]] nsfd::Field<nsfd::Scalar> &p) = 0;
};

class NoSlipNBCond : public NBCond {
 private:
  double ux_;
  nsfd::grid::StaggeredGrid &grid_;

 public:
  NoSlipNBCond(nsfd::grid::StaggeredGrid &grid) : ux_{0}, grid_(grid) {}
  NoSlipNBCond(nsfd::grid::StaggeredGrid &grid, double ux)
      : ux_{ux}, grid_(grid) {}

  void operator()(nsfd::Field<nsfd::Vector> &u,
                  [[maybe_unused]] nsfd::Field<nsfd::Scalar> &p) override {
    for (size_t i = 1; i <= grid_.imax(); ++i) {
      u(i, grid_.jmax() + 1).x = 2.0 * ux_ - u(i, grid_.jmax()).x;
      u(i, grid_.jmax()).y = 0.0;
    }
  }
};

/*
 * South boundary conditions
 */
class SBCond : public BCond {
 public:
  virtual void operator()(nsfd::Field<nsfd::Vector> &u,
                          [[maybe_unused]] nsfd::Field<nsfd::Scalar> &p) = 0;
};

class NoSlipSBCond : public SBCond {
 private:
  double ux_;
  nsfd::grid::StaggeredGrid &grid_;

 public:
  NoSlipSBCond(nsfd::grid::StaggeredGrid &grid) : ux_{0}, grid_(grid) {}
  NoSlipSBCond(nsfd::grid::StaggeredGrid &grid, double ux)
      : ux_{ux}, grid_(grid) {}

  void operator()(nsfd::Field<nsfd::Vector> &u,
                  [[maybe_unused]] nsfd::Field<nsfd::Scalar> &p) override {
    for (size_t i = 1; i <= grid_.imax(); ++i) {
      u(i, 0).x = 2.0 * ux_ - u(i, 1).x;
      u(i, 0).y = 0.0;
    }
  }
};

/*
 * East boundary conditions
 */

class EBCond : public BCond {
 public:
  virtual void operator()(nsfd::Field<nsfd::Vector> &u,
                          nsfd::Field<nsfd::Scalar> &p) = 0;
};

class NoSlipEBCond : public EBCond {
 private:
  double uy_;
  nsfd::grid::StaggeredGrid &grid_;

 public:
  NoSlipEBCond(nsfd::grid::StaggeredGrid &grid) : uy_{0}, grid_{grid} {}
  NoSlipEBCond(nsfd::grid::StaggeredGrid &grid, double uy)
      : uy_{uy}, grid_{grid} {}

  void operator()(nsfd::Field<nsfd::Vector> &u,
                  [[maybe_unused]] nsfd::Field<nsfd::Scalar> &p) override {
    for (size_t j = 1; j <= grid_.imax(); ++j) {
      u(grid_.imax(), j).x = 0.0;
      u(grid_.imax() + 1, j).y = 2.0 * uy_ - u(grid_.imax(), j).y;
    }
  }
};

/*
 * West boundary conditions
 */
class WBCond : public BCond {
 public:
  virtual void operator()(nsfd::Field<nsfd::Vector> &u,
                          [[maybe_unused]] nsfd::Field<nsfd::Scalar> &p) = 0;
};

class NoSlipWBCond : public WBCond {
 private:
  double uy_;
  nsfd::grid::StaggeredGrid &grid_;

 public:
  NoSlipWBCond(nsfd::grid::StaggeredGrid &grid) : uy_{0}, grid_{grid} {}
  NoSlipWBCond(nsfd::grid::StaggeredGrid &grid, double uy)
      : uy_{uy}, grid_{grid} {}

  void operator()(nsfd::Field<nsfd::Vector> &u,
                  [[maybe_unused]] nsfd::Field<nsfd::Scalar> &p) override {
    for (size_t j = 1; j <= grid_.imax(); ++j) {
      u(0, j).x = 0.0;
      u(0, j).y = 2.0 * uy_ - u(1, j).y;
    }
  }
};

class ApplyBCond {
 private:
  std::unique_ptr<NBCond> n_bcond_;
  std::unique_ptr<SBCond> s_bcond_;
  std::unique_ptr<EBCond> e_bcond_;
  std::unique_ptr<WBCond> w_bcond_;

 public:
  ApplyBCond(nsfd::grid::StaggeredGrid &grid, BCData n_bc, BCData s_bc,
             BCData e_bc, BCData w_bc) {
    switch (n_bc.type()) {
      case BCType::NoSlip:
        n_bcond_ =
            std::make_unique<NoSlipNBCond>(grid, n_bc.data().value_or(0));
        break;
    }

    switch (s_bc.type()) {
      case BCType::NoSlip:
        s_bcond_ =
            std::make_unique<NoSlipSBCond>(grid, s_bc.data().value_or(0));
        break;
    }

    switch (e_bc.type()) {
      case BCType::NoSlip:
        e_bcond_ =
            std::make_unique<NoSlipEBCond>(grid, e_bc.data().value_or(0));
        break;
    }

    switch (w_bc.type()) {
      case BCType::NoSlip:
        w_bcond_ =
            std::make_unique<NoSlipWBCond>(grid, w_bc.data().value_or(0));
        break;
    }
  }

  void operator()(nsfd::Field<nsfd::Vector> &u, nsfd::Field<nsfd::Scalar> &p) {
    n_bcond_->operator()(u, p);
    s_bcond_->operator()(u, p);
    e_bcond_->operator()(u, p);
    w_bcond_->operator()(u, p);
  }
};

}  // namespace nsfd

#endif
