# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
import numpy as np
import numpy.typing as npt

from nsfdpy import Vector
from nsfdpy.field import ScalarField, VectorField
from nsfdpy.ops import ScalarGradient
from nsfdpy.grid import StaggeredGrid

from nsfdpy._nsfd.comp import FG as CompFG


class CompRHS:

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def __call__(
        self,
        FG: VectorField,
        delt: float,
        output: ScalarField | None = None,
    ) -> ScalarField:

        imax = self._grid.imax
        jmax = self._grid.jmax

        if output is None:
            output = ScalarField(imax, jmax)

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


__all__ = ["CompFG"]
