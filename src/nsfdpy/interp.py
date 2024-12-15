# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
from typing import cast

from nsfdpy.grid import StaggeredGrid
from nsfdpy.field import VectorField


class VectorFieldInterp:

    def __init__(self, grid: StaggeredGrid, field: VectorField):

        self._grid = grid
        self._field = field

    def __call__(self, x: float, y: float) -> tuple[float, float]:

        return self._interp_u(x, y), self._interp_v(x, y)

    def _interp_u(self, x: float, y: float) -> float:

        delx = self._grid.delx
        dely = self._grid.dely

        i = int(x / delx) + 1
        j = int((y + dely / 2) / dely) + 1

        x1 = (i - 1) * delx
        x2 = i * delx
        y1 = ((j - 1) - 0.5) * dely
        y2 = (j - 0.5) * dely

        u1 = self._field[i - 1, j - 1].u
        u2 = self._field[i, j - 1].u
        u3 = self._field[i - 1, j].u
        u4 = self._field[i, j].u

        w1 = (x2 - x) * (y2 - y)
        w2 = (x - x1) * (y2 - y)
        w3 = (x2 - x) * (y - y1)
        w4 = (x - x1) * (y - y1)

        u = cast(float, 1 / (delx * dely) * (w1 * u1 + w2 * u2 + w3 * u3 + w4 * u4))

        return u

    def _interp_v(self, x: float, y: float) -> float:

        delx = self._grid.delx
        dely = self._grid.dely

        i = int((x + delx / 2) / delx) + 1
        j = int(y / dely) + 1

        x1 = ((i - 1) - 0.5) * delx
        x2 = (i - 0.5) * delx
        y1 = (j - 1) * dely
        y2 = j * dely

        v1 = self._field[i - 1, j - 1].v
        v2 = self._field[i, j - 1].v
        v3 = self._field[i - 1, j].v
        v4 = self._field[i, j].v

        w1 = (x2 - x) * (y2 - y)
        w2 = (x - x1) * (y2 - y)
        w3 = (x2 - x) * (y - y1)
        w4 = (x - x1) * (y - y1)

        v = cast(float, 1 / (delx * dely) * (w1 * v1 + w2 * v2 + w3 * v3 + w4 * v4))

        return v
