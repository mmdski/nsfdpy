from math import isfinite
from ctypes import c_size_t

import matplotlib.pyplot as plt
import numpy as np

import nsfd.libnsfd as libnsfd


class Grid:
    """Grid base class"""

    def __init__(self, m_rows, n_cols, x_min, x_max, y_min, y_max, init_value=0):

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

        ax.scatter(self._x_coords.flatten(), self._y_coords.flatten(), **kwargs)

        return ax

    def values(self):

        values = np.empty((self._m_rows, self._n_cols))
        libnsfd.grid_get_values(self._grid_ptr, values.ctypes.data_as(libnsfd.Real_p))

        return values


class PGrid(Grid):
    """Pressure grid"""

    def __init__(self, grid_props):

        m_cell_rows = grid_props["m_rows"]
        n_cell_cols = grid_props["n_cols"]

        domain_x_min = grid_props["x_min"]
        domain_x_max = grid_props["x_max"]
        dx = (domain_x_max - domain_x_min) / n_cell_cols

        grid_x_min = domain_x_min + dx / 2
        grid_x_max = domain_x_max - dx / 2

        domain_y_min = grid_props["y_min"]
        domain_y_max = grid_props["y_max"]
        dy = (domain_y_max - domain_y_min) / m_cell_rows

        grid_y_min = domain_y_min + dy / 2
        grid_y_max = domain_y_max - dy / 2

        super().__init__(
            m_cell_rows,
            n_cell_cols,
            grid_x_min,
            grid_x_max,
            grid_y_min,
            grid_y_max,
        )


class UGrid(Grid):
    """Horizontal (x-axis) velocity grid"""

    def __init__(self, grid_props):

        m_rows = grid_props["m_rows"]
        n_cols = grid_props["n_cols"] + 1

        domain_x_min = grid_props["x_min"]
        domain_x_max = grid_props["x_max"]

        # include left boundary nodes
        grid_x_min = domain_x_min
        grid_x_max = domain_x_max

        domain_y_min = grid_props["y_min"]
        domain_y_max = grid_props["y_max"]
        dy = (domain_y_max - domain_y_min) / m_rows

        grid_y_min = domain_y_min + dy / 2
        grid_y_max = domain_y_max - dy / 2

        super().__init__(
            m_rows,
            n_cols,
            grid_x_min,
            grid_x_max,
            grid_y_min,
            grid_y_max,
        )


class VGrid(Grid):
    """Vertical (y-axis) velocity grid"""

    def __init__(self, grid_props):

        m_rows = grid_props["m_rows"] + 1
        n_cols = grid_props["n_cols"]

        domain_x_min = grid_props["x_min"]
        domain_x_max = grid_props["x_max"]
        dx = (domain_x_max - domain_x_min) / n_cols

        grid_x_min = domain_x_min + dx / 2
        grid_x_max = domain_x_max - dx / 2

        domain_y_min = grid_props["y_min"]
        domain_y_max = grid_props["y_max"]

        # include bottom boundary nodes
        grid_y_min = domain_y_min
        grid_y_max = domain_y_max

        super().__init__(
            m_rows,
            n_cols,
            grid_x_min,
            grid_x_max,
            grid_y_min,
            grid_y_max,
        )
