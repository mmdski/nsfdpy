# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
import numpy.typing as npt
import numpy as np

from nsfdpy.field import ScalarField, VectorField
from nsfdpy.grid import StaggeredGrid


class ScalarGradient:

    def __init__(self, grid: StaggeredGrid):
        self._grid = grid

    def __call__(
        self, p: ScalarField, output: VectorField | None = None
    ) -> VectorField:

        if output is None:
            output = VectorField(self._grid.imax, self._grid.jmax)

        for i in range(1, self._grid.imax):
            for j in range(1, self._grid.jmax + 1):
                output[i, j].u = (p[i + 1, j] - p[i, j]) / self._grid.delx

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax):
                output[i, j].v = (p[i, j - 1] - p[i, j]) / self._grid.dely

        return output


class VectorLaplacian:

    def __init__(self, grid: StaggeredGrid):
        self._grid = grid

    def __call__(
        self, u: VectorField, output: VectorField | None = None
    ) -> VectorField:

        if output is None:
            output = VectorField(self._grid.imax, self._grid.jmax)

        for i in range(1, self._grid.imax):
            for j in range(1, self._grid.jmax + 1):

                d2udx2 = (
                    u[i + 1, j].u - 2 * u[i, j].u + u[i - 1, j].u
                ) / self._grid.delx**2
                d2udy2 = (
                    u[i, j + 1].u - 2 * u[i, j].u + u[i, j - 1].u
                ) / self._grid.dely**2

                output[i, j].u = d2udx2 + d2udy2

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax):

                d2vdx2 = (
                    u[i + 1, j].v - 2 * u[i, j].v + u[i - 1, j].v
                ) / self._grid.delx**2
                d2vdy2 = (
                    u[i, j + 1].v - 2 * u[i, j].v + u[i, j - 1].v
                ) / self._grid.dely**2

                output[i, j].v = d2vdx2 + d2vdy2

        return output


class VectorAdvection:

    def __init__(self, grid: StaggeredGrid, gamma: float):

        self._grid = grid
        self._gamma = gamma

    def __call__(
        self,
        a: VectorField,
        u: VectorField,
        output: VectorField | None = None,
    ) -> VectorField:

        if output is None:
            output = VectorField(self._grid.imax, self._grid.jmax)

        self._x_component(a, u, output)
        self._y_component(a, u, output)

        return output

    def _x_component(self, a: VectorField, u: VectorField, output: VectorField) -> None:

        for i in range(1, self._grid.imax):
            for j in range(1, self._grid.jmax + 1):

                kr_u = (u[i, j].u + u[i + 1, j].u) / 2
                kl_u = (u[i - 1, j].u + u[i, j].u) / 2

                du2dx = (
                    1
                    / self._grid.delx
                    * (
                        (
                            kr_u * (a[i, j].u + a[i + 1, j].u) / 2
                            - kl_u * (a[i - 1, j].u + a[i, j].u) / 2
                        )
                        + self._gamma
                        * (
                            np.abs(kr_u) * (a[i, j].u - a[i + 1, j].u) / 2
                            - np.abs(kl_u) * (a[i - 1, j].u - a[i, j].u) / 2
                        )
                    )
                )

                kr_v = (u[i, j].v + u[i + 1, j].v) / 2
                kl_v = (u[i, j - 1].v + u[i + 1, j - 1].v) / 2

                duvdy = (
                    1
                    / self._grid.dely
                    * (
                        (
                            kr_v * (a[i, j].u + a[i, j + 1].u) / 2
                            - kl_v * (a[i, j - 1].u + a[i, j].u) / 2
                        )
                        + self._gamma
                        * (
                            np.abs(kr_v) * (a[i, j].u - a[i, j + 1].u) / 2
                            - np.abs(kl_v) * (a[i, j - 1].u - a[i, j].u) / 2
                        )
                    )
                )

                output.u[i, j] = du2dx + duvdy

    def _y_component(self, a: VectorField, u: VectorField, output: VectorField) -> None:

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax):

                kr_u = (u[i, j].u + u[i, j + 1].u) / 2
                kl_u = (u[i - 1, j].u + u[i - 1, j + 1].u) / 2

                duvdx = (
                    1
                    / self._grid.delx
                    * (
                        (
                            kr_u * (a[i, j].v + a[i + 1, j].v) / 2
                            - kl_u * (a[i - 1, j].v + a[i, j].v) / 2
                        )
                        + self._gamma
                        * (
                            np.abs(kr_u) * (a[i, j].v - a[i + 1, j].v) / 2
                            - np.abs(kl_u) * (a[i - 1, j].v - a[i, j].v) / 2
                        )
                    )
                )

                kr_v = (u[i, j].v + u[i, j + 1].v) / 2
                kl_v = (u[i, j - 1].v + u[i, j].v) / 2

                dv2dy = (
                    1
                    / self._grid.dely
                    * (
                        (
                            kr_v * (a[i, j].v + a[i, j + 1].v) / 2
                            - kl_v * (a[i, j - 1].v + a[i, j].v) / 2
                        )
                        + self._gamma
                        * (
                            np.abs(kr_v) * (a[i, j].v - a[i, j + 1].v) / 2
                            - np.abs(kl_v) * (a[i, j - 1].v - a[i, j].v) / 2
                        )
                    )
                )

                output[i, j].v = duvdx + dv2dy
