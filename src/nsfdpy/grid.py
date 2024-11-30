from typing import Any
from copy import copy

import numpy.typing as npt
import numpy as np

from nsfdpy.typing import GeometryData


class GridAxis:

    def __init__(self, length: float, max: int, frac: float):

        self._del = length / max
        self._frac = frac

    def __getitem__(self, i: int) -> float:
        return self._del * (i - self._frac)

    def __setitem__(self, i: int, value: Any) -> None:
        raise NotImplementedError

    def cell_size(self) -> float:
        return self._del


class Grid:

    def __init__(self, data: GeometryData, x_frac: float, y_frac: float):

        self._imax = data["imax"]
        self._jmax = data["jmax"]
        self.x = GridAxis(data["xlength"], data["imax"], x_frac)
        self.y = GridAxis(data["ylength"], data["jmax"], y_frac)

    def __getitem__(self, idx: tuple[int, int]) -> tuple[float, float]:
        i = idx[0]
        j = idx[1]
        return self.x[i], self.y[j]

    def cell_size(self) -> tuple[float, float]:
        return self.x.cell_size(), self.y.cell_size()

    def dim(self) -> tuple[int, int]:
        return self._imax, self._jmax

    def meshgrid(self) -> tuple[npt.NDArray[Any], ...]:

        x = [self.x[i] for i in range(self._imax + 2)]
        y = [self.y[j] for j in range(self._jmax + 2)]

        return np.meshgrid(x, y, indexing="ij")


class StaggeredGrid:

    def __init__(self, data: GeometryData):

        self._data = copy(data)

        self.p = Grid(data, 0.5, 0.5)
        self.u = Grid(data, 0, 0.5)
        self.v = Grid(data, 0.5, 0)

        self.delx = data["xlength"] / data["imax"]
        self.dely = data["ylength"] / data["jmax"]

    @property
    def geom_data(self) -> GeometryData:

        return copy(self._data)

    @property
    def imax(self) -> int:
        return self._data["imax"]

    @property
    def jmax(self) -> int:
        return self._data["jmax"]
