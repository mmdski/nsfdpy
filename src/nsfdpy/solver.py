# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
from copy import deepcopy
from typing import cast

import numpy as np
import numpy.typing as npt

from nsfdpy.field import ScalarField, VectorField
from nsfdpy.ops import VectorAdvection, VectorLaplacian
from nsfdpy.grid import StaggeredGrid


class CompFG:

    def __init__(
        self, grid: StaggeredGrid, g: tuple[float, float], Re: float, gamma: float
    ):

        self._grid = grid
        self._g = g
        self._Re = Re

        self._lap = VectorLaplacian(self._grid)
        self._adv = VectorAdvection(self._grid, gamma)

    def __call__(self, u: VectorField, delt: float) -> VectorField:

        FG = u + delt * (self._g + 1 / self._Re * (self._lap(u) - self._adv(u, u)))

        for j in range(1, self._grid.jmax + 1):
            FG[0, j].u = u[0, j].u
            FG[self._grid.imax, j].u = u[self._grid.imax, j].u

        for i in range(1, self._grid.imax + 1):
            FG[i, 0].v = u[i, 0].v
            FG[i, self._grid.jmax].v = u[i, self._grid.jmax].v

        return FG


class CompRHS:

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(
        self,
        FG: VectorField,
        delt: float,
        output: npt.NDArray[np.float64] | None = None,
    ) -> npt.NDArray[np.float64]:

        imax = self._grid.imax
        jmax = self._grid.jmax

        if output is None:
            output = np.zeros((imax + 2, jmax + 2), dtype=np.float64)

        for i in range(1, imax + 1):
            for j in range(1, jmax + 1):
                output[i, j] = (
                    1
                    / delt
                    * (
                        (FG[i, j].u - FG[i - 1, j].u) / self._grid.delx
                        + (FG[i, j].v - FG[i, j - 1].v) / self._grid.dely
                    )
                )

        return output


class IterPressure:

    def __init__(self, grid: StaggeredGrid, omg: float, itermax: int, eps: float):

        self._grid = grid
        self._omg = omg
        self._itermax = itermax
        self._eps = eps

        self._rit = np.zeros((grid.imax + 2, grid.jmax + 2))

        self._n_it = 0

    def __call__(
        self, p: ScalarField, rhs: npt.NDArray[np.float64], overwrite: bool = True
    ) -> ScalarField:

        if not overwrite:
            pit = deepcopy(p)
        else:
            pit = p

        for it in range(1, self._itermax + 1):
            self._set_bc(pit)
            for i in range(1, self._grid.imax + 1):
                for j in range(1, self._grid.jmax + 1):
                    pit[i, j] = (1 - self._omg) * pit[i, j] + self._omg / (
                        2 / self._grid.delx**2 + 2 / self._grid.dely**2
                    ) * (
                        (pit[i + 1, j] + pit[i - 1, j]) / self._grid.delx**2
                        + (pit[i, j + 1] + pit[i, j - 1]) / self._grid.dely**2
                        - rhs[i, j]
                    )

            norm = self._calc_norm(pit, rhs)

            if norm < self._eps:
                self._n_it = it
                return pit

        self._n_it = it
        return pit

    def _calc_norm(self, pit: ScalarField, rhs: npt.NDArray[np.float64]) -> float:

        rit = self._calc_rit(pit, rhs)

        s = 0

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax + 1):
                s += rit[i, j] ** 2

        l2_norm = cast(float, np.sqrt(s))

        return l2_norm

    def _calc_rit(
        self, pit: ScalarField, rhs: npt.NDArray[np.float64]
    ) -> npt.NDArray[np.float64]:

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax + 1):
                self._rit[i, j] = (
                    (pit[i + 1, j] - 2 * pit[i, j] + pit[i - 1, j]) / self._grid.delx**2
                    + (pit[i, j + 1] - 2 * pit[i, j] + pit[i, j - 1])
                    / self._grid.dely**2
                    - rhs[i, j]
                )

        return self._rit

    def _set_bc(self, pit: ScalarField) -> None:

        for i in range(1, self._grid.imax + 1):
            pit[i, 0] = pit[i, 1]
            pit[i, self._grid.jmax + 1] = pit[i, self._grid.jmax]

        for j in range(1, self._grid.jmax + 1):
            pit[0, j] = pit[1, j]
            pit[self._grid.imax + 1, j] = pit[self._grid.imax, j]

    @property
    def n_it(self) -> int:
        return self._n_it
