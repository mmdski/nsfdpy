/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_FIELD_SCALAR_HPP_
#define NSFD_FIELD_SCALAR_HPP_

#include "../scalar.hpp"
#include "field.hpp"

namespace nsfd {
namespace field {
class Scalar : public Field<nsfd::Scalar, Scalar> {
 public:
  using Field<nsfd::Scalar, Scalar>::Field;
  Scalar(size_t imax, size_t jmax, double inital_value)
      : Field(imax, jmax, inital_value) {}
};
}  // namespace field
}  // namespace nsfd

#endif
