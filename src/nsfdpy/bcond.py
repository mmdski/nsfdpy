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
            u.u[0, j] = 0
            u.v[0, j] = -u.v[1, j]


class NoSlipEBCond(BCond):

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for j in range(1, self._grid.jmax + 1):
            u.u[self._grid.imax, j] = 0
            u.v[self._grid.imax + 1, j] = -u.v[self._grid.imax, j]


class NoSlipNBCond(BCond):

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for i in range(1, self._grid.imax + 1):
            u.u[i, self._grid.jmax + 1] = -u.u[i, self._grid.jmax]
            u.v[i, self._grid.jmax] = 0.0


class MovingWallNBCond(BCond):

    def __init__(self, grid: StaggeredGrid, wall_u: float):

        self._grid = grid
        self._u = wall_u

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for i in range(1, self._grid.imax + 1):
            u.u[i, self._grid.jmax + 1] = 2.0 * self._u - u.u[i, self._grid.jmax]
            u.v[i, self._grid.jmax] = 0.0


class NoSlipSBCond(BCond):

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(self, u: VectorField, p: ScalarField) -> None:

        for i in range(1, self._grid.imax + 1):
            u.u[i, 0] = -u.u[i, 1]
            u.v[i, 0] = 0.0


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
