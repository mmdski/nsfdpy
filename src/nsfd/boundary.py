import numpy as np
from numpy.typing import NDArray

from nsfd.typing import BCondition


def set_b_cond(
    u: NDArray[np.float64],
    v: NDArray[np.float64],
    imax: int,
    jmax: int,
    wW: BCondition,
    wE: BCondition,
    wN: BCondition,
    wS: BCondition,
):

    shape = (imax + 2, jmax + 2)

    if not u.shape == shape:
        raise ValueError

    if not v.shape == shape:
        raise ValueError

    # west (left) boundary
    if wW == BCondition.NO_SLIP:
        u[0, :] = 0
        v[0, :] = -v[1, :]
    elif wW == BCondition.FREE_SLIP:
        u[0, :] = 0
        v[0, :] = v[1, :]
    elif wW == BCondition.OUTFLOW:
        u[0, :] = u[1, :]
        v[0, :] = v[1, :]

    # east (right) boundary
    if wE == BCondition.NO_SLIP:
        u[imax, :] = 0
        v[imax + 1, :] = -v[imax, :]
    elif wE == BCondition.FREE_SLIP:
        u[imax, :] = 0
        v[imax + 1, :] = v[imax, :]
    elif wE == BCondition.OUTFLOW:
        u[imax, :] = u[imax - 1, :]
        v[imax + 1, :] = v[imax, :]

    # north (upper) boundary
    if wN == BCondition.NO_SLIP:
        v[:, jmax] = 0
        u[:, jmax + 1] = -u[:, jmax]
    elif wN == BCondition.FREE_SLIP:
        v[:, jmax] = 0
        u[:, jmax + 1] = u[:, jmax]
    elif wN == BCondition.OUTFLOW:
        u[:, jmax + 1] = u[:, jmax]
        v[:, jmax] = v[:, jmax - 1]

    # south (lower) boundary
    if wS == BCondition.NO_SLIP:
        v[:, 0] = 0
        u[:, 0] = -u[:, 1]
    elif wS == BCondition.FREE_SLIP:
        v[:, 0] = 0
        u[:, 0] = u[:, 1]
    elif wN == BCondition.OUTFLOW:
        u[:, 0] = u[:, 1]
        v[:, 0] = v[:, 1]
