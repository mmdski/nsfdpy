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

namespace nsfd {
namespace field {
template <typename T, typename D>
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

  /* addition */
  D operator+(const Field<T, D> &rhs) const {
    /* fields must be the same size*/
    if ((this->imax_ != rhs.imax_) || (this->jmax_ != rhs.jmax_))
      throw std::invalid_argument("Size of rhs not equal to size of this");
    D res(imax_, jmax_);
    for (size_t i = 0; i < imax_ + 2; ++i) {
      for (size_t j = 0; j < imax_ + 2; ++j) {
        res(i, j) = (*this)(i, j) + rhs(i, j);
      }
    }
    return res;
  }

  /* subtraction */
  D operator-(const Field<T, D> &rhs) const {
    /* fields must be the same size*/
    if ((this->imax_ != rhs.imax_) || (this->jmax_ != rhs.jmax_))
      throw std::invalid_argument("Size of rhs not equal to size of this");
    D res(imax_, jmax_);
    for (size_t i = 0; i < imax_ + 2; ++i) {
      for (size_t j = 0; j < imax_ + 2; ++j) {
        res(i, j) = (*this)(i, j) - rhs(i, j);
      }
    }
    return res;
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

  double max_abs() {
    double max_abs = 0;
    double abs = 0;
    for (auto v : values_) {
      abs = v.abs();
      if (abs > max_abs) max_abs = abs;
    }
    return max_abs;
  }
  std::tuple<size_t, size_t> n_interior() const { return {imax_, jmax_}; }
  std::tuple<size_t, size_t> shape() const { return {imax_ + 2, jmax_ + 2}; }
};
}  // namespace field
}  // namespace nsfd

#endif
