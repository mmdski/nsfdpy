from copy import deepcopy
from typing import cast, Any

import numpy as np
import numpy.typing as npt

from nsfdpy.typing import GeometryData


class ScalarField:

    def __init__(self, geom_data: GeometryData, initial_value: float | None = None):

        size = (geom_data["imax"] + 2, geom_data["jmax"] + 2)
        if initial_value is not None:
            self._values = np.full(size, initial_value, dtype=np.float64)
        else:
            self._values = np.empty(size, dtype=np.float64)

    def __deepcopy__(self, memo: dict[int, Any]) -> "ScalarField":

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

    def abs_max(self) -> float:

        return cast(float, np.absolute(self._values).max())

    @property
    def values(self) -> npt.NDArray[np.float64]:
        return self._values.copy()


class VectorField:

    def __init__(
        self, geom_data: GeometryData, initial_value: tuple[float, float] | None = None
    ):

        size = (geom_data["imax"] + 2, geom_data["jmax"] + 2, 2)
        if initial_value:
            self._values = np.full(size, initial_value, dtype=float)
        else:
            self._values = np.empty(size, dtype=float)

        self.u = self._values[:, :, 0]
        self.v = self._values[:, :, 1]

    def __deepcopy__(self, memo: dict[int, Any]) -> "VectorField":

        cls = self.__class__
        result = cls.__new__(cls)
        memo[id(self)] = result
        for k, v in self.__dict__.items():
            setattr(result, k, deepcopy(v, memo))

        return result

    def __getitem__(self, idx: tuple[int, int]) -> tuple[float, float]:

        return cast(tuple[float, float], self._values[idx[0], idx[1], :])

    def __setitem__(self, idx: tuple[int, int], value: tuple[float, float]) -> None:

        self._values[idx[0], idx[1], :] = value

    def abs_max(self) -> tuple[float, float]:

        u_abs_max = np.absolute(self._values[:, :, 0]).max()
        v_abs_max = np.absolute(self._values[:, :, 1]).max()

        return u_abs_max, v_abs_max
