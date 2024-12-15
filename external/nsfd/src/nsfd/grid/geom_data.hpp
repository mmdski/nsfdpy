/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef NSFD_GRID_GEOM_DATA_H_
#define NSFD_GRID_GEOM_DATA_H_

#include <cstddef>

namespace nsfd {
namespace grid {
struct GeometryData {
  double xlength;
  double ylength;
  size_t imax;
  size_t jmax;
};
}  // namespace grid
}  // namespace nsfd

#endif
