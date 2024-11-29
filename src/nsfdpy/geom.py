from copy import copy
from typing import Any

from matplotlib.axes import Axes
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.path as mpath
import numpy as np

from nsfdpy.typing import GeometryData
from nsfdpy.grid import grid_lines


class Coordinates:

    def __init__(self, length: float, max: int, frac: float):

        self._del = length / max
        self._frac = frac

    def __getitem__(self, i: int) -> float:
        return self._del * (i - self._frac)

    def __setitem__(self, i: int, value: Any) -> None:
        raise NotImplementedError


class GridCoordinates:

    def __init__(self, data: GeometryData, x_frac: float, y_frac: float):

        self._imax = data["imax"]
        self._jmax = data["jmax"]
        self.x = Coordinates(data["xlength"], data["imax"], x_frac)
        self.y = Coordinates(data["ylength"], data["jmax"], y_frac)

    def __getitem__(self, idx: tuple[int, int]) -> tuple[float, float]:
        i = idx[0]
        j = idx[1]
        return self.x[i], self.y[j]

    def dim(self) -> tuple[int, int]:
        return self._imax, self._jmax

    def plot(
        self, ax: Axes | None = None, boundary: bool = False, **kwargs: dict[str, Any]
    ) -> Axes:

        if not ax:
            ax = plt.axes()

        if boundary:
            size = (self._imax + 2, self._jmax + 2)
            offset = 0
        else:
            size = (self._imax, self._jmax)
            offset = 1

        X = np.empty(size)
        Y = np.empty(size)
        for i in range(size[0]):
            for j in range(size[1]):
                x, y = self[i + offset, j + offset]
                X[i, j] = x
                Y[i, j] = y

        ax.scatter(X, Y, **kwargs)  # type: ignore[arg-type]

        return ax


class Geometry:

    def __init__(self, data: GeometryData):

        self._data = copy(data)

        self.p = GridCoordinates(data, 0.5, 0.5)
        self.u = GridCoordinates(data, 0, 0.5)
        self.v = GridCoordinates(data, 0.5, 0)

    @staticmethod
    def _plot_grid_cells(
        geom_data: GeometryData, ax: Axes, boundary: bool = False
    ) -> Axes:

        v_grid_lines, h_grid_lines = grid_lines(geom_data, boundary)

        ymin = h_grid_lines[0]
        ymax = h_grid_lines[-1]

        xmin = v_grid_lines[0]
        xmax = v_grid_lines[-1]

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
