/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_VECTOR_HPP_
#define NSFD_VECTOR_HPP_

#include <cmath>
#include <tuple>

#include "scalar.hpp"

namespace nsfd {

struct Vector {
  double x;
  double y;

  Vector() : x{0}, y{0} {}
  Vector(double x, double y) : x{x}, y{y} {}
  Vector(std::tuple<double, double> U) : x{std::get<0>(U)}, y{std::get<1>(U)} {}

  /* assignment */
  Vector& operator=(double rhs) {
    this->x = rhs;
    this->y = rhs;
    return *this;
  }

  Vector& operator=(std::tuple<double, double> rhs) {
    this->x = std::get<0>(rhs);
    this->y = std::get<1>(rhs);
    return *this;
  }

  /* addition */
  Vector operator+(const Vector& r) const {
    return {this->x + r.x, this->y + r.y};
  }

  Vector& operator+(double r) {
    this->x += r;
    this->y += r;
    return *this;
  }

  Vector operator+(const Scalar& r) const { return {this->x + r, this->y + r}; }

  friend Vector operator+(const Scalar& l, const Vector& r) {
    return {r.x + l, r.y + l};
  }

  Vector& operator+=(const Vector& r) {
    this->x += r.x;
    this->y += r.y;
    return *this;
  }

  /* subtraction */
  Vector operator-(const Vector& r) const {
    return {this->x - r.x, this->y - r.y};
  }

  /* multiplication */
  friend Vector operator*(const Scalar& l, const Vector& r) {
    return {r.x * l, r.y * l};
  }

  friend Vector operator*(double l, const Vector& r) {
    return {l * r.x, l * r.y};
  }

  /* division */
  Vector operator/(const Scalar& r) const { return {this->x / r, this->y / r}; }

  Vector operator/(const double r) const { return {this->x / r, this->y / r}; }

  double abs() { return std::sqrt(this->x * this->x + this->y * this->y); }
};
}  // namespace nsfd

#endif
