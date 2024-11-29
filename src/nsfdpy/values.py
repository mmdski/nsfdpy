from typing import Any
from copy import copy, deepcopy
from typing import cast

import matplotlib.pyplot as plt
from matplotlib.axes import Axes
import numpy as np
import numpy.typing as npt

from nsfdpy.grid import grid_lines
from nsfdpy.geom import GridCoordinates
from nsfdpy.typing import BCondition, GeometryData, ProblemData


class ScalarVariable:
    def __init__(self, geom_data: GeometryData, initial_value: float):
        self._geom_data = copy(geom_data)
        self._values: npt.NDArray[np.float64] = np.full(
            (geom_data["imax"] + 2, geom_data["jmax"] + 2),
            initial_value,
            dtype=np.float64,
        )

    def __deepcopy__(self, memo: dict[int, Any]) -> "ScalarVariable":

        cls = self.__class__
        result = cls.__new__(cls)
        memo[id(self)] = result
        for k, v in self.__dict__.items():
            setattr(result, k, deepcopy(v, memo))
        return result

    def __getitem__(self, idx: tuple[int, int]) -> float:

        return cast(float, self._values[idx[0], idx[1]])

    def __setitem__(self, idx: tuple[int, int], value: float) -> None:

        self._values[idx[0], idx[1]] = value

    def abs_max(self) -> np.float64:

        return cast(np.float64, np.absolute(self._values).max())

    def plot(
        self, ax: Axes | None = None, boundary: bool = False, **kwargs: dict[str, Any]
    ) -> Axes:

        if not ax:
            ax = plt.axes()

        v_grid_lines, h_grid_lines = grid_lines(self._geom_data, boundary)

        if boundary:
            ax.pcolormesh(
                v_grid_lines, h_grid_lines, self._values.T, **kwargs  # type: ignore[arg-type]
            )
        else:
            ax.pcolormesh(
                v_grid_lines, h_grid_lines, self._values[1:-1, 1:-1].T, **kwargs  # type: ignore[arg-type]
            )

        return ax


class VectorVariable:

    def __init__(self, geom_data: GeometryData, u: ScalarVariable, v: ScalarVariable):

        self._u = u
        self._v = v
        self._grid_coords = GridCoordinates(geom_data, 0.5, 0.5)

    def __getitem__(self, idx: tuple[int, int]) -> tuple[float, float]:

        i = idx[0]
        j = idx[1]
        u = 0.5 * (self._u[i, j + 1] + self._u[i + 1, j + 1])
        v = 0.5 * (self._v[i + 1, j] + self._v[i + 1, j + 1])
        return u, v

    def plot(self, ax: Axes | None = None, **kwargs: dict[str, Any]) -> Axes:

        if not ax:
            ax = plt.axes()

        imax, jmax = self._grid_coords.dim()

        X = np.empty((imax, jmax))
        Y = np.empty((imax, jmax))
        U = np.empty((imax, jmax))
        V = np.empty((imax, jmax))

        for i in range(imax):
            for j in range(jmax):
                u, v = self[i, j]
                # skip west and south boundaries
                x, y = self._grid_coords[i + 1, j + 1]
                X[i, j] = x
                Y[i, j] = y
                U[i, j] = u
                V[i, j] = v

        ax.quiver(X, Y, U, V, **kwargs)

        return ax


class GridValues:

    def __init__(self, geom_data: GeometryData, prob_data: ProblemData):

        self.p = ScalarVariable(geom_data, prob_data["PI"])
        self.u = ScalarVariable(geom_data, prob_data["UI"])
        self.v = ScalarVariable(geom_data, prob_data["VI"])
        self.uv = VectorVariable(geom_data, self.u, self.v)

        self._geom_data = deepcopy(geom_data)
        self._Re = prob_data["Re"]
        self._delx = geom_data["xlength"] / geom_data["imax"]
        self._dely = geom_data["ylength"] / geom_data["jmax"]
        self._imax = geom_data["imax"]
        self._jmax = geom_data["jmax"]

    def _set_bc_w(self, wW: BCondition) -> None:

        if wW == BCondition.NONE:
            return
        elif wW == BCondition.NO_SLIP:
            for j in range(1, self._jmax + 1):
                self.u[0, j] = 0
                self.v[0, j] = -self.v[1, j]
        elif wW == BCondition.FREE_SLIP:
            for j in range(1, self._jmax + 1):
                self.u[0, j] = 0
                self.v[0, j] = self.v[1, j]
        elif wW == BCondition.OUTFLOW:
            for j in range(1, self._jmax + 1):
                self.u[0, j] = self.u[1, j]
                self.v[0, j] = self.v[1, j]
        elif wW == BCondition.PERIODIC:
            for j in range(1, self._jmax + 1):
                self.u[0, j] = self.u[self._imax - 1, j]
                self.v[0, j] = self.v[self._imax - 1, j]
                self.v[1, j] = self.v[self._imax, j]
                self.p[1, j] = self.p[self._imax, j]
        else:
            raise ValueError(f"Unrecognized BCondition: {wW}")

    def _set_bc_e(self, wE: BCondition) -> None:

        if wE == BCondition.NONE:
            return
        elif wE == BCondition.NO_SLIP:
            for j in range(1, self._jmax + 1):
                self.u[self._imax, j] = 0
                self.v[self._imax + 1, j] = -self.v[self._imax, j]
        elif wE == BCondition.FREE_SLIP:
            for j in range(1, self._jmax + 1):
                self.u[self._imax, j] = 0
                self.v[self._imax + 1, j] = 0
        elif wE == BCondition.OUTFLOW:
            for j in range(1, self._jmax + 1):
                self.u[self._imax, j] = self.u[self._imax - 1, j]
                self.v[self._imax + 1, j] = self.v[self._imax, j]
        elif wE == BCondition.PERIODIC:
            for j in range(1, self._jmax + 1):
                self.u[self._imax, j] = self.u[1, j]
                self.v[self._imax + 1, j] = self.v[2, j]

    def _set_bc_n(self, wN: BCondition) -> None:

        if wN == BCondition.NONE:
            return
        elif wN == BCondition.NO_SLIP:
            for i in range(1, self._imax + 1):
                self.u[i, self._jmax + 1] = -self.u[i, self._jmax]
                self.v[i, self._jmax] = 0.0
        elif wN == BCondition.FREE_SLIP:
            for i in range(1, self._imax + 1):
                self.u[i, self._jmax + 1] = self.u[i, self._jmax]
                self.v[i, self._jmax] = 0.0
        elif wN == BCondition.OUTFLOW:
            for i in range(1, self._imax + 1):
                self.u[i, self._jmax + 1] = self.u[i, self._jmax]
                self.v[i, self._jmax] = self.v[i, self._jmax - 1]
        else:
            raise ValueError(f"Unrecognized BCondition: {wN}")

    def _set_bc_s(self, wS: BCondition) -> None:

        if wS == BCondition.NONE:
            return
        elif wS == BCondition.NO_SLIP:
            for i in range(1, self._imax + 1):
                self.u[i, 0] = -self.u[i, 1]
                self.v[i, 0] = 0.0
        elif wS == BCondition.FREE_SLIP:
            for i in range(1, self._imax + 1):
                self.u[i, 0] = self.u[i, 1]
                self.v[i, 0] = 0.0
        elif wS == BCondition.OUTFLOW:
            for i in range(1, self._imax + 1):
                self.u[i, 0] = self.u[i, 1]
                self.v[i, 0] = self.v[i, 1]
        else:
            raise ValueError(f"Unrecognized BCondition: {wS}")

    def set_bc(self, prob_data: ProblemData) -> None:

        self._set_bc_w(prob_data["wW"])
        self._set_bc_e(prob_data["wE"])
        self._set_bc_n(prob_data["wN"])
        self._set_bc_s(prob_data["wS"])

        return
