/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_FIELD_VECTOR_HPP_
#define NSFD_FIELD_VECTOR_HPP_

#include <cmath>
#include <tuple>

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

  std::tuple<double, double> max_abs_components() {
    nsfd::Vector u = {0, 0};
    double u_abs = 0, v_abs = 0;
    double u_max = 0, v_max = 0;
    auto [imax, jmax] = this->n_interior();
    for (size_t i = 1; i <= imax; ++i) {
      for (size_t j = 1; j <= jmax; ++j) {
        u = this->operator()(i, j);
        u_abs = std::abs(u.u);
        v_abs = std::abs(u.v);
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
