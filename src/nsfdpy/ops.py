# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
from typing import cast

from nsfdpy._nsfd.ops import (
    Advection as VectorAdvection,
    Gradient as ScalarGradient,
    VectorLaplace,
)

from nsfdpy.grid import StaggeredGrid
from nsfdpy.field import VectorField


class Curl:

    def __init__(self, grid: StaggeredGrid, field: VectorField):

        self._grid = grid
        self._field = field

    def __call__(self, i: int, j: int) -> float:

        dx = (self._field[i + 1, j].y - self._field[i - 1, j].y) / (2 * self._grid.delx)
        dy = (self._field[i, j + 1].x - self._field[i, j - 1].x) / (2 * self._grid.delx)

        return cast(float, dx - dy)


__all__ = ["ScalarGradient", "VectorAdvection", "VectorLaplace"]
