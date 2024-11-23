import numpy as np
from numpy.typing import NDArray


def init_uvp(
    imax: int, jmax: int, ui: float, vi: float, pi: float
) -> tuple[NDArray, NDArray, NDArray]:

    shape = (imax + 2, jmax + 2)
    u = np.full(shape, ui)
    v = np.full(shape, vi)
    p = np.full(shape, pi)

    return u, v, p


def comp_fg(
    u: NDArray,
    v: NDArray,
    F: NDArray,
    G: NDArray,
    imax: int,
    jmax: int,
    delt: float,
    delx: float,
    dely: float,
    gx: float,
    gy: float,
    gamma: float,
    Re: float,
) -> None:

    shape = (imax + 2, jmax + 2)

    if not u.shape == shape:
        raise ValueError

    if not v.shape == shape:
        raise ValueError

    if not F.shape == (shape):
        raise ValueError

    if not G.shape == shape:
        raise ValueError

    # compute F
    for i in range(1, imax + 1):
        for j in range(1, jmax + 1):

            # fmt: off
            d2udx2 = (u[i + 1, j] - 2 * u[i, j] + u[i - 1, j]) / delx**2
            d2udy2 = (u[i, j + 1] - 2 * u[i, j] + u[i, j - 1]) / dely**2

            du2dx = (
                1 / delx * (
                    ((u[i, j] + u[i + 1, j]) / 2) ** 2
                    - ((u[i - 1, j] + u[i, j]) / 2) ** 2
                ) + gamma / delx * (
                    (np.abs(u[i, j] + u[i + 1, j]) / 2) * ((u[i, j] - u[i + 1, j]) / 2)
                    - (np.abs(u[i - 1, j] + u[i, j]) / 2) * ((u[i - 1, j] - u[i, j]) / 2)
                )
            )

            duvdy = (
                1 / dely * (
                    ((v[i, j] + v[i + 1, j]) / 2) * ((u[i, j] + u[i, j + 1]) / 2)
                    - ((v[i, j - 1] + v[i + 1, j - 1]) / 2) * ((u[i, j - 1] + u[i, j]) / 2)
                ) + gamma / dely * (
                    (np.abs(v[i, j] + v[i + 1, j]) / 2) * ((u[i, j] - u[i, j + 1]) / 2)
                    - (np.abs(v[i, j - 1] + v[i + 1, j - 1]) / 2) * ((u[i, j - 1] - u[i, j]) / 2)
                )
            )

            F[i, j] = u[i, j] + delt * (1 / Re * (d2udx2 + d2udy2) - du2dx - duvdy + gx)
            # fmt: on

    F[0, :] = u[0, :]
    F[imax, :] = u[imax, :]

    # compute G
    for i in range(1, imax + 1):
        for j in range(1, jmax):

            # fmt: off
            d2vdx2 = (v[i + 1, j] - 2 * v[i, j] + v[i - 1, j]) / delx**2
            d2vdy2 = (v[i, j + 1] - 2 * v[i, j] + v[i, j - 1]) / dely**2

            duvdx = (
                1 / delx * (
                    ((u[i, j] + u[i, j + 1]) / 2) * ((v[i, j] + v[i + 1, j]) / 2)
                    - ((u[i - 1, j] + u[i - 1, j + 1]) / 2) * ((v[i - 1, j] + v[i, j]) / 2)
                ) + gamma / delx * (
                    (np.abs(u[i, j] + u[i, j + 1]) / 2) * ((v[i, j] - v[i + 1, j]) / 2)
                    - (np.abs(u[i - 1, j] + u[i - 1, j + 1]) / 2) * ((v[i - 1, j] - v[i, j]) / 2)
                )
            )

            dv2dy = (
                1 / dely * (
                    ((v[i, j] + v[i, j + 1]) / 2) ** 2
                    - ((v[i, j - 1] + v[i, j]) / 2) ** 2
                ) + gamma / dely * (
                    (np.abs(v[i, j] + v[i, j + 1]) / 2) * ((v[i, j] - v[i, j + 1]) / 2)
                    - (np.abs(v[i, j - 1] + v[i, j]) / 2) * ((v[i, j - 1] - v[i, j]) / 2)
                )
            )

            G[i, j] = v[i, j] + delt * (1 / Re * (d2vdx2 + d2vdy2) - duvdx - dv2dy + gy)
            # fmt: on

    G[:, 0] = v[:, 0]
    G[:, jmax] = v[:, jmax]


def comp_rhs(
    F: NDArray,
    G: NDArray,
    RHS: NDArray,
    imax: int,
    jmax: int,
    delt: float,
    delx: float,
    dely: float,
) -> None:

    shape = (imax + 2, jmax + 2)

    if not F.shape == shape:
        raise ValueError

    if not G.shape == shape:
        raise ValueError

    if not RHS.shape == shape:
        raise ValueError

    for i in range(1, imax + 1):
        for j in range(1, jmax + 1):
            # fmt: off
            RHS[i, j] = (
                1 / delt
                * ((F[i, j] - F[i - 1, j]) / delx + (G[i, j] - G[i, j - 1]) / dely)
            )
            # fmt: on


def poisson(
    p: NDArray,
    RHS: NDArray,
    imax: int,
    jmax: int,
    delx: float,
    dely: float,
    eps: float,
    itermax: int,
    omg: float,
    res: NDArray,
) -> tuple[int, float]:

    shape = (imax + 2, jmax + 2)

    if p.shape != shape:
        raise ValueError

    if RHS.shape != shape:
        raise ValueError

    if res.shape != shape:
        raise ValueError

    for it in range(1, itermax + 1):
        p[0, :] = p[1, :]
        p[imax + 1, :] = p[imax, :]
        p[:, 0] = p[:, 1]
        p[:, jmax + 1] = p[:, jmax]

        for i in range(1, imax + 1):

            if i == 1:
                eps_w = 0
            else:
                eps_w = 1

            if i == imax:
                eps_e = 0
            else:
                eps_e = 1

            for j in range(1, jmax + 1):

                if j == 1:
                    eps_s = 0
                else:
                    eps_s = 1

                if j == jmax:
                    eps_n = 0
                else:
                    eps_n = 1

                # fmt: off
                p[i, j] = (
                    (1 - omg) * p[i, j]
                    + omg / ((eps_e + eps_w) / delx**2 + (eps_n + eps_s) / dely**2)
                    * (
                        (eps_e * p[i + 1, j] + eps_w * p[i - 1, j]) / delx**2
                        + (eps_n * p[i, j + 1] + eps_s * p[i, j - 1]) / dely**2
                        - RHS[i, j]
                    )
                )
                # fmt: on

                # fmt: off
                res[i, j] = (
                    (eps_e * (p[i + 1, j] - p[i, j]) - eps_w * (p[i, j] - p[i - 1, j])) / delx**2
                    + (eps_n * (p[i, j + 1] - p[i, j]) - eps_s * (p[i, j] - p[i, j - 1])) / dely**2
                    - RHS[i, j]
                )
                # fmt: on

        res_norm = np.sqrt(1 / (imax * jmax) * (res[1:-1, 1:-1] ** 2).flatten().sum())
        res_norm = 0
        for i in range(1, imax + 1):
            for j in range(1, jmax + 1):
                res_norm = res_norm + res[i, j] ** 2

        res_norm = np.sqrt(1 / imax / jmax * res_norm)
        # res_norm = np.abs(res.flatten()).max()

        if res_norm < eps:
            break

    return it, res_norm


def adap_uv(
    u: NDArray,
    v: NDArray,
    F: NDArray,
    G: NDArray,
    p: NDArray,
    imax: int,
    jmax: int,
    delt: float,
    delx: float,
    dely: float,
) -> None:

    shape = (imax + 2, jmax + 2)

    if u.shape != shape:
        raise ValueError

    if v.shape != shape:
        raise ValueError

    if F.shape != shape:
        raise ValueError

    if G.shape != shape:
        raise ValueError

    if p.shape != shape:
        raise ValueError

    for i in range(1, imax + 1):
        for j in range(1, jmax + 1):
            u[i, j] = F[i, j] - delt / delx * (p[i + 1, j] - p[i, j])
            v[i, j] = G[i, j] - delt / dely * (p[i, j + 1] - p[i, j])
