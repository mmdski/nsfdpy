/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_SCALAR_HPP_
#define NSFD_SCALAR_HPP_

#include <cmath>

namespace nsfd {
class Scalar {
 private:
  double value_;

 public:
  Scalar() : value_{0} {}
  Scalar(double value) : value_{value} {}

  /* addition */
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
