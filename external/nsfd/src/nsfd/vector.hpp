/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_VECTOR_HPP_
#define NSFD_VECTOR_HPP_

#include <cmath>
#include <tuple>

namespace nsfd {

namespace field {
class Vector;
}

struct Vector {
  double u;
  double v;

  Vector() : u{0}, v{0} {}
  Vector(double u, double v) : u{u}, v{v} {}
  Vector(std::tuple<double, double> U) : u{std::get<0>(U)}, v{std::get<1>(U)} {}

  /* assignment */
  Vector& operator=(double rhs) {
    this->u = rhs;
    this->v = rhs;
    return *this;
  }

  Vector& operator=(std::tuple<double, double> rhs) {
    this->u = std::get<0>(rhs);
    this->v = std::get<1>(rhs);
    return *this;
  }

  /* addition */
  Vector operator+(const Vector& rhs) const {
    return {this->u + rhs.u, this->v + rhs.v};
  }

  Vector& operator+(double rhs) {
    this->u += rhs;
    this->v += rhs;
    return *this;
  }

  Vector& operator+=(const Vector& rhs) {
    this->u += rhs.u;
    this->v += rhs.v;
    return *this;
  }

  /* subtraction */
  Vector operator-(const Vector& rhs) const {
    return {this->u - rhs.u, this->v - rhs.v};
  }

  double abs() { return std::sqrt(this->u * this->u + this->v * this->v); }
};
}  // namespace nsfd

#endif
