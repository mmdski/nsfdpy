/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_FIELD_FIELD_HPP_
#define NSFD_FIELD_FIELD_HPP_

#include <stdexcept>
#include <tuple>
#include <vector>

#include "grid/staggered_grid.hpp"

namespace nsfd {
template <typename T>
class Field {
 private:
  size_t imax_;
  size_t jmax_;
  std::vector<T> values_;

 public:
  Field() : values_() {}
  Field(size_t imax, size_t jmax)
      : imax_{imax}, jmax_{jmax}, values_(((imax + 2) * (jmax + 2))) {}
  Field(size_t imax, size_t jmax, T initial_value)
      : imax_{imax}, jmax_{jmax}, values_(((imax + 2) * (jmax + 2))) {
    for (auto &u : values_) u = initial_value;
  }
  Field(std::tuple<size_t, size_t> n_interior)
      : imax_{std::get<0>(n_interior)},
        jmax_{std::get<1>(n_interior)},
        values_(
            ((std::get<0>(n_interior) + 2) * (std::get<1>(n_interior) + 2))) {}
  Field(nsfd::grid::StaggeredGrid &grid)
      : imax_{grid.imax()},
        jmax_{grid.jmax()},
        values_((grid.imax() + 2) * (grid.jmax() + 2)) {}
  Field(nsfd::grid::StaggeredGrid &grid, T initial_value)
      : imax_{grid.imax()},
        jmax_{grid.jmax()},
        values_((grid.imax() + 2) * (grid.jmax() + 2)) {
    for (auto &u : values_) u = initial_value;
  }

  T &operator()(size_t i, size_t j) {
    if (i > imax_ + 1) throw std::out_of_range("i is out of range");
    if (j > jmax_ + 1) throw std::out_of_range("j is out of range");
    return values_[i * (jmax_ + 2) + j];
  }
  const T &operator()(size_t i, size_t j) const {
    if (i > imax_ + 1) throw std::out_of_range("i is out of range");
    if (j > jmax_ + 1) throw std::out_of_range("j is out of range");
    return values_[i * (jmax_ + 2) + j];
  }

  void copy(const nsfd::Field<T> &other) {
    for (size_t i = 0; i <= imax_ + 1; ++i) {
      for (size_t j = 0; j <= jmax_ + 1; ++j) {
        this->operator()(i, j) = other(i, j);
      }
    }
  }

  double max_abs() {
    double max_abs = 0;
    double abs = 0;
    for (auto v : values_) {
      abs = v.abs();
      if (abs > max_abs) max_abs = abs;
    }
    return max_abs;
  }

  double res(const nsfd::Field<T> &other) const {
    double sum = 0;
    for (size_t i = 1; i <= imax_; ++i) {
      for (size_t j = 1; j <= jmax_; ++j) {
        sum += this->operator()(i, j).abs() - other(i, j).abs();
      }
    }

    double res = std::sqrt(sum * sum / static_cast<double>(imax_ * jmax_));

    return res;
  }

  std::tuple<size_t, size_t> n_interior() const { return {imax_, jmax_}; }
  std::tuple<size_t, size_t> shape() const { return {imax_ + 2, jmax_ + 2}; }
};

}  // namespace nsfd

#endif
