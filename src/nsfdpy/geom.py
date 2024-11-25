from copy import copy
from typing import Any

from matplotlib.axes import Axes
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.path as mpath
import numpy as np
import numpy.typing as npt

from nsfdpy.typing import GeometryData


class Coordinates:

    def __init__(self, length: float, max: int, frac: float):

        self._del = length / max
        self._frac = frac

    def __call__(self, i: int) -> float:
        return self._del * (i - self._frac)


class GridCoordinates:

    def __init__(self, data: GeometryData, x_frac: float, y_frac: float):

        self._imax = data["imax"]
        self._jmax = data["jmax"]
        self.x = Coordinates(data["xlength"], data["imax"], x_frac)
        self.y = Coordinates(data["ylength"], data["jmax"], y_frac)

    def meshgrid(self, boundary: bool = False) -> tuple[npt.NDArray[np.double], ...]:

        if boundary:
            i_start = 0
            i_stop = self._imax + 2
            j_start = 0
            j_stop = self._jmax + 2
        else:
            i_start = 1
            i_stop = self._imax + 1
            j_start = 1
            j_stop = self._jmax + 1

        x = [self.x(i) for i in range(i_start, i_stop)]
        y = [self.y(j) for j in range(j_start, j_stop)]

        return np.meshgrid(x, y)

    def plot(
        self, ax: Axes | None = None, boundary: bool = False, **kwargs: dict[str, Any]
    ) -> Axes:

        if not ax:
            ax = plt.axes()

        X, Y = self.meshgrid(boundary)

        ax.scatter(X, Y, **kwargs)  # type: ignore[arg-type]

        return ax


class Geometry:

    def __init__(self, data: GeometryData):

        self._data = copy(data)

        self.p = GridCoordinates(data, 0.5, 0.5)
        self.u = GridCoordinates(data, 0, 0.5)
        self.v = GridCoordinates(data, 0.5, 0)

    @staticmethod
    def _plot_grid_cells(geom_data: GeometryData, ax: Axes, bc: bool = False) -> Axes:

        delx = geom_data["xlength"] / geom_data["imax"]
        dely = geom_data["ylength"] / geom_data["jmax"]

        if bc:
            ymin = -dely
            ymax = geom_data["ylength"] + dely
            num_v_grid_lines = geom_data["imax"] + 3

            xmin = -delx
            xmax = geom_data["xlength"] + delx
            num_h_grid_lines = geom_data["jmax"] + 3
        else:
            ymin = 0
            ymax = geom_data["ylength"]
            num_v_grid_lines = geom_data["imax"] + 1

            xmin = 0
            xmax = geom_data["xlength"]
            num_h_grid_lines = geom_data["jmax"] + 1

        v_grid_lines = np.linspace(xmin, xmax, num_v_grid_lines)
        h_grid_lines = np.linspace(ymin, ymax, num_h_grid_lines)

        ax.vlines(
            v_grid_lines,
            ymin,
            ymax,
            colors="gray",
            linestyles="dashed",
            linewidth=0.5,
        )

        ax.hlines(
            h_grid_lines,
            xmin,
            xmax,
            colors="gray",
            linestyles="dashed",
            linewidth=0.5,
        )

        return ax

    @staticmethod
    def _plot_boundary_patch(grid_props: GeometryData, ax: Axes) -> Axes:

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
            np.ones(len(inside_vertices), dtype=mpath.Path.code_type)
            * mpath.Path.LINETO
        )
        codes[0] = mpath.Path.MOVETO

        vertices = np.concatenate((outside_vertices[::1], inside_vertices[::-1]))
        all_codes = np.concatenate((codes, codes))
        path = mpath.Path(vertices, all_codes)
        patch = mpatches.PathPatch(path, alpha=0.5, facecolor="gray")
        ax.add_patch(patch)

        return ax

    def plot_cells(self, ax: Axes | None = None, bc: bool = False) -> Axes:

        if not ax:
            ax = plt.axes()

        self._plot_grid_cells(self._data, ax, bc)
        if bc:
            self._plot_boundary_patch(self._data, ax)

        return ax
