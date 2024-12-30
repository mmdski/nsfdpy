/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_BCOND_DATA_HPP_
#define NSFD_BCOND_DATA_HPP_

#include <optional>

namespace nsfd {
namespace bcond {

enum class Type { NoSlip, Periodic };

struct Data {
  Type type;
  std::optional<double> value;

  Data(Type type) : type{type}, value{std::nullopt} {}
  Data(Type type, double value) : type{type}, value{value} {}
};
}  // namespace bcond
}  // namespace nsfd

#endif
