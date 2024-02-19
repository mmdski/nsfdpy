Equations
=========

Continuous Equations
--------------------

--------
Momentum
--------

.. math::

    \frac{\partial u}{\partial t} + \frac{\partial p}{\partial x}
        = \frac{1}{Re}
            \left(
                \frac{\partial^2 u}{\partial x^2}
                + \frac{\partial^2 u}{\partial y^2}
            \right)
            - \frac{\partial\left(u^2\right)}{\partial x}
            - \frac{\partial \left(uv\right)}{\partial y}
            + g_x

.. math::

    \frac{\partial v}{\partial t} + \frac{\partial p}{\partial y}
        = \frac{1}{Re}
            \left(
                \frac{\partial^2 v}{\partial x^2}
                + \frac{\partial^2 v}{\partial y^2}
            \right)
            - \frac{\partial\left(uv\right)}{\partial x}
            - \frac{\partial \left(v^2\right)}{\partial y}
            + g_y

----------
Continuity
----------

.. math::

    \frac{\partial u}{\partial x} + \frac{\partial v}{\partial y} = 0

Discrete Equations
------------------

-------------
Time-Stepping
-------------

.. math::

    u^{\left(n+1\right)} = u^{\left(n\right)}
        + \delta t\left[
            \frac{1}{Re}
                \left(\frac{\partial^2 u}{\partial x^2}
                + \frac{\partial^2 u}{\partial y^2}
                \right)
            - \frac{\partial \left(u^2\right)}{\partial x}
            - \frac{\partial \left(uv\right)}{\partial y}
            + g_x
            - \frac{\partial p}{\partial x}
        \right]

.. math::

    v^{\left(n+1\right)} = v^{\left(n\right)}
        + \delta t\left[
            \frac{1}{Re}
                \left(\frac{\partial^2 v}{\partial x^2}
                + \frac{\partial^2 v}{\partial y^2}
                \right)
            - \frac{\partial \left(uv\right)}{\partial y}
            - \frac{\partial \left(v^2\right)}{\partial x}
            + g_y
            - \frac{\partial p}{\partial y}
        \right]

.. math::

    F := u^{\left(n\right)}
        + \delta t\left[
            \frac{1}{Re}
                \left(\frac{\partial^2 u}{\partial x^2}
                + \frac{\partial^2 u}{\partial y^2}
                \right)
            - \frac{\partial \left(u^2\right)}{\partial x}
            - \frac{\partial \left(uv\right)}{\partial y}
            + g_x
        \right]

.. math::

    G  := v^{\left(n\right)}
        + \delta t\left[
            \frac{1}{Re}
                \left(\frac{\partial^2 v}{\partial x^2}
                + \frac{\partial^2 v}{\partial y^2}
                \right)
            - \frac{\partial \left(uv\right)}{\partial y}
            - \frac{\partial \left(v^2\right)}{\partial x}
            + g_y
        \right]

.. math::

    u^{\left(n+1\right)} = F - \delta t \frac{\partial p}{\partial x}

.. math::

    v^{\left(n+1\right)} = G - \delta t \frac{\partial p}{\partial y}

.. math::

    u^{\left(n+1\right)} = F^{\left(n\right)}
        - \delta t \frac{\partial p^{\left(n+1\right)}}{\partial x}

.. math::

    v^{\left(n+1\right)} = G^{\left(n\right)}
        - \delta t \frac{\partial p^{\left(n+1\right)}}{\partial y}

Poisson Equation for Pressure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. math::

    \frac{\partial^2 p^{\left(n+1\right)}}{\partial x^2}
        + \frac{\partial^2 p^{\left(n+1\right)}}{\partial y^2}
        = \frac{1}{\delta t}
            \left(
                \frac{\partial F^{\left(n\right)}}{\partial x}
                + \frac{\partial G^{\left(n\right)}}{\partial y}
            \right)

---------------------------
Discrete Momentum Equations
---------------------------

.. math::

    F_{i,j} & := u_{i,j} + \delta t \left(
            \frac{1}{Re}
                \left(
                    \left[\frac{\partial^2 u}{\partial x^2}\right]_{i,j}
                    + \left[\frac{\partial^2 u}{\partial y^2}\right]_{i,j}
                \right)
                - \left[\frac{\partial \left(u^2\right)}{\partial x}\right]_{i,j}
                - \left[\frac{\partial \left(uv\right)}{\partial y}\right]_{i,j}
                + g_x
        \right), \\
        & i = 1,\dots, i_{\text{max}} - 1,\hspace{10 pt}j = 1,\dots,j_{\text{max}}

.. math::

    G_{i,j} & := v_{i,j} + \delta t \left(
            \frac{1}{Re}
                \left(
                    \left[\frac{\partial^2 v}{\partial x^2}\right]_{i,j}
                    + \left[\frac{\partial^2 v}{\partial y^2}\right]_{i,j}
                \right)
                - \left[\frac{\partial \left(uv\right)}{\partial x}\right]_{i,j}
                - \left[\frac{\partial \left(v^2\right)}{\partial y}\right]_{i,j}
                + g_y
        \right) \\
        & i = 1,\dots, i_{\text{max}},\hspace{10 pt}j = 1,\dots,j_{\text{max}}-1
