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
            output = VectorField(self._grid.geom_data)

        for i in range(1, self._grid.imax):
            for j in range(1, self._grid.jmax + 1):
                output.u[i, j] = (p[i + 1, j] - p[i, j]) / self._grid.delx

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax):
                output.v[i, j] = (p[i, j - 1] - p[i, j]) / self._grid.dely

        return output


class VectorLaplacian:

    def __init__(self, grid: StaggeredGrid):
        self._grid = grid

    def __call__(
        self, u: VectorField, output: VectorField | None = None
    ) -> VectorField:

        if output is None:
            output = VectorField(self._grid.geom_data)

        for i in range(1, self._grid.imax):
            for j in range(1, self._grid.jmax + 1):

                d2udx2 = (
                    u.u[i + 1, j] - 2 * u.u[i, j] + u.u[i - 1, j]
                ) / self._grid.delx**2
                d2udy2 = (
                    u.u[i, j + 1] - 2 * u.u[i, j] + u.u[i, j - 1]
                ) / self._grid.dely**2

                output.u[i, j] = d2udx2 + d2udy2

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax):

                d2vdx2 = (
                    u.v[i + 1, j] - 2 * u.v[i, j] + u.v[i - 1, j]
                ) / self._grid.delx**2
                d2vdy2 = (
                    u.v[i, j + 1] - 2 * u.v[i, j] + u.v[i, j - 1]
                ) / self._grid.dely**2

                output.v[i, j] = d2vdx2 + d2vdy2

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
            output = VectorField(self._grid.geom_data)

        self._x_component(a, u, output)
        self._y_component(a, u, output)

        return output

    def _x_component(self, a: VectorField, u: VectorField, output: VectorField) -> None:

        for i in range(1, self._grid.imax):
            for j in range(1, self._grid.jmax + 1):

                kr_u = (u.u[i, j] + u.u[i + 1, j]) / 2
                kl_u = (u.u[i - 1, j] + u.u[i, j]) / 2

                du2dx = (
                    1
                    / self._grid.delx
                    * (
                        (
                            kr_u * (a.u[i, j] + a.u[i + 1, j]) / 2
                            - kl_u * (a.u[i - 1, j] + a.u[i, j]) / 2
                        )
                        + self._gamma
                        * (
                            np.abs(kr_u) * (a.u[i, j] - a.u[i + 1, j]) / 2
                            - np.abs(kl_u) * (a.u[i - 1, j] - a.u[i, j]) / 2
                        )
                    )
                )

                kr_v = (u.v[i, j] + u.v[i + 1, j]) / 2
                kl_v = (u.v[i, j - 1] + u.v[i + 1, j - 1]) / 2

                duvdy = (
                    1
                    / self._grid.dely
                    * (
                        (
                            kr_v * (a.u[i, j] + a.u[i, j + 1]) / 2
                            - kl_v * (a.u[i, j - 1] + a.u[i, j]) / 2
                        )
                        + self._gamma
                        * (
                            np.abs(kr_v) * (a.u[i, j] - a.u[i, j + 1]) / 2
                            - np.abs(kl_v) * (a.u[i, j - 1] - a.u[i, j]) / 2
                        )
                    )
                )

                output.u[i, j] = du2dx + duvdy

    def _y_component(self, a: VectorField, u: VectorField, output: VectorField) -> None:

        for i in range(1, self._grid.imax + 1):
            for j in range(1, self._grid.jmax):

                kr_u = (u.u[i, j] + u.u[i, j + 1]) / 2
                kl_u = (u.u[i - 1, j] + u.u[i - 1, j + 1]) / 2

                duvdx = (
                    1
                    / self._grid.delx
                    * (
                        (
                            kr_u * (a.v[i, j] + a.v[i + 1, j]) / 2
                            - kl_u * (a.v[i - 1, j] + a.v[i, j]) / 2
                        )
                        + self._gamma
                        * (
                            np.abs(kr_u) * (a.v[i, j] - a.v[i + 1, j]) / 2
                            - np.abs(kl_u) * (a.v[i - 1, j] - a.v[i, j]) / 2
                        )
                    )
                )

                kr_v = (u.v[i, j] + u.v[i, j + 1]) / 2
                kl_v = (u.v[i, j - 1] + u.v[i, j]) / 2

                dv2dy = (
                    1
                    / self._grid.dely
                    * (
                        (
                            kr_v * (a.v[i, j] + a.v[i, j + 1]) / 2
                            - kl_v * (a.v[i, j - 1] + a.v[i, j]) / 2
                        )
                        + self._gamma
                        * (
                            np.abs(kr_v) * (a.v[i, j] - a.v[i, j + 1]) / 2
                            - np.abs(kl_v) * (a.v[i, j - 1] - a.v[i, j]) / 2
                        )
                    )
                )

                output.v[i, j] = duvdx + dv2dy
