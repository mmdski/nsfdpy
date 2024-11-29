import numpy.typing as npt
import numpy as np

from nsfdpy.typing import GeometryData


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
