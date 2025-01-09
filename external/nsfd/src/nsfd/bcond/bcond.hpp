/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_BCOND_BCOND_HPP_
#define NSFD_BCOND_BCOND_HPP_

#include "../field.hpp"
#include "../scalar.hpp"
#include "../vector.hpp"

namespace nsfd {
namespace bcond {

class NorthDomain {
 public:
  NorthDomain(nsfd::grid::StaggeredGrid &grid, Data data)
      : grid_{grid}, type_{data.type}, value_{data.value} {}

  void set_fg(nsfd::Field<nsfd::Vector> &u, nsfd::Field<nsfd::Vector> &fg) {
    for (size_t i = 1; i <= grid_.imax(); ++i) {
      fg(i, grid_.jmax()).y = u(i, grid_.jmax()).y;
    }
  }

  void set_p(nsfd::Field<nsfd::Scalar> &p) {
    for (size_t i = 1; i <= grid_.imax(); ++i) {
      p(i, grid_.jmax() + 1) = p(i, grid_.jmax());
    }
  }

  void set_u(nsfd::Field<nsfd::Vector> &u) {
    switch (type_) {
      case Type::NoSlip:
        for (size_t i = 1; i <= grid_.imax(); ++i) {
          u(i, grid_.jmax() + 1).x = 2.0 * value_ - u(i, grid_.jmax()).x;
          u(i, grid_.jmax()).y = 0.0;
        }
        break;
      default:
        break;
    }
  }

 private:
  nsfd::grid::StaggeredGrid &grid_;
  Type type_;
  double value_;
};

class SouthDomain {
 public:
  SouthDomain(nsfd::grid::StaggeredGrid &grid, Data data)
      : grid_{grid}, type_{data.type}, value_{data.value} {}

  void set_fg(nsfd::Field<nsfd::Vector> &u, nsfd::Field<nsfd::Vector> &fg) {
    for (size_t i = 1; i <= grid_.imax(); ++i) {
      fg(i, 0).y = u(i, 0).y;
    }
  }

  void set_p(nsfd::Field<nsfd::Scalar> &p) {
    for (size_t i = 1; i <= grid_.imax(); ++i) {
      p(i, 0) = p(i, 1);
    }
  }

  void set_u(nsfd::Field<nsfd::Vector> &u) {
    switch (type_) {
      case Type::NoSlip:
        for (size_t i = 1; i <= grid_.imax(); ++i) {
          u(i, 0).x = 2.0 * value_ - u(i, 1).x;
          u(i, 0).y = 0.0;
        }
        break;
      default:
        break;
    }
  }

 private:
  nsfd::grid::StaggeredGrid &grid_;
  Type type_;
  double value_;
};

class EastDomain {
 public:
  EastDomain(nsfd::grid::StaggeredGrid &grid, Data data)
      : grid_{grid}, type_{data.type}, value_{data.value} {}

  void set_fg(nsfd::Field<nsfd::Vector> &u, nsfd::Field<nsfd::Vector> &fg) {
    for (size_t j = 1; j <= grid_.jmax(); ++j) {
      fg(grid_.imax(), j).x = u(grid_.imax(), j).x;
    }
  }

  void set_p(nsfd::Field<nsfd::Scalar> &p) {
    for (size_t j = 1; j <= grid_.jmax(); ++j) {
      p(grid_.imax() + 1, j) = p(grid_.imax(), j);
    }
  }

  void set_u(nsfd::Field<nsfd::Vector> &u) {
    switch (type_) {
      case Type::NoSlip:
        for (size_t j = 1; j <= grid_.jmax(); ++j) {
          u(grid_.imax(), j).x = 0.0;
          u(grid_.imax() + 1, j).y = 2.0 * value_ - u(grid_.imax(), j).y;
        }
        break;
      case Type::Outflow:
        for (size_t j = 1; j <= grid_.jmax(); ++j) {
          u(grid_.imax(), j).x = u(grid_.imax() - 1, j).x;
          u(grid_.imax() + 1, j).y = u(grid_.imax(), j).y;
        }
        break;
      case Type::Periodic:
        for (size_t j = 1; j <= grid_.jmax(); ++j) {
          u(grid_.imax(), j).x = u(1, j).x;
          u(grid_.imax() + 1, j).y = u(2, j).y;
        }
        break;
      default:
        break;
    }
  }

 private:
  nsfd::grid::StaggeredGrid &grid_;
  Type type_;
  double value_;
};

class WestDomain {
 public:
  WestDomain(nsfd::grid::StaggeredGrid &grid, Data data)
      : grid_{grid}, type_{data.type}, value_{data.value} {}

  void set_fg(nsfd::Field<nsfd::Vector> &u, nsfd::Field<nsfd::Vector> &fg) {
    for (size_t j = 1; j <= grid_.jmax(); ++j) {
      fg(0, j).x = u(0, j).x;
    }
  }

  void set_p(nsfd::Field<nsfd::Scalar> &p) {
    if (type_ == Type::Periodic) {
      for (size_t j = 1; j <= grid_.jmax(); ++j) {
        p(1, j) = p(grid_.imax(), j);
      }
    }
    for (size_t j = 1; j <= grid_.jmax(); ++j) {
      p(0, j) = p(1, j);
    }
  }

  void set_u(nsfd::Field<nsfd::Vector> &u) {
    switch (type_) {
      case Type::Inflow:
        for (size_t j = 1; j <= grid_.jmax(); ++j) {
          u(0, j).x = value_;
          u(0, j).y = -u(1, j).y;
        }
        break;
      case Type::NoSlip:
        for (size_t j = 1; j <= grid_.jmax(); ++j) {
          u(0, j).x = 0.0;
          u(0, j).y = 2.0 * value_ - u(1, j).y;
        }
        break;
      case Type::Periodic:
        for (size_t j = 1; j <= grid_.jmax(); ++j) {
          u(0, j).x = u(grid_.imax() - 1, j).x;
          u(0, j).y = u(grid_.imax() - 1, j).y;
          u(1, j).y = u(grid_.imax(), j).y;
        }
        break;
      default:
        break;
    }
  }

 private:
  nsfd::grid::StaggeredGrid &grid_;
  Type type_;
  double value_;
};
}  // namespace bcond
}  // namespace nsfd

#endif
