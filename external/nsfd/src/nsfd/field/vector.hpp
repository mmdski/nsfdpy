/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_FIELD_VECTOR_HPP_
#define NSFD_FIELD_VECTOR_HPP_

#include <cmath>
#include <tuple>

#include "../scalar.hpp"
#include "../vector.hpp"
#include "field.hpp"

namespace nsfd {
struct Vector;
namespace field {
class Vector : public Field<nsfd::Vector, Vector> {
 public:
  using Field<nsfd::Vector, Vector>::Field;
  Vector(size_t imax, size_t jmax, std::tuple<double, double> initial_value)
      : Field(imax, jmax, initial_value) {}

  /* addition */
  friend Vector operator+(const nsfd::Scalar &l, const Vector &r) {
    auto [imax, jmax] = r.n_interior();
    Vector res(imax, jmax);
    for (size_t i = 0; i < imax + 2; ++i) {
      for (size_t j = 0; j < imax + 2; ++j) {
        res(i, j) = l + r(i, j);
      }
    }

    return res;
  }

  friend Vector operator+(const nsfd::Vector &l, const Vector &r) {
    auto [imax, jmax] = r.n_interior();
    Vector res(imax, jmax);
    for (size_t i = 0; i < imax + 2; ++i) {
      for (size_t j = 0; j < imax + 2; ++j) {
        res(i, j) = l + r(i, j);
      }
    }

    return res;
  }

  /* multiplication */
  friend Vector operator*(const nsfd::Scalar &l, const Vector &r) {
    auto [imax, jmax] = r.n_interior();
    Vector res(imax, jmax);
    for (size_t i = 0; i < imax + 2; ++i) {
      for (size_t j = 0; j < imax + 2; ++j) {
        res(i, j) = l * r(i, j);
      }
    }

    return res;
  }

  std::tuple<double, double> max_abs_components() {
    nsfd::Vector u = {0, 0};
    double u_abs = 0, v_abs = 0;
    double u_max = 0, v_max = 0;
    auto [imax, jmax] = this->n_interior();
    for (size_t i = 1; i <= imax; ++i) {
      for (size_t j = 1; j <= jmax; ++j) {
        u = this->operator()(i, j);
        u_abs = std::abs(u.x);
        v_abs = std::abs(u.y);
        if (u_abs >= u_max) u_max = u_abs;
        if (v_abs >= v_max) v_max = v_abs;
      }
    }
    return {u_max, v_max};
  }
};
}  // namespace field
}  // namespace nsfd

#endif
