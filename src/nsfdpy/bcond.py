from typing import Any

from nsfdpy.grid import StaggeredGrid
from nsfdpy.field import ScalarField, VectorField


class NoSlipWBCond:

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, *args: list[Any]) -> None:

        for j in range(1, self._grid.jmax + 1):
            u.u[0, j] = 0
            u.v[0, j] = -u.v[1, j]


class NoSlipEBCond:

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, *args: list[Any]) -> None:

        for j in range(1, self._grid.jmax + 1):
            u.u[self._grid.imax, j] = 0
            u.v[self._grid.imax + 1, j] = -u.v[self._grid.imax, j]


class NoSlipNBCond:

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, *args: list[Any]) -> None:

        for i in range(1, self._grid.imax + 1):
            u.u[i, self._grid.jmax + 1] = -u.u[i, self._grid.jmax]
            u.v[i, self._grid.jmax] = 0.0


class NoSlipSBCond:

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, *args: list[Any]) -> None:

        for i in range(1, self._grid.imax + 1):
            u.u[i, 0] = -u.u[i, 1]
            u.v[i, 0] = 0.0
