/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "vector.hpp"

#include "field/vector.hpp"

namespace nsfd {
field::Vector Vector::operator+(const field::Vector& rhs) const {
  auto [imax, jmax] = rhs.n_interior();
  field::Vector res(imax, jmax);

  for (size_t i = 0; i < imax + 2; ++i) {
    for (size_t j = 0; j < imax + 2; ++j) {
      res(i, j).u = this->u + rhs(i, j).u;
      res(i, j).v = this->v + rhs(i, j).v;
    }
  }

  return res;
}

}  // namespace nsfd
