C API
=====

Types and Structures
--------------------

.. c:type:: NSFDReal

    .. code-block:: c

        typedef double NSFDReal;

.. c:type:: NSFGridGeomData

    .. c:member:: size_t imax
    .. c:member:: size_t jmax
    .. c:member:: NSFDReal xlength
    .. c:member:: NSFDReal ylength

.. c:type:: NSFDGridGeom

.. c:type:: NSFDGridValues

.. c:type:: NSFDTimeStepData

    .. c:member:: NSFDReal t
    .. c:member:: NSFDReal t_end
    .. c:member:: NSFDReal delt
    .. c:member:: NSFDReal tau

.. c:type:: NSFDInitValData

    .. c:member:: NSFDReal ui
    .. c:member:: NSFDReal vi
    .. c:member:: NSFDReal pi

.. c:type:: NSFDProblemData

    .. c:member:: NSFDReal Re
    .. c:member:: NSFDReal gx
    .. c:member:: NSFDReal gy

.. c:type:: NSFDBCondType

    .. c:enumerator:: kBCondNone
    .. c:enumerator:: kBCondFreeSlip
    .. c:enumerator:: kBCondNoSlip
    .. c:enumerator:: kBCondOutflow
    .. c:enumerator:: kBCondPeriodic

.. c:type:: NSFDBCondData

    .. c:member:: NSFDBCondType wW
    .. c:member:: NSFDBCondType wE
    .. c:member:: NSFDBCondType wN
    .. c:member:: NSFDBCondType wS

Functions
---------

.. c:function:: void nsfd_init_uvp (NSFDInitValData *iv_data_p, \
                                   NSFDGridValues *u_p, \
                                   NSFDGridValues *v_p, \
                                   NSFDGridValues *p_p);

    Initializes u, v, and p from initialization data in iv_data.


.. c:function:: NSFDReal nsfd_comp_delt(NSFDTimeStepData *ts_data_p, \
                                        NSFDProblemData  *prob_data_p, \
                                        NSFDGridValues   *u_p, \
                                        NSFDGridValues   *v_p);

    Computes :math:`\delta t` from

    .. math:: \delta t := \tau \min\left(
                \frac{Re}{2}\left(
                    \frac{1}{\delta x^2}+\frac{1}{\delta y^2}
                    \right)^{-1},
                \frac{\delta x}{\lvert u_{\text{max}}\rvert },
                \frac{\delta y}{\lvert v_{\text{max}}\rvert }
            \right)

.. c:function:: void nsfd_set_b_cond (NSFDGridValues *u_p, \
                                      NSFDGridValues *v_p, \
                                      NSFDGridValues *p_p, \
                                      NSFDBCondData  *b_cond_data_p);

    Sets boundary east (left), west (right), north (upper), and south (lower)
    boundary conditions.

.. c:function:: void nsfd_comp_fg (NSFDGridValues *u_p, \
                                   NSFDGridValues *v_p, \
                                   NSFDGridValues *f_p, \
                                   NSFDGridValues *g_p, \
                                   NSFDProblemData *prob_data_p)

    Computes :math:`F` and :math:`G`
