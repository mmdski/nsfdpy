# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
from typing import cast

import numpy as np

from nsfdpy import Vector
from nsfdpy.field import ScalarField, VectorField
from nsfdpy.grid import StaggeredGrid


class ScalarGradient:

    def __init__(self, grid: StaggeredGrid, p: ScalarField):
        self._grid = grid
        self._p = p

    def __call__(self, i: int, j: int) -> Vector:

        x = (self._p[i + 1, j] - self._p[i, j]) / self._grid.delx

        y = (self._p[i, j + 1] - self._p[i, j]) / self._grid.dely

        return Vector(x, y)


class VectorLaplacian:

    def __init__(self, grid: StaggeredGrid, u: VectorField):
        self._grid = grid
        self._u = u

    def __call__(self, i: int, j: int) -> Vector:

        d2udx2 = (
            self._u[i + 1, j].x - 2 * self._u[i, j].x + self._u[i - 1, j].x
        ) / self._grid.delx**2
        d2udy2 = (
            self._u[i, j + 1].x - 2 * self._u[i, j].x + self._u[i, j - 1].x
        ) / self._grid.dely**2

        d2vdx2 = (
            self._u[i + 1, j].y - 2 * self._u[i, j].y + self._u[i - 1, j].y
        ) / self._grid.delx**2
        d2vdy2 = (
            self._u[i, j + 1].y - 2 * self._u[i, j].y + self._u[i, j - 1].y
        ) / self._grid.dely**2

        return Vector(d2udx2 + d2udy2, d2vdx2 + d2vdy2)


class VectorAdvection:

    def __init__(
        self, grid: StaggeredGrid, gamma: float, a: VectorField, u: VectorField
    ):

        self._grid = grid
        self._gamma = gamma
        self._a = a
        self._u = u

    def __call__(self, i: int, j: int) -> Vector:

        return Vector(self._x_component(i, j), self._y_component(i, j))

    def _x_component(self, i: int, j: int) -> float:

        kr_u = (self._u[i, j].x + self._u[i + 1, j].x) / 2
        kl_u = (self._u[i - 1, j].x + self._u[i, j].x) / 2

        du2dx = (
            1
            / self._grid.delx
            * (
                (
                    kr_u * (self._a[i, j].x + self._a[i + 1, j].x) / 2
                    - kl_u * (self._a[i - 1, j].x + self._a[i, j].x) / 2
                )
                + self._gamma
                * (
                    np.abs(kr_u) * (self._a[i, j].x - self._a[i + 1, j].x) / 2
                    - np.abs(kl_u) * (self._a[i - 1, j].x - self._a[i, j].x) / 2
                )
            )
        )

        kr_v = (self._u[i, j].y + self._u[i + 1, j].y) / 2
        kl_v = (self._u[i, j - 1].y + self._u[i + 1, j - 1].y) / 2

        duvdy = (
            1
            / self._grid.dely
            * (
                (
                    kr_v * (self._a[i, j].x + self._a[i, j + 1].x) / 2
                    - kl_v * (self._a[i, j - 1].x + self._a[i, j].x) / 2
                )
                + self._gamma
                * (
                    np.abs(kr_v) * (self._a[i, j].x - self._a[i, j + 1].x) / 2
                    - np.abs(kl_v) * (self._a[i, j - 1].x - self._a[i, j].x) / 2
                )
            )
        )

        return cast(float, du2dx + duvdy)

    def _y_component(self, i: int, j: int) -> float:

        kr_u = (self._u[i, j].x + self._u[i, j + 1].x) / 2
        kl_u = (self._u[i - 1, j].x + self._u[i - 1, j + 1].x) / 2

        duvdx = (
            1
            / self._grid.delx
            * (
                (
                    kr_u * (self._a[i, j].y + self._a[i + 1, j].y) / 2
                    - kl_u * (self._a[i - 1, j].y + self._a[i, j].y) / 2
                )
                + self._gamma
                * (
                    np.abs(kr_u) * (self._a[i, j].y - self._a[i + 1, j].y) / 2
                    - np.abs(kl_u) * (self._a[i - 1, j].y - self._a[i, j].y) / 2
                )
            )
        )

        kr_v = (self._u[i, j].y + self._u[i, j + 1].y) / 2
        kl_v = (self._u[i, j - 1].y + self._u[i, j].y) / 2

        dv2dy = (
            1
            / self._grid.dely
            * (
                (
                    kr_v * (self._a[i, j].y + self._a[i, j + 1].y) / 2
                    - kl_v * (self._a[i, j - 1].y + self._a[i, j].y) / 2
                )
                + self._gamma
                * (
                    np.abs(kr_v) * (self._a[i, j].y - self._a[i, j + 1].y) / 2
                    - np.abs(kl_v) * (self._a[i, j - 1].y - self._a[i, j].y) / 2
                )
            )
        )

        return cast(float, duvdx + dv2dy)
