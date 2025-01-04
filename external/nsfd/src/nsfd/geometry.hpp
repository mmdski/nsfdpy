/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_GEOM_GEOMETRY_HPP_
#define NSFD_GEOM_GEOMETRY_HPP_

#include <string>
#include <utility>
#include <vector>

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

  std::vector<std::pair<size_t, size_t>> boundary_cells() { return boundary_; }
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
  std::vector<std::pair<size_t, size_t>> boundary_;
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
          case Cell::Type::Obstacle:
            if (is_interior_boundary(i, j))
              boundary_.emplace_back(std::make_pair(i, j));
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
    if (operator()(i, j).type == Cell::Type::Fluid) return true;

    // admit all domain boundary cells
    if (i == 0 || i == imax_ + 1 || j == 0 || j == jmax_ + 1) return true;

    // must not be a single layer in either direction
    return !((operator()(i + 1, j).type == Cell::Type::Fluid &&
              operator()(i - 1, j).type == Cell::Type::Fluid) ||
             (operator()(i, j + 1).type == Cell::Type::Fluid &&
              operator()(i, j - 1).type == Cell::Type::Fluid));

    return true;
  }

  bool is_interior_boundary(size_t i, size_t j) {
    // checked after admissible cells are checked
    return (operator()(i - 1, j).type == Cell::Type::Fluid) ||
           (operator()(i + 1, j).type == Cell::Type::Fluid) ||
           (operator()(i, j + 1).type == Cell::Type::Fluid) ||
           (operator()(i, j - 1).type == Cell::Type::Fluid);
  }
};
}  // namespace nsfd

#endif
