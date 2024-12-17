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
  field::Vector operator+(const field::Vector &r) const {
    auto [imax, jmax] = r.n_interior();
    field::Vector res(imax, jmax);

    for (size_t i = 0; i < imax + 2; ++i) {
      for (size_t j = 0; j < jmax + 2; ++j) {
        res(i, j).u = value_ + r(i, j).u;
        res(i, j).v = value_ + r(i, j).v;
      }
    }

    return res;
  }

  Scalar operator+(const Scalar &other) const {
    return Scalar(this->value_ + other.value_);
  }

  /* subtraction */
  Scalar operator-(const Scalar &other) const {
    return Scalar(this->value_ - other.value_);
  }

  Scalar operator-(double r) const { return Scalar(this->value_ - r); }

  friend Scalar operator-(double l, const Scalar &r) {
    return Scalar(l - r.value_);
  }

  /* multiplication */
  Scalar operator*(const Scalar &other) const {
    return Scalar(this->value_ * other.value_);
  }

  Scalar operator*(double r) const { return Scalar(this->value_ * r); }

  friend Scalar operator*(double l, const Scalar &r) {
    return Scalar(l * r.value_);
  }

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

  /* division */
  Scalar operator/(const Scalar &other) const {
    return Scalar(this->value_ / other.value_);
  }

  Scalar operator/(double r) const { return Scalar(this->value_ / r); }

  friend Scalar operator/(double l, const Scalar &r) {
    return Scalar(l / r.value_);
  }

  operator double() const { return value_; }

  Scalar abs() { return Scalar(std::abs(value_)); }
};

}  // namespace nsfd

#endif
