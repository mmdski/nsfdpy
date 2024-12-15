/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_GRID_AXIS_HPP_
#define NSFD_GRID_AXIS_HPP_

#include <cstddef>

namespace nsfd {
namespace grid {
class Axis {
 private:
  double length_;
  double frac_;
  double del_;

 public:
  size_t max;

  Axis() : length_{0}, frac_{0}, del_{0}, max{0} {};
  Axis(double length, size_t max, double frac)
      : length_{length}, frac_{frac}, del_{length / max}, max{max} {};
  Axis(double length, size_t max)
      : length_{length}, frac_{0.5}, del_{length / max}, max{max} {};

  double operator[](size_t i) const {
    return del_ * (static_cast<double>(i) - frac_);
  }

  double del() { return del_; }
  double length() { return length_; };
};
}  // namespace grid
}  // namespace nsfd

#endif
