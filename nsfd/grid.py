from copy import copy
from ctypes import pointer

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.path as mpath
import numpy as np

from nsfd import libnsfd


def plot_boundary_patch(grid_props, ax):

    xlength = grid_props["xlength"]
    ylength = grid_props["ylength"]
    imax = grid_props["imax"]
    jmax = grid_props["jmax"]
    delx = xlength / imax
    dely = ylength / jmax

    inside_vertices = np.array(
        [[0, 0], [xlength, 0], [xlength, ylength], [0, ylength], [0, 0]]
    )
    outside_vertices = np.array(
        [
            [-delx, -dely],
            [xlength + delx, -dely],
            [xlength + delx, ylength + dely],
            [-delx, ylength + dely],
            [-delx, -dely],
        ]
    )
    codes = (
        np.ones(len(inside_vertices), dtype=mpath.Path.code_type) * mpath.Path.LINETO
    )
    codes[0] = mpath.Path.MOVETO

    vertices = np.concatenate((outside_vertices[::1], inside_vertices[::-1]))
    all_codes = np.concatenate((codes, codes))
    path = mpath.Path(vertices, all_codes)
    patch = mpatches.PathPatch(path, alpha=0.5, facecolor="gray")
    ax.add_patch(patch)

    return ax


def plot_cells_grid(grid_props, ax):

    xlength = grid_props["xlength"]
    ylength = grid_props["ylength"]
    imax = grid_props["imax"]
    jmax = grid_props["jmax"]
    delx = xlength / imax
    dely = ylength / jmax

    v_grid_lines = np.linspace(-delx, xlength + delx, imax + 3)
    h_grid_lines = np.linspace(-dely, ylength + dely, jmax + 3)

    ax.vlines(
        v_grid_lines,
        -dely,
        ylength + dely,
        colors="gray",
        linestyles="dashed",
        linewidth=0.5,
    )

    ax.hlines(
        h_grid_lines,
        -delx,
        xlength + delx,
        colors="gray",
        linestyles="dashed",
        linewidth=0.5,
    )

    return ax


class _GridGeom:

    def __init__(self, geom_grid_p, grid_props):

        self._geom_grid_p = geom_grid_p

        n_cells = libnsfd.grid_geom_n_cells(self._geom_grid_p)
        x_coords = np.empty((n_cells,), dtype=libnsfd.Real)
        y_coords = np.empty((n_cells,), dtype=libnsfd.Real)
        libnsfd.grid_geom_coords(
            self._geom_grid_p,
            x_coords.ctypes.data_as(libnsfd.RealPtr),
            y_coords.ctypes.data_as(libnsfd.RealPtr),
        )

        self._x_coords = x_coords
        self._y_coords = y_coords
        self._grid_props = copy(grid_props)

    def __del__(self):

        libnsfd.grid_geom_free(pointer(self._geom_grid_p))

    def coords(self):

        return self._x_coords.copy(), self._y_coords.copy()

    def plot(self, ax=None, grid=False, boundary=False, **kwargs):

        if not ax:
            ax = plt.axes()

        if grid:
            plot_cells_grid(self._grid_props, ax)

        if boundary:
            plot_boundary_patch(self._grid_props, ax)

        ax.scatter(self._x_coords, self._y_coords, **kwargs)

        return ax


class PGridGeom(_GridGeom):

    def __init__(self, grid_props):

        grid_data = libnsfd.GridGeomData(
            grid_props["imax"],
            grid_props["jmax"],
            grid_props["xlength"],
            grid_props["ylength"],
        )

        grid_geom_p = libnsfd.grid_geom_new(pointer(grid_data))
        libnsfd.grid_geom_init_p(grid_geom_p)

        super().__init__(grid_geom_p, grid_props)


class UGridGeom(_GridGeom):

    def __init__(self, grid_props):

        grid_data = libnsfd.GridGeomData(
            grid_props["imax"],
            grid_props["jmax"],
            grid_props["xlength"],
            grid_props["ylength"],
        )

        grid_geom_p = libnsfd.grid_geom_new(pointer(grid_data))
        libnsfd.grid_geom_init_u(grid_geom_p)

        super().__init__(grid_geom_p, grid_props)


class VGridGeom(_GridGeom):

    def __init__(self, grid_props):

        grid_data = libnsfd.GridGeomData(
            grid_props["imax"],
            grid_props["jmax"],
            grid_props["xlength"],
            grid_props["ylength"],
        )

        grid_geom_p = libnsfd.grid_geom_new(pointer(grid_data))
        libnsfd.grid_geom_init_v(grid_geom_p)

        super().__init__(grid_geom_p, grid_props)
