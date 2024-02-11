from math import isfinite
from ctypes import c_size_t

import matplotlib.pyplot as plt
import numpy as np

from nsfd.libnsfd import *


class ScalarField:

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

        s_field_ptr = s_field_new(c_size_t(m_rows), c_size_t(n_cols))
        s_field_init_const(s_field_ptr, Real(init_value))
        s_field_init_grid(s_field_ptr, x_min, x_max, y_min, y_max)

        x_grid = np.empty((m_rows, n_cols))
        y_grid = np.empty((m_rows, n_cols))

        s_field_get_grid(
            s_field_ptr, x_grid.ctypes.data_as(Real_p), y_grid.ctypes.data_as(Real_p)
        )

        self._m_rows = m_rows
        self._n_cols = n_cols
        self._s_field_ptr = s_field_ptr
        self._x_grid = x_grid
        self._y_grid = y_grid

    def __del__(self):

        s_field_free(self._s_field_ptr)

    def grid(self):

        return (self._x_grid.copy(), self._y_grid.copy())

    def plot(self, ax=None):

        if ax is None:
            ax = plt.axes()

        ax.scatter(
            self._x_grid.flatten(), self._y_grid.flatten(), c=self.values().flatten()
        )

        return ax

    def values(self):

        s_values = np.empty((self._m_rows, self._n_cols))
        s_field_get_values(self._s_field_ptr, s_values.ctypes.data_as(Real_p))

        return s_values
