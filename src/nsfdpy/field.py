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
        self,
        geom_data: GeometryData,
        initial_value: tuple[float, float] | None = None,
        values: npt.NDArray[np.float64] | None = None,
    ):

        size = (geom_data["imax"] + 2, geom_data["jmax"] + 2, 2)
        if values is not None:
            if values.shape != size:
                raise ValueError("Invalid shape of ndarray")
            self._values = values.copy()
        else:
            self._values = np.empty(size, dtype=float)

        if initial_value is not None:
            self._values[:, :, 0].fill(initial_value[0])
            self._values[:, :, 1].fill(initial_value[1])

        self.u = self._values[:, :, 0]
        self.v = self._values[:, :, 1]

    def __add__(self, other: "VectorField") -> "VectorField":

        cls = self.__class__
        result = cls.__new__(cls)

        result._values = self._values + other._values
        result.u = result._values[:, :, 0]
        result.v = result._values[:, :, 1]

        return result

    def __sub__(self, other: "VectorField") -> "VectorField":

        cls = self.__class__
        result = cls.__new__(cls)

        result._values = self._values - other._values
        result.u = result._values[:, :, 0]
        result.v = result._values[:, :, 1]

        return result

    def __rmul__(self, other: float) -> "VectorField":

        cls = self.__class__
        result = cls.__new__(cls)

        result._values = other * self._values
        result.u = result._values[:, :, 0]
        result.v = result._values[:, :, 1]

        return result

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

    def shape(self) -> tuple[int, int, int]:

        shape = self._values.shape

        return shape[0], shape[1], shape[2]
