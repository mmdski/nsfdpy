import ctypes
import typing

import numpy as np
import numpy.typing as npt

from nsfdpy import nsfd, pymem


class ScalarField:

    def __init__(self, imax: int, jmax: int, init_val: float = 0):

        if imax <= 0:
            raise ValueError("imax must be greater than 0")
        if jmax <= 0:
            raise ValueError("jmax must be greater than 0")

        init_val = nsfd.Real(init_val)  # type: ignore[assignment]

        self._shape = nsfd.DomainShape(imax, jmax)
        s_field_size = nsfd.s_field_size(self._shape)
        s_field_ptr = ctypes.cast(pymem.malloc(s_field_size), nsfd.ScalarFieldPtr)
        if s_field_ptr is None:
            raise MemoryError
        self._s_field_ptr = nsfd.s_field_init(self._shape, s_field_ptr, init_val)

    def __del__(self) -> None:

        try:
            pymem.free(self._s_field_ptr)
        except AttributeError:
            pass

    def __getitem__(self, idx: tuple[int, int]) -> float:

        if len(idx) != 2:
            raise IndexError

        if idx[0] < 0:
            raise IndexError
        if idx[0] > self._shape.imax + 1:
            raise IndexError
        if idx[1] < 0:
            raise IndexError
        if idx[1] > self._shape.jmax + 1:
            raise IndexError

        return typing.cast(
            float, nsfd.s_field_get(self._s_field_ptr, idx[0], idx[1]).contents.value
        )

    def __setitem__(self, idx: tuple[int, int], value: float) -> None:

        if len(idx) != 2:
            raise IndexError

        if idx[0] < 0:
            raise IndexError
        if idx[0] > self._shape.imax + 1:
            raise IndexError
        if idx[1] < 0:
            raise IndexError
        if idx[1] > self._shape.jmax + 1:
            raise IndexError

        nsfd.s_field_set(self._s_field_ptr, idx[0], idx[1], nsfd.Real(value))

    def values(self) -> npt.NDArray[typing.Any]:
        grid_shape = nsfd.s_field_grid_shape(self._s_field_ptr)
        values = np.empty((grid_shape.nx, grid_shape.ny), dtype=nsfd.Real)
        nsfd.s_field_values(self._s_field_ptr, values.ctypes.data_as(nsfd.RealPtr))
        return values
