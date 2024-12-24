# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
from abc import ABC, abstractmethod
from typing import Any

from nsfdpy.grid import StaggeredGrid
from nsfdpy.field import ScalarField, VectorField


class BCond(ABC):

    @abstractmethod
    def __call__(self, u: VectorField, p: ScalarField) -> None:
        pass


class NoSlipWBCond(BCond):

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for j in range(1, self._grid.jmax + 1):
            u[0, j].x = 0
            u[0, j].y = -u[1, j].y


class PeriodicWBCond(BCond):

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for j in range(1, self._grid.jmax + 1):
            u[0, j] = u[self._grid.imax - 1, j]
            u[1, j].y = u[self._grid.imax, j].y
            p[1, j] = p[self._grid.imax, j]


class NoSlipEBCond(BCond):

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for j in range(1, self._grid.jmax + 1):
            u[self._grid.imax, j].x = 0
            u[self._grid.imax + 1, j].y = -u[self._grid.imax, j].y


class PeriodicEBCond(BCond):

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for j in range(1, self._grid.jmax + 1):
            u[self._grid.imax, j].x = u[1, j].x
            u[self._grid.imax + 1, j].y = u[2, j].y


class NoSlipNBCond(BCond):

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for i in range(1, self._grid.imax + 1):
            u[i, self._grid.jmax + 1].x = -u[i, self._grid.jmax].x
            u[i, self._grid.jmax].y = 0.0


class MovingWallNBCond(BCond):

    def __init__(self, grid: StaggeredGrid, wall_u: float):

        self._grid = grid
        self._u = wall_u

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for i in range(1, self._grid.imax + 1):
            u[i, self._grid.jmax + 1].x = 2.0 * self._u - u[i, self._grid.jmax].x
            u[i, self._grid.jmax].y = 0.0


class NoSlipSBCond(BCond):

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for i in range(1, self._grid.imax + 1):
            u[i, 0].x = -u[i, 1].x
            u[i, 0].y = 0.0


class ApplyBCond:

    def __init__(self, wW: BCond, wE: BCond, wN: BCond, wS: BCond):

        self._w_bcond = wW
        self._e_bcond = wE
        self._n_bcond = wN
        self._s_bcond = wS

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        self._w_bcond(u, p)
        self._e_bcond(u, p)
        self._n_bcond(u, p)
        self._s_bcond(u, p)
