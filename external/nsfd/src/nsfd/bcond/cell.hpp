/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_BCOND_CELL_HPP_
#define NSFD_BCOND_CELL_HPP_

#include <cstddef>

#include "../field.hpp"
#include "../scalar.hpp"
#include "../vector.hpp"
#include "data.hpp"

namespace nsfd {
namespace bcond {
class Cell {
 public:
  Cell(size_t i, size_t j, Direction direction)
      : i_{i}, j_{j}, direction_{direction} {}

  void set_fg(nsfd::Field<nsfd::Vector> &u, nsfd::Field<nsfd::Vector> &fg) {
    switch (direction_) {
      case Direction::North:
        fg(i_, j_).y = u(i_, j_).y;
        break;
      case Direction::South:
        fg(i_, j_ - 1).y = u(i_, j_ - 1).y;
        break;
      case Direction::East:
        fg(i_, j_).x = u(i_, j_).x;
        break;
      case Direction::West:
        fg(i_ - 1, j_).x = u(i_ - 1, j_).x;
        break;
      case Direction::NorthEast:
        fg(i_, j_) = u(i_, j_);
        break;
      case Direction::NorthWest:
        fg(i_ - 1, j_).x = u(i_ - 1, j_).x;
        fg(i_, j_).y = u(i_, j_).y;
        break;
      case Direction::SouthEast:
        fg(i_, j_).x = u(i_, j_).x;
        fg(i_, j_ - 1).y = u(i_, j_ - 1).y;
        break;
      case Direction::SouthWest:
        fg(i_ - 1, j_).x = u(i_ - 1, j_).x;
        fg(i_, j_ - 1).y = u(i_, j_ - 1).y;
        break;
      default:
        break;
    }
  }

  void set_p(nsfd::Field<nsfd::Scalar> &p) {
    switch (direction_) {
      case Direction::North:
        p(i_, j_) = p(i_, j_ + 1);
        break;
      case Direction::South:
        p(i_, j_) = p(i_, j_ - 1);
        break;
      case Direction::East:
        p(i_, j_) = p(i_ + 1, j_);
        break;
      case Direction::West:
        p(i_, j_) = p(i_ - 1, j_);
        break;
      case Direction::NorthEast:
        p(i_, j_) = 0.5 * (p(i_, j_ + 1) + p(i_ + 1, j_));
        break;
      case Direction::NorthWest:
        p(i_, j_) = 0.5 * (p(i_, j_ + 1) + p(i_ - 1, j_));
        break;
      case Direction::SouthEast:
        p(i_, j_) = 0.5 * (p(i_, j_ - 1) + p(i_ + 1, j_));
        break;
      case Direction::SouthWest:
        p(i_, j_) = 0.5 * (p(i_, j_ - 1) + p(i_ - 1, j_));
        break;
      default:
        break;
    }
  }

  void set_u(nsfd::Field<nsfd::Vector> &u) {
    switch (direction_) {
      case Direction::North:
        u(i_, j_).x = -u(i_, j_ + 1).x;
        u(i_, j_).y = 0.0;
        break;
      case Direction::South:
        u(i_, j_).x = -u(i_, j_ - 1).x;
        u(i_, j_ - 1).y = 0.0;
        break;
      case Direction::East:
        u(i_, j_).x = 0.0;
        u(i_, j_).y = -u(i_ + 1, j_).y;
        break;
      case Direction::West:
        u(i_ - 1, j_).x = 0.0;
        u(i_, j_).y = -u(i_ - 1, j_).y;
        break;
      case Direction::NorthEast:
        u(i_, j_).x = 0.0;
        u(i_, j_).y = 0.0;
        break;
      case Direction::NorthWest:
        u(i_ - 1, j_).x = 0.0;
        u(i_, j_).x = -u(i_, j_ + 1).x;
        u(i_, j_).y = 0.0;
        break;
      case Direction::SouthEast:
        u(i_, j_).x = 0.0;
        u(i_, j_).y = -u(i_ + 1, j_).y;
        u(i_, j_ - 1).y = 0.0;
        break;
      case Direction::SouthWest:
        u(i_ - 1, j_).x = 0.0;
        u(i_, j_).y = -u(i_ - 1, j_).y;
        u(i_, j_ - 1).y = 0.0;
        break;
      default:
        break;
    }
  }

 private:
  size_t i_, j_;
  Direction direction_;
};
}  // namespace bcond
}  // namespace nsfd

#endif
