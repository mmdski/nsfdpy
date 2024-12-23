# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
import numpy as np

from typing import cast

from nsfdpy.field import ScalarField
from nsfdpy.grid import StaggeredGrid


class IterPressure:

    def __init__(self, grid: StaggeredGrid, omg: float, itermax: int, eps: float):

        self._grid = grid
        self._omg = omg
        self._itermax = itermax
        self._eps = eps

        self._rit = ScalarField(grid.imax, grid.jmax)

        self._n_it = 0

    def __call__(
        self, p: ScalarField, rhs: ScalarField, overwrite: bool = True
    ) -> ScalarField:

        if not overwrite:
            pit = ScalarField(p)
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

    def _calc_norm(self, pit: ScalarField, rhs: ScalarField) -> float:

        rit = self._calc_rit(pit, rhs)

        s = 0.0

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax + 1):
                s += float(rit[i, j]) ** 2

        l2_norm = cast(float, np.sqrt(s))

        return l2_norm

    def _calc_rit(self, pit: ScalarField, rhs: ScalarField) -> ScalarField:

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
