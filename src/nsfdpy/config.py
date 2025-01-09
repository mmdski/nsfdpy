# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
from typing import Union

import numpy as np
import yaml

from nsfdpy._nsfd.grid import StaggeredGrid
from nsfdpy._nsfd.bcond import Type as BCType, Data as BCData
from nsfdpy._nsfd.config import (
    BoundaryCond,
    Constants,
    Geometry,
    InitialCond,
    Solver,
    Time,
)


class _Shape:

    def is_inside(self, x: float, y: float) -> bool:
        return True


class _Circle(_Shape):

    def __init__(self, x: float, y: float, d: float):
        self._x = x
        self._y = y
        self._d = d

    def is_inside(self, x: float, y: float) -> bool:

        d = np.sqrt((x - self._x) ** 2 + (y - self._y) ** 2)
        if d < self._d:
            return True
        else:
            return False


class _Rectangle(_Shape):

    def __init__(self, x1: float, x2: float, y1: float, y2: float):

        self._x1 = x1
        self._x2 = x2
        self._y1 = y1
        self._y2 = y2

    def is_inside(self, x: float, y: float) -> bool:

        if (x >= self._x1) and (x <= self._x2) and (y >= self._y1) and (y <= self._y2):
            return True
        else:
            return False


class Config:

    def __init__(self, file_name: str):

        with open(file_name, "r") as stream:
            try:
                self._config = yaml.safe_load(stream)
            except yaml.YAMLError as e:
                print(e)

    def _bc_data(self, bc_dict: dict[str, str]) -> BCData:

        bc_map = {
            "inflow": BCType.Inflow,
            "no slip": BCType.NoSlip,
            "outflow": BCType.Outflow,
            "periodic": BCType.Periodic,
        }

        bc_type = bc_map[bc_dict["type"]]

        try:
            return BCData(bc_type, bc_dict["value"])
        except KeyError:
            return BCData(bc_type)

    def boundary_cond(self) -> BoundaryCond:

        bc_dict = self._config["boundary"]

        n_bc = self._bc_data(bc_dict["north"])
        s_bc = self._bc_data(bc_dict["south"])
        e_bc = self._bc_data(bc_dict["east"])
        w_bc = self._bc_data(bc_dict["west"])

        return BoundaryCond(n_bc, s_bc, e_bc, w_bc)

    def constants(self) -> Constants:

        return Constants(
            self._config["constants"]["Re"],
            self._config["constants"]["GX"],
            self._config["constants"]["GY"],
        )

    def geometry(self) -> Geometry:

        imax = self._config["geometry"]["imax"]
        jmax = self._config["geometry"]["jmax"]
        xlength = self._config["geometry"]["xlength"]
        ylength = self._config["geometry"]["ylength"]

        if "obstacles" in self._config["geometry"].keys():

            obstacles = []
            grid = StaggeredGrid(self._config["geometry"])

            for k, v in self._config["geometry"]["obstacles"].items():

                shape: Union[_Rectangle, _Circle]

                if k == "circle":

                    x_c = v["x"]
                    y_c = v["y"]
                    d = v["diameter"]
                    shape = _Circle(x_c, y_c, d)

                elif k == "rectangle":

                    x1 = float(v["x1"])
                    x2 = float(v["x2"])
                    y1 = float(v["y1"])
                    y2 = float(v["y2"])
                    shape = _Rectangle(x1, x2, y1, y2)

                for i in range(grid.imax + 2):
                    for j in range(grid.jmax + 2):
                        if shape.is_inside(grid.p.x[i], grid.p.y[j]):
                            obstacles.append((i, j))

        return Geometry(imax, jmax, xlength, ylength)

    def initial_cond(self) -> InitialCond:

        pi = self._config["initial"]["PI"]
        ui = self._config["initial"]["UI"]
        vi = self._config["initial"]["VI"]

        return InitialCond(pi, ui, vi)

    def solver(self) -> Solver:

        omg = self._config["solver"]["omg"]
        itermax = self._config["solver"]["itermax"]
        eps = self._config["solver"]["eps"]
        gamma = self._config["solver"]["gamma"]

        return Solver(omg, itermax, eps, gamma)

    def time(self) -> Time:

        delt = self._config["time"]["delt"]
        try:
            tau = self._config["time"]["tau"]
        except KeyError:
            tau = None

        return Time(delt, tau)
