# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
from nsfdpy.field import VectorField
from nsfdpy.grid import StaggeredGrid


class CompDelT:

    def __init__(
        self, grid: StaggeredGrid, delt: float, Re: float, tau: float | None = None
    ):
        self._grid = grid
        self._Re = Re
        self._delt = delt
        self._tau = tau

    def __call__(self, u: VectorField) -> float:

        if (self._tau is None) or (self._tau < 0):
            return self._delt

        ts1 = (self._Re / 2) / ((1 / self._grid.delx**2) + (1 / self._grid.dely**2))

        u_abs_max, v_abs_max = u.max_abs_components()

        if u_abs_max == 0:
            cfl_x = ts1
        else:
            cfl_x = self._grid.delx / u_abs_max

        if v_abs_max == 0:
            cfl_y = ts1
        else:
            cfl_y = self._grid.dely / v_abs_max

        delt = self._tau * min(ts1, cfl_x, cfl_y)

        return delt
