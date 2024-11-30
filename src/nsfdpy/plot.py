from typing import Any, Literal

from matplotlib.axes import Axes
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.path as mpath
import numpy.typing as npt
import numpy as np

from nsfdpy.grid import Grid, StaggeredGrid
from nsfdpy.typing import GeometryData
from nsfdpy.field import ScalarField, VectorField
from nsfdpy.interp import VectorFieldInterp


def grid_lines(
    geom_data: GeometryData, boundary: bool = False
) -> tuple[npt.NDArray[np.float64], npt.NDArray[np.float64]]:

    delx = geom_data["xlength"] / geom_data["imax"]
    dely = geom_data["ylength"] / geom_data["jmax"]

    if boundary:
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

    return v_grid_lines, h_grid_lines


class GridPlot:

    def __init__(self, grid: Grid):

        self._grid = grid

    def scatter(
        self, ax: Axes | None = None, boundary: bool = False, **kwargs: dict[str, Any]
    ) -> Axes:

        if not ax:
            ax = plt.axes()

        X, Y = self._grid.meshgrid()

        if boundary:
            ax.scatter(X, Y, **kwargs)  # type: ignore[arg-type]
        else:
            ax.scatter(X[1:-1, 1:-1], Y[1:-1, 1:-1], **kwargs)  # type: ignore[arg-type]

        return ax


class StaggeredGridPlot:

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid
        self.p = GridPlot(self._grid.p)
        self.u = GridPlot(self._grid.u)
        self.v = GridPlot(self._grid.v)

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

    def cells(self, ax: Axes | None = None, boundary: bool = False) -> Axes:

        if not ax:
            ax = plt.axes()

        v_grid_lines, h_grid_lines = grid_lines(self._grid.geom_data, boundary)

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

        if boundary:
            ax = self._plot_boundary_patch(self._grid.geom_data, ax)

        return ax


class ScalarFieldPlot:

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

    def pcolormesh(
        self,
        field: ScalarField,
        ax: Axes | None = None,
        boundary: bool = False,
        **kwargs: dict[str, Any],
    ) -> Axes:

        if not ax:
            ax = plt.axes()

        v_grid_lines, h_grid_lines = grid_lines(self._grid.geom_data, boundary)

        if boundary:
            ax.pcolormesh(
                v_grid_lines, h_grid_lines, field.values.T, **kwargs  # type: ignore[arg-type]
            )
        else:
            ax.pcolormesh(
                v_grid_lines, h_grid_lines, field.values[1:-1, 1:-1].T, **kwargs  # type: ignore[arg-type]
            )

        return ax


class VectorFieldPlot:

    def __init__(self, grid: StaggeredGrid):

        self._grid = grid

        X, Y = self._grid.p.meshgrid()
        self._X = X[1:-1, 1:-1]
        self._Y = Y[1:-1, 1:-1]

    def pcolormesh(
        self,
        field: VectorField,
        component: Literal["u", "v"],
        ax: Axes | None = None,
        boundary: bool = False,
        **kwargs: dict[str, Any],
    ) -> Axes:

        if not ax:
            ax = plt.axes()

        if component == "u":
            values = field.u
        elif component == "v":
            values = field.v
        else:
            raise ValueError(f"Unrecognized component: {component}")

        v_grid_lines, h_grid_lines = grid_lines(self._grid.geom_data, boundary)

        if boundary:
            ax.pcolormesh(
                v_grid_lines, h_grid_lines, values.T, **kwargs  # type: ignore[arg-type]
            )
        else:
            ax.pcolormesh(
                v_grid_lines, h_grid_lines, values[1:-1, 1:-1].T, **kwargs  # type: ignore[arg-type]
            )

        return ax

    def quiver(
        self,
        field: VectorField,
        plot_cells: bool = False,
        ax: Axes | None = None,
        **kwargs: dict[str, Any],
    ) -> Axes:

        if not ax:
            ax = plt.axes()

        U = np.empty((self._grid.imax, self._grid.jmax))
        V = np.empty((self._grid.imax, self._grid.jmax))

        interp = VectorFieldInterp(self._grid, field)

        # interior grid points only
        for i in range(self._grid.imax):
            for j in range(self._grid.jmax):
                U[i, j], V[i, j] = interp(self._X[i, j], self._Y[i, j])

        if plot_cells:
            StaggeredGridPlot(self._grid).cells(ax)
        ax.quiver(self._X, self._Y, U, V, **kwargs)

        return ax
