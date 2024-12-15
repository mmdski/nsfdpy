# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
from enum import Enum
from typing import TypedDict, Optional


class GeometryData(TypedDict):
    """Geometry data"""

    xlength: float
    """Domain size in x-direction"""

    ylength: float
    """Domain size in y-direction"""

    imax: int
    """Number of interior cells in x-direction"""

    jmax: int
    """Number of interior cells in y-direction"""


class TimeStepData(TypedDict):
    """Time-stepping data"""

    t_max: float
    """Final time :math:`t_{\\text{end}}`"""

    delt: float
    """Time step size :math:`\\delta t`"""

    tau: float
    """Safety factor for time step size control :math:`\\tau`"""

    t: Optional[float]
    """Current time value"""


class PressureIterData(TypedDict):
    """Pressure-iteration data"""

    itermax: int
    """Maximum number of pressure iterations in one time step"""

    eps: float
    """Stopping tolerance :math:`\\epsilon` for pressure iteration"""

    omg: float
    """Relaxation parameter :math:`\\omega` for SOR iteration"""

    gamma: float
    """Upwind differencing factor :math:`\\gamma`"""

    it: Optional[int]
    """SOR iteration counter"""

    res: Optional[float]
    """Norm of pressure equation residual"""


class BCondition(Enum):

    NONE = 0
    """Do not set boundary condition"""

    FREE_SLIP = 1
    """Free-slip boundary condition"""

    NO_SLIP = 2
    """No-slip boundary condition"""

    OUTFLOW = 3
    """Outflow boundary condition"""

    PERIODIC = 4
    """Periodic boundary condition"""


class ProblemData(TypedDict):
    """Problem-dependent quantities"""

    Re: float
    """Reynolds number, :math:`Re`"""

    GX: float
    """Body force :math:`g_x`"""

    GY: float
    """Body force :math:`g_y` (e.g., gravity)"""

    UI: float
    """Initial data for velocity in x-direction"""

    VI: float
    """Initial data for velocity in y-direction"""

    PI: float
    """Initial data for pressure"""

    wW: BCondition
    """Type of boundary condition along the western (left) boundary"""

    wE: BCondition
    """Type of boundary condition along the eastern (right) boundary"""

    wN: BCondition
    """Type of boundary condition along the northern (upper) boundary"""

    wS: BCondition
    """Type of boundary condition along the southern (lower) boundary"""

    problem: Optional[str]
    """Specifies flow-specific quantities"""
