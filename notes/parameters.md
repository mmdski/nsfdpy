# Problem Parameters and Data Structures

## Geometry data

| Parameter | Description                                    |
| --------- | ---------------------------------------------- |
| `xlength` | domain size in $x$-direction                   |
| `ylength` | domain size in $y$-direction                   |
| `imax`    | number of interior cells in $x$-direction      |
| `jmax`    | number of interior cells in $y$-direction      |
| `delx`    | length $\delta x$ of one cell in $x$-direction |
| `dely`    | length $\delta y$ of one cell in $y$-direction |

## Time-stepping data

| Parameter | Description                                     |
| --------- | ----------------------------------------------- |
| `t`       | current time value                              |
| `t_end`   | final time $t_{end}$                            |
| `delt`    | time step size $\delta t$                       |
| `tau`     | safety factor for time step size control $\tau$ |

## Pressure-iteration data

| Parameter | Description                                            |
| --------- | ------------------------------------------------------ |
| `itermax` | maximal number of pressure iterations in one time step |
| `it`      | SOR iteration counter                                  |
| `res`     | norm of pressure equation residual                     |
| `eps`     | stopping tolerance $eps$ for pressure iteration        |
| `omg`     | relaxation parameter $\omega$ for SOR iteration        |
| `gamma`   | upwind differencing factor $\gamma$                    |

## Problem-dependent quantities

| Parameter | Description                                            |
| --------- | ------------------------------------------------------ |
| `Re` | Reynolds number $Re$ |
| `GX`, `GY` | body forces $g_x$, $g_y$, (e.g., gravity) |
| `UI`, `VI`, `PI` | initial data for velocities and pressure |
| `wW`, `wE`, `wN`, `wS` | boundary condition specification $^1$ |
| `problem` | This variable allows further flow-specific quantities, such as inflow velocity or internal obstacles, to be specified depending on the problem type |

$^1$ `wW`, `wE`, `wN`, `wS` specify the type of boundary condition along the
western (left), eastern (right), northern (upper), and southern (lower)
boundaries of $\Omega = \left[0,\texttt{xlength}\right] \times \left[0,\texttt
{ylength}\right]$; each may have one of the values:

- 1 for free-slip conditions
- 2 for non-slip conditions
- 3 for outflow conditions
- 4 for periodic boundary conditions
