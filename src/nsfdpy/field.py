from ctypes import c_double, c_float, cast, pythonapi

import numpy as np
import numpy.typing as npt

from nsfdpy import nsfd


class ScalarField:

    def __init__(self, imax: int, jmax: int, init_val=0):

        if imax <= 0:
            raise ValueError("imax must be greater than 0")
        if jmax <= 0:
            raise ValueError("jmax must be greater than 0")

        init_val = nsfd.Real(init_val)

        self._shape = nsfd.DomainShape(imax, jmax)
        s_field_size = nsfd.s_field_size(self._shape)
        s_field_ptr = cast(pythonapi.PyMem_Malloc(s_field_size), nsfd.ScalarFieldPtr)
        if not s_field_ptr:
            raise MemoryError
        self._s_field_ptr = nsfd.s_field_init(self._shape, s_field_ptr, init_val)

    def __del__(self) -> None:

        pythonapi.PyMem_Free(self._s_field_ptr)

    def __getitem__(self, idx: tuple[int, int]) -> float:

        if idx[0] < 0:
            raise IndexError
        if idx[0] > self._shape.imax + 1:
            raise IndexError
        if idx[1] < 0:
            raise IndexError
        if idx[1] > self._shape.jmax + 1:
            raise IndexError

        return nsfd.s_field_get(self._s_field_ptr, idx[0], idx[1]).contents.value

    def __setitem__(self, idx: tuple[int, int], value: float) -> None:
        if idx[0] < 0:
            raise IndexError
        if idx[0] > self._shape.imax + 1:
            raise IndexError
        if idx[1] < 0:
            raise IndexError
        if idx[1] > self._shape.jmax + 1:
            raise IndexError

        nsfd.s_field_set(self._s_field_ptr, idx[0], idx[1], nsfd.Real(value))

    def values(self) -> np.ndarray:
        grid_shape = nsfd.s_field_grid_shape(self._s_field_ptr)
        values = np.empty((grid_shape.nx, grid_shape.ny), dtype=nsfd.Real)
        nsfd.s_field_values(self._s_field_ptr, values.ctypes.data_as(nsfd.RealPtr))
        return values
