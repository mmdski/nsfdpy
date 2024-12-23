/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_OPS_ADVECTION_HPP_
#define NSFD_OPS_ADVECTION_HPP_

#include <cmath>

#include "../field/vector.hpp"
#include "../grid/staggered_grid.hpp"

namespace nsfd {
namespace ops {
class Advection {
 private:
  nsfd::grid::StaggeredGrid &grid_;
  double gamma_;
  nsfd::field::Vector &a_;
  nsfd::field::Vector &u_;

  double x_component(size_t i, size_t j) {
    double kr_u = (u_(i, j).x + u_(i + 1, j).x) / 2.0;
    double kl_u = (u_(i - 1, j).x + u_(i, j).x) / 2.0;
    double du2dx =
        1.0 / grid_.delx() *
        ((kr_u * (a_(i, j).x + a_(i + 1, j).x) / 2.0 -
          kl_u * (a_(i - 1, j).x + a_(i, j).x) / 2.0) +
         gamma_ * (std::abs(kr_u) * (a_(i, j).x - a_(i + 1, j).x) / 2.0 -
                   std::abs(kl_u) * (a_(i - 1, j).x - a_(i, j).x) / 2.0));

    double kr_v = (u_(i, j).y + u_(i + 1, j).y) / 2.0;
    double kl_v = (u_(i, j - 1).y + u_(i + 1, j - 1).y) / 2.0;

    double duvdy =
        1.0 / grid_.dely() *
        ((kr_v * (a_(i, j).x + a_(i, j + 1).x) / 2.0 -
          kl_v * (a_(i, j - 1).x + a_(i, j).x) / 2.0) +
         gamma_ * (std::abs(kr_v) * (a_(i, j).x - a_(i, j + 1).x) / 2.0 -
                   std::abs(kl_v) * (a_(i, j - 1).x - a_(i, j).x) / 2.0));

    return du2dx + duvdy;
  }

  double y_component(size_t i, size_t j) {
    double kr_u = (u_(i, j).x + u_(i, j + 1).x) / 2.0;
    double kl_u = (u_(i - 1, j).x + u_(i - 1, j + 1).x) / 2.0;

    double duvdx =
        1 / grid_.delx() *
        ((kr_u * (a_(i, j).y + a_(i + 1, j).y) / 2 -
          kl_u * (a_(i - 1, j).y + a_(i, j).y) / 2) +
         gamma_ * (std::abs(kr_u) * (a_(i, j).y - a_(i + 1, j).y) / 2.0 -
                   std::abs(kl_u) * (a_(i - 1, j).y - a_(i, j).y) / 2.0));

    double kr_v = (u_(i, j).y + u_(i, j + 1).y) / 2.0;
    double kl_v = (u_(i, j - 1).y + u_(i, j).y) / 2.0;

    double dv2dy =
        1 / grid_.dely() *
        ((kr_v * (a_(i, j).y + a_(i, j + 1).y) / 2.0 -
          kl_v * (a_(i, j - 1).y + a_(i, j).y) / 2.0) +
         gamma_ * (std::abs(kr_v) * (a_(i, j).y - a_(i, j + 1).y) / 2.0 -
                   std::abs(kl_v) * (a_(i, j - 1).y - a_(i, j).y) / 2.0));

    return duvdx + dv2dy;
  }

 public:
  Advection(nsfd::grid::StaggeredGrid &grid, double gamma,
            nsfd::field::Vector &field, nsfd::field::Vector &velocity)
      : grid_{grid}, gamma_{gamma}, a_{field}, u_{velocity} {}

  nsfd::Vector operator()(size_t i, size_t j) {
    return nsfd::Vector(x_component(i, j), y_component(i, j));
  }
};
}  // namespace ops
}  // namespace nsfd

#endif
