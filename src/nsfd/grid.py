from typing import Optional

from matplotlib.axes import Axes
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.path as mpath
import numpy as np
import numpy.typing as npt

from nsfd.typing import GeometryData

CoordIndex = tuple[int, int]


def plot_boundary_patch(grid_props: GeometryData, ax: Axes) -> Axes:

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


def plot_cells_grid(grid_props: GeometryData, ax: Axes) -> Axes:

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


class Geometry:

    def __init__(self, props: GeometryData, x_frac: float = 0.5, y_frac: float = 0.5):

        # must at least have one internal cell
        if props["imax"] < 1:
            raise ValueError("imax must be greater than or equal to 1")
        if props["jmax"] < 1:
            raise ValueError("jmax must be greater than or equal to 1")

        # must be a non-zero, positive length
        if props["xlength"] <= 0:
            raise ValueError("xlength must be greater than 0")
        if props["ylength"] <= 0:
            raise ValueError("ylength must be greater than 0")

        if x_frac < 0 or 1 < x_frac:
            raise ValueError("x_frac must be between 0 and 1")
        if y_frac < 0 or 1 < y_frac:
            raise ValueError("y_frac must be between 0 and 1")

        self._imax = int(props["imax"])
        self._jmax = int(props["jmax"])
        self._xlength = float(props["xlength"])
        self._ylength = float(props["ylength"])
        self._delx = self._xlength / self._imax
        self._dely = self._ylength / self._jmax
        self._x_frac = float(x_frac)
        self._y_frac = float(y_frac)

    def __getitem__(self, key: CoordIndex) -> tuple[float, float]:
        i = key[0]
        j = key[1]
        if i < 0 or i > self._imax + 1:
            raise ValueError
        if j < 0 or j > self._jmax + 1:
            raise ValueError

        return self._delx * (i - self._x_frac), self._dely * (j - self._y_frac)

    def delx(self) -> float:
        return self._delx

    def dely(self) -> float:
        return self._dely

    def grid_coords(self) -> list[npt.NDArray[np.double]]:
        x = self._delx * (np.arange(-1, self._imax + 1, dtype=np.double) + self._x_frac)
        y = self._dely * (np.arange(-1, self._jmax + 1, dtype=np.double) + self._y_frac)

        return np.meshgrid(x, y)

    def geom_data(self) -> GeometryData:
        return {
            "imax": self._imax,
            "jmax": self._jmax,
            "xlength": self._xlength,
            "ylength": self._ylength,
        }

    def plot(
        self,
        ax: Optional[Axes] = None,
        grid: bool = False,
        boundary: bool = False,
        **kwargs,
    ) -> Axes:
        if not ax:
            ax = plt.axes()

        if grid:
            plot_cells_grid(self.geom_data(), ax)

        if boundary:
            plot_boundary_patch(self.geom_data(), ax)

        X, Y = self.grid_coords()

        ax.scatter(X, Y, **kwargs)

        return ax

    def shape(self) -> tuple[int, int]:
        return self._imax + 1, self._jmax + 1


class GridValues:

    def __init__(
        self,
        grid_props: GeometryData,
        x_frac: float,
        y_frac: float,
        init_value: float = 0,
    ):

        self._grid_geom = Geometry(grid_props, x_frac, y_frac)
        self._values = np.full(self._grid_geom.shape(), init_value)

    def __getitem__(self, key: tuple[int, int]) -> float:
        return self._values[key]

    def __setitem__(self, key: tuple[int, int], value: float) -> None:
        self._values[*key] = value

    def delx(self) -> float:
        return self._grid_geom.delx()

    def dely(self) -> float:
        return self._grid_geom.dely()

    def geom_data(self) -> GeometryData:
        return self._grid_geom.geom_data()

    def max(self) -> np.double:
        return self._values.max()

    def shape(self) -> tuple[int, int]:
        return self._grid_geom.shape()

    def values(self) -> npt.NDArray[np.double]:
        return self._values.copy()
