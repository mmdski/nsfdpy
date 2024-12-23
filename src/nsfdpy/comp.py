# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
import numpy as np
import numpy.typing as npt

from nsfdpy import Vector
from nsfdpy.field import ScalarField, VectorField
from nsfdpy.ops import ScalarGradient, VectorAdvection, VectorLaplacian
from nsfdpy.grid import StaggeredGrid


class CompFG:

    def __init__(
        self, grid: StaggeredGrid, g: tuple[float, float], Re: float, gamma: float
    ):

        self._grid = grid
        self._g = Vector(g)
        self._Re = Re

        self._gamma = gamma

    def __call__(self, u: VectorField, delt: float) -> VectorField:

        lap = VectorLaplacian(self._grid, u)
        adv = VectorAdvection(self._grid, self._gamma, u, u)

        FG = u.new_like()

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax + 1):
                FG[i, j] = u[i, j] + delt * (
                    self._g + 1 / self._Re * (lap(i, j) - adv(i, j))
                )

        for j in range(1, self._grid.jmax + 1):
            FG[0, j].x = u[0, j].x
            FG[self._grid.imax, j].x = u[self._grid.imax, j].x

        for i in range(1, self._grid.imax + 1):
            FG[i, 0].y = u[i, 0].y
            FG[i, self._grid.jmax].y = u[i, self._grid.jmax].y

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
                        (FG[i, j].x - FG[i - 1, j].x) / self._grid.delx
                        + (FG[i, j].y - FG[i, j - 1].y) / self._grid.dely
                    )
                )

        return output


class CompUNextTime:

    def __init__(
        self, grid: StaggeredGrid, fg: VectorField, p: ScalarField, delt: float
    ):

        self._fg = fg
        self._grad_p = ScalarGradient(grid, p)
        self._delt = delt

    def __call__(self, i: int, j: int) -> Vector:

        return self._fg[i, j] - self._delt * self._grad_p(i, j)
