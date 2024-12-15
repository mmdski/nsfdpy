/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_SCALAR_HPP_
#define NSFD_SCALAR_HPP_

#include <cmath>

#include "field/vector.hpp"

namespace nsfd {
class Scalar {
 private:
  double value_;

 public:
  Scalar() : value_{0} {}
  Scalar(double value) : value_{value} {}

  /* addition */
  field::Vector operator+(const field::Vector &rhs) const {
    auto [imax, jmax] = rhs.n_interior();
    field::Vector res(imax, jmax);

    for (size_t i = 0; i < imax + 2; ++i) {
      for (size_t j = 0; j < jmax + 2; ++j) {
        res(i, j).u = value_ + rhs(i, j).u;
        res(i, j).v = value_ + rhs(i, j).v;
      }
    }

    return res;
  }

  /* multiplication */
  field::Vector operator*(const field::Vector &rhs) const {
    auto [imax, jmax] = rhs.n_interior();
    field::Vector res(imax, jmax);

    for (size_t i = 0; i < imax + 2; ++i) {
      for (size_t j = 0; j < jmax + 2; ++j) {
        res(i, j).u = value_ * rhs(i, j).u;
        res(i, j).v = value_ * rhs(i, j).v;
      }
    }

    return res;
  }

  operator double() const { return value_; }

  Scalar abs() { return Scalar(std::abs(value_)); }
};

}  // namespace nsfd

#endif
