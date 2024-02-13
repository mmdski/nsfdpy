Parameters
==========

Geometry data
-------------
.. list-table::
    :widths: 40 40
    :header-rows: 0
    :align: left

    * - :math:`\texttt{xlength}`
      - domain size in :math:`x`-direction
    * - :math:`\texttt{ylength}`
      - domain size in :math:`y`-direction
    * - :math:`\texttt{imax}`
      - number of interior cells in :math:`x`-direction
    * - :math:`\texttt{jmax}`
      - number of interior cells in :math:`y`-direction
    * - :math:`\texttt{delx}`
      - length :math:`\delta x` of one cell in :math:`x`-direction
    * - :math:`\texttt{dely}`
      - length :math:`\delta y` of one cell in :math:`y`-direction

Time-stepping data
------------------
.. list-table::
    :widths: 40 40
    :header-rows: 0
    :align: left

    * - :math:`\texttt{t}`
      - current time value
    * - :math:`\texttt{t_end}`
      - final time :math:`t_{\text{end}}`
    * - :math:`\texttt{delt}`
      - time step size :math:`\delta t`
    * - :math:`\texttt{tau}`
      - safety factor for time step size control :math:`\tau`

Pressure-iteration data
-----------------------
.. list-table::
    :widths: 25 40
    :header-rows: 0
    :align: left

    * - :math:`\texttt{itermax}`
      - maximal number of pressure iterations in one time step
    * - :math:`\texttt{it}`
      - SOR iteration counter
    * - :math:`\texttt{res}`
      - norm of pressure equation residual
    * - :math:`\texttt{eps}`
      - stopping tolerance :math:`\epsilon` for pressure iteration
    * - :math:`\texttt{omg}`
      - relaxation parameter :math:`\omega` for SOR iteration
    * - :math:`\texttt{gamma}`
      - upwind differencing factor :math:`\gamma`

Problem-dependent quantities
----------------------------
.. list-table::
    :widths: 25 40
    :header-rows: 0
    :align: left

    * - :math:`\texttt{Re}`
      - Reynolds number :math:`Re`
    * - :math:`\texttt{GX}`, :math:`\texttt{GX}`
      - body forces :math:`g_x`, :math:`g_y`, (e.g., gravity)
    * - :math:`\texttt{UI}`, :math:`\texttt{VI}`, :math:`\texttt{PI}`
      - initial data for velocities and pressure
    * - :math:`\texttt{wW}`, :math:`\texttt{wE}`, :math:`\texttt{wN}`,
        :math:`\texttt{wN}`
      - western (left), eastern (right), northern (upper), and southern (lower)
        boundaries of :math:`\Omega = \left[0,\texttt{xlength}\right]
        \times \left[0,\texttt{ylength}\right]`; each may have one of the
        values:

        .. list-table::
            :widths: 20
            :header-rows: 0
            :align: left

            * - 1 for free-slip conditions
            * - 2 for no-slip conditions
            * - 3 for outflow conditions
            * - 4 for periodic boundary conditions
    * - :math:`\texttt{problem}`
      - This variable allows further flow-specific quantities, such as inflow
        velocity or internal obstacles, to be specified depending on the problem
        type
