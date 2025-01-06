/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_GEOM_GEOMETRY_HPP_
#define NSFD_GEOM_GEOMETRY_HPP_

#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "bcond/data.hpp"
#include "grid/staggered_grid.hpp"

namespace nsfd {
class Geometry {
 public:
  Geometry(size_t imax, size_t jmax)
      : imax_{imax},
        jmax_{jmax},
        cells_((imax + 2) * (jmax + 2)),
        boundary_{},
        fluid_{} {
    identify_fluid();
  }

  Geometry(nsfd::grid::StaggeredGrid &grid)
      : Geometry(grid.imax(), grid.jmax()) {}

  Geometry(nsfd::grid::StaggeredGrid &grid,
           std::vector<std::pair<size_t, size_t>> obstacle)
      : imax_{grid.imax()},
        jmax_{grid.jmax()},
        cells_((grid.imax() + 2) * (grid.jmax() + 2)),
        boundary_{},
        fluid_{} {
    for (const auto [i, j] : obstacle) {
      operator()(i, j).type = Cell::Type::Obstacle;
    }

    check_admissible();
    set_boundary();
    identify_fluid();
  }

  std::vector<std::pair<size_t, size_t>> boundary_cells() {
    std::vector<std::pair<size_t, size_t>> cells;
    for (auto const &[i, j, direction] : boundary_) {
      cells.emplace_back(std::make_pair(i, j));
    }
    return cells;
  }

  std::vector<std::tuple<size_t, size_t, nsfd::bcond::Direction>>
  boundary_cond() {
    return boundary_;
  }

  std::vector<std::pair<size_t, size_t>> fluid_cells() { return fluid_; }

  std::vector<std::pair<size_t, size_t>> obstacle_cells() {
    std::vector<std::pair<size_t, size_t>> cells;
    for (size_t i = 0; i <= imax_ + 1; ++i) {
      for (size_t j = 0; j <= jmax_ + 1; ++j) {
        if (operator()(i, j).type == Cell::Type::Obstacle) {
          cells.emplace_back(std::make_pair(i, j));
        }
      }
    }
    return cells;
  }

 private:
  struct Cell {
    enum class Type { Fluid, Obstacle };
    Type type = Type::Fluid;
  };

  size_t imax_;
  size_t jmax_;
  std::vector<Cell> cells_;
  std::vector<std::tuple<size_t, size_t, nsfd::bcond::Direction>> boundary_;
  std::vector<std::pair<size_t, size_t>> fluid_;

  Cell &operator()(size_t i, size_t j) {
    if (i > imax_ + 1) throw std::out_of_range("i is out of range");
    if (j > jmax_ + 1) throw std::out_of_range("j is out of range");
    return cells_[i * (jmax_ + 2) + j];
  }

  const Cell &operator()(size_t i, size_t j) const {
    if (i > imax_ + 1) throw std::out_of_range("i is out of range");
    if (j > jmax_ + 1) throw std::out_of_range("j is out of range");
    return cells_[i * (jmax_ + 2) + j];
  }

  void check_admissible() {
    for (size_t i = 0; i <= imax_ + 1; ++i) {
      for (size_t j = 0; j <= jmax_ + 1; ++j) {
        if (!is_admissible(i, j)) {
          throw std::runtime_error("Cell (" + std::to_string(i) + ", " +
                                   std::to_string(j) + ") is not admissible");
        }
      }
    }
  }

  void set_boundary() {
    for (size_t i = 1; i <= imax_; ++i) {
      for (size_t j = 1; j <= jmax_; ++j) {
        switch (operator()(i, j).type) {
          case Cell::Type::Obstacle: {
            nsfd::bcond::Direction direction = boundary_direction(i, j);
            if (direction == nsfd::bcond::Direction::None)
              continue;
            else
              boundary_.emplace_back(std::make_tuple(i, j, direction));
            break;
          }
          default:
            break;
        }
      }
    }
  }

  void identify_fluid() {
    for (size_t i = 1; i <= imax_; ++i) {
      for (size_t j = 1; j <= jmax_; ++j) {
        if (operator()(i, j).type == Cell::Type::Fluid)
          fluid_.emplace_back(std::make_pair(i, j));
      }
    }
  }

  bool is_admissible(size_t i, size_t j) {
    // return true for fluid cells
    if (is_fluid(i, j)) return true;

    // admit all domain boundary cells
    if (i == 0 || i == imax_ + 1 || j == 0 || j == jmax_ + 1) return true;

    // must not be a single layer in either direction
    return !((is_fluid(i + 1, j) && is_fluid(i - 1, j)) ||
             (is_fluid(i, j + 1) && is_fluid(i, j - 1)));

    return true;
  }

  bool is_fluid(size_t i, size_t j) {
    return operator()(i, j).type == Cell::Type::Fluid;
  }

  bool is_interior_boundary(size_t i, size_t j) {
    // checked after admissible cells are checked
    return (is_fluid(i - 1, j) || is_fluid(i + 1, j) || is_fluid(i, j + 1) ||
            is_fluid(i, j - 1));
  }

  nsfd::bcond::Direction boundary_direction(size_t i, size_t j) {
    // assuming admissible cells have been checked
    if (is_fluid(i, j + 1)) {
      if (is_fluid(i + 1, j + 1)) return nsfd::bcond::Direction::NorthEast;
      if (is_fluid(i - 1, j + 1))
        return nsfd::bcond::Direction::NorthWest;
      else
        return nsfd::bcond::Direction::North;
    } else if (is_fluid(i, j - 1)) {
      if (is_fluid(i + 1, j - 1)) return nsfd::bcond::Direction::SouthEast;
      if (is_fluid(i - 1, j - 1))
        return nsfd::bcond::Direction::SouthWest;
      else
        return nsfd::bcond::Direction::South;
    } else if (is_fluid(i + 1, j)) {
      return nsfd::bcond::Direction::East;
    } else if (is_fluid(i - 1, j)) {
      return nsfd::bcond::Direction::West;
    } else
      return nsfd::bcond::Direction::None;
  }
};
}  // namespace nsfd

#endif
