from typing import cast

from numpy.typing import NDArray


def comp_delt(
    delx: float,
    dely: float,
    u: NDArray,
    v: NDArray,
    Re: float,
    tau: float,
) -> float:

    # avoid boundary cells
    u_max = u[1:-1, 1:-1].max()
    v_max = v[1:-1, 1:-1].max()

    if tau < 0:
        tau = 1

    v1 = cast(float, Re / 2.0 * ((1.0 / delx**2) + (1.0 / dely**2)) ** -1.0)
    v2 = cast(float, delx / u_max)
    v3 = cast(float, dely / v_max)

    return float(min([v1, v2, v3]))
