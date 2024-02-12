from math import isfinite
from ctypes import c_size_t

import matplotlib.pyplot as plt
import numpy as np

import nsfd.libnsfd as libnsfd


class Grid:

    def __init__(
        self, m_rows, n_cols, x_min=0, x_max=1, y_min=0, y_max=1, init_value=0
    ):

        if m_rows < 0:
            raise ValueError
        if n_cols < 0:
            raise ValueError

        if not (
            isfinite(x_min)
            and isfinite(x_max)
            and isfinite(y_min)
            and isfinite(y_max)
            and isfinite(init_value)
        ):
            raise ValueError

        if x_min >= x_max:
            raise ValueError

        if y_min >= y_max:
            raise ValueError

        grid_ptr = libnsfd.grid_new(c_size_t(m_rows), c_size_t(n_cols))
        libnsfd.grid_init_const(grid_ptr, libnsfd.Real(init_value))
        libnsfd.grid_init_grid(grid_ptr, x_min, x_max, y_min, y_max)

        x_coords = np.empty((m_rows, n_cols))
        y_coords = np.empty((m_rows, n_cols))

        libnsfd.grid_get_coords(
            grid_ptr,
            x_coords.ctypes.data_as(libnsfd.Real_p),
            y_coords.ctypes.data_as(libnsfd.Real_p),
        )

        self._m_rows = m_rows
        self._n_cols = n_cols
        self._grid_ptr = grid_ptr
        self._x_coords = x_coords
        self._y_coords = y_coords

    def __del__(self):

        libnsfd.grid_free(self._grid_ptr)

    def grid_coords(self):

        return (self._x_grid.copy(), self._y_grid.copy())

    def plot(self, ax=None, **kwargs):

        if ax is None:
            ax = plt.axes()

        ax.scatter(
            self._x_coords.flatten(),
            self._y_coords.flatten(),
            c=self.values().flatten(),
            **kwargs
        )

        return ax

    def values(self):

        values = np.empty((self._m_rows, self._n_cols))
        libnsfd.grid_get_values(self._grid_ptr, values.ctypes.data_as(libnsfd.Real_p))

        return values
