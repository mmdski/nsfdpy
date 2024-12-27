/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_CONFIG_HPP_
#define NSFD_CONFIG_HPP_

#include <cstddef>
#include <optional>

#include "bcond/data.hpp"
#include "vector.hpp"

namespace nsfd {
namespace config {

struct BoundaryCond {
  nsfd::bcond::Data n;
  nsfd::bcond::Data s;
  nsfd::bcond::Data e;
  nsfd::bcond::Data w;

  BoundaryCond(nsfd::bcond::Data n_bc, nsfd::bcond::Data s_bc,
               nsfd::bcond::Data e_bc, nsfd::bcond::Data w_bc)
      : n{n_bc}, s{s_bc}, e{e_bc}, w{w_bc} {}
};

struct Constants {
  double Re;
  double gx;
  double gy;

  Constants(double Re, double gx, double gy) : Re{Re}, gx{gx}, gy{gy} {}
};

struct Geometry {
  size_t imax;
  size_t jmax;
  double xlength;
  double ylength;

  Geometry(size_t imax, size_t jmax, double xlength, double ylength)
      : imax{imax}, jmax{jmax}, xlength{xlength}, ylength{ylength} {}
};

struct InitialCond {
  double PI;
  double UI;
  double VI;

  InitialCond(double PI, double UI, double VI) : PI{PI}, UI{UI}, VI{VI} {}

  nsfd::Scalar p() { return PI; }
  nsfd::Vector u() { return {UI, VI}; }
};

struct Solver {
  double omg;
  int itermax;
  double eps;
  double gamma;

  Solver(double omg, int itermax, double eps, double gamma)
      : omg{omg}, itermax{itermax}, eps{eps}, gamma{gamma} {}
};

struct Time {
  double delt;
  std::optional<double> tau;
  Time(double delt) : delt{delt}, tau{std::nullopt} {}
};

}  // namespace config
}  // namespace nsfd

#endif
