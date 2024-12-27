# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
import yaml

from nsfdpy._nsfd.bcond import Type as BCType, Data as BCData
from nsfdpy._nsfd.config import (
    BoundaryCond,
    Constants,
    Geometry,
    InitialCond,
    Solver,
    Time,
)


class Config:

    def __init__(self, file_name: str):

        with open(file_name, "r") as stream:
            try:
                self._config = yaml.safe_load(stream)
            except yaml.YAMLError as e:
                print(e)

    def _bc_data(self, bc_dict: dict[str, str]) -> BCData:

        bc_map = {"no slip": BCType.NoSlip}

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

        return Time(delt)
