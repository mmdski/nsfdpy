#include <assert.h>
#include <stdlib.h>

#include <nsfd.h>

#include "nsfd_internal.h"

size_t
nsfd_sizeof_real (void)
{
  return sizeof (NSFDReal);
}

void
nsfd_init_uvp (NSFDInitValData *iv_data_p,
               NSFDGridValues  *u_p,
               NSFDGridValues  *v_p,
               NSFDGridValues  *p_p)
{
  assert (iv_data_p);
  assert (u_p && v_p && p_p);

  nsfd_grid_values_init (u_p, iv_data_p->ui);
  nsfd_grid_values_init (v_p, iv_data_p->vi);
  nsfd_grid_values_init (p_p, iv_data_p->pi);
}

NSFDReal
nsfd_comp_delt (NSFDTimeStepData *ts_data_p,
                NSFDProblemData  *prob_data_p,
                NSFDGridValues   *u_p,
                NSFDGridValues   *v_p)
{
  assert (ts_data_p && prob_data_p && u_p && v_p);
  assert (nsfd_grid_geom_data_eq (&u_p->geom_data, &v_p->geom_data));

  NSFDReal u_max = nsfd_grid_values_abs_max (u_p);
  NSFDReal v_max = nsfd_grid_values_abs_max (v_p);
  NSFDReal delx  = u_p->geom_data.xlength / (NSFDReal) u_p->geom_data.imax;
  NSFDReal dely  = v_p->geom_data.ylength / (NSFDReal) v_p->geom_data.jmax;
  NSFDReal tau   = ts_data_p->tau;
  if (tau < 0)
    tau = 1;

  NSFDReal v1 =
      prob_data_p->Re / 2.0 * 1 / (1 / (delx * delx) + 1 / (dely * dely));
  NSFDReal v2 = delx / u_max;
  NSFDReal v3 = dely / v_max;

  NSFDReal min_v = nsfd_real_min (nsfd_real_min (v1, v2), v3);

  return tau * min_v;
}

void
nsfd_set_b_cond (NSFDGridValues *u_p,
                 NSFDGridValues *v_p,
                 NSFDGridValues *p_p,
                 NSFDBCondData  *b_cond_data_p)
{
  assert (u_p && v_p && p_p && b_cond_data_p);
  assert (nsfd_grid_geom_data_eq (&u_p->geom_data, &v_p->geom_data));

  // check for periodic boundary condition consistency
  if (b_cond_data_p->wE == kBCondPeriodic ||
      b_cond_data_p->wW == kBCondPeriodic)
    {
      assert (b_cond_data_p->wE == kBCondPeriodic &&
              b_cond_data_p->wW == kBCondPeriodic);
    }
  if (b_cond_data_p->wN == kBCondPeriodic ||
      b_cond_data_p->wS == kBCondPeriodic)
    {
      assert (b_cond_data_p->wN == kBCondPeriodic &&
              b_cond_data_p->wS == kBCondPeriodic);
    }

  size_t imax = u_p->geom_data.imax;
  size_t jmax = u_p->geom_data.jmax;

  NSFDReal *u = u_p->values;
  NSFDReal *v = v_p->values;
  NSFDReal *p = p_p->values;

  // west (left) boundary condition
  switch (b_cond_data_p->wW)
    {
    case kBCondNone:
      break;
    case kBCondNoSlip:
      for (size_t j = 1; j <= jmax; ++j)
        {
          u[NSFD_GRID_INDEX (imax, 0, j)] = 0;
          v[NSFD_GRID_INDEX (imax, 0, j)] = -v[NSFD_GRID_INDEX (imax, 1, j)];
        }
      break;
    case kBCondFreeSlip:
      for (size_t j = 1; j <= jmax; ++j)
        {
          u[NSFD_GRID_INDEX (imax, 0, j)] = 0;
          v[NSFD_GRID_INDEX (imax, 0, j)] = v[NSFD_GRID_INDEX (imax, 1, j)];
        }
      break;
    case kBCondOutflow:
      for (size_t j = 1; j <= jmax; ++j)
        {
          u[NSFD_GRID_INDEX (imax, 0, j)] = u[NSFD_GRID_INDEX (imax, 1, j)];
          v[NSFD_GRID_INDEX (imax, 0, j)] = v[NSFD_GRID_INDEX (imax, 1, j)];
        }
      break;
    case kBCondPeriodic:
      for (size_t j = 1; j <= jmax; ++j)
        {
          u[NSFD_GRID_INDEX (imax, 0, j)] =
              u[NSFD_GRID_INDEX (imax, imax - 1, j)];
          u[NSFD_GRID_INDEX (imax, imax, j)] = u[NSFD_GRID_INDEX (imax, 1, j)];

          p[NSFD_GRID_INDEX (imax, 1, j)] = p[NSFD_GRID_INDEX (imax, imax, j)];

          v[NSFD_GRID_INDEX (imax, 0, j)] =
              v[NSFD_GRID_INDEX (imax, imax - 1, j)];
          v[NSFD_GRID_INDEX (imax, 1, j)] = v[NSFD_GRID_INDEX (imax, imax, j)];
          v[NSFD_GRID_INDEX (imax, imax + 1, j)] =
              v[NSFD_GRID_INDEX (imax, 2, j)];
        }
      break;
    }

  // east (right) boundary condition
  switch (b_cond_data_p->wE)
    {
    case kBCondNone:
      break;
    case kBCondNoSlip:
      for (size_t j = 1; j <= jmax; ++j)
        {
          u[NSFD_GRID_INDEX (imax, imax, j)] = 0;
          v[NSFD_GRID_INDEX (imax, imax + 1, j)] =
              -v[NSFD_GRID_INDEX (imax, imax, j)];
        }
      break;
    case kBCondFreeSlip:
      for (size_t j = 1; j <= jmax; ++j)
        {
          u[NSFD_GRID_INDEX (imax, imax, j)] = 0;
          v[NSFD_GRID_INDEX (imax, imax + 1, j)] =
              v[NSFD_GRID_INDEX (imax, imax, j)];
        }
      break;
    case kBCondOutflow:
      for (size_t j = 1; j <= jmax; ++j)
        {
          u[NSFD_GRID_INDEX (imax, imax, j)] =
              u[NSFD_GRID_INDEX (imax, imax - 1, j)];
          v[NSFD_GRID_INDEX (imax, imax + 1, j)] =
              v[NSFD_GRID_INDEX (imax, 1, j)];
        }
      break;
    case kBCondPeriodic:
      // already set in east boundary condition
      break;
    }

  // north (upper) boundary condition
  switch (b_cond_data_p->wN)
    {
    case kBCondNone:
      break;
    case kBCondNoSlip:
      for (size_t i = 0; i <= imax; ++i)
        {
          v[NSFD_GRID_INDEX (imax, i, jmax)] = 0;
          u[NSFD_GRID_INDEX (imax, i, jmax + 1)] =
              -u[NSFD_GRID_INDEX (imax, i, jmax)];
        }
      break;
    case kBCondFreeSlip:
      for (size_t i = 0; i <= imax; ++i)
        {
          v[NSFD_GRID_INDEX (imax, i, jmax)] = 0;
          u[NSFD_GRID_INDEX (imax, i, jmax + 1)] =
              u[NSFD_GRID_INDEX (imax, i, jmax)];
        }
      break;
    case kBCondOutflow:
      for (size_t i = 1; i <= imax; ++i)
        {
          u[NSFD_GRID_INDEX (imax, i, jmax + 1)] =
              u[NSFD_GRID_INDEX (imax, i, jmax)];
          v[NSFD_GRID_INDEX (imax, i, jmax)] =
              v[NSFD_GRID_INDEX (imax, i, jmax - 1)];
        }
      break;
    case kBCondPeriodic:
      for (size_t i = 1; i <= imax; ++i)
        {
          u[NSFD_GRID_INDEX (imax, i, 0)] =
              u[NSFD_GRID_INDEX (imax, i, jmax - 1)];
          u[NSFD_GRID_INDEX (imax, i, jmax)] = u[NSFD_GRID_INDEX (imax, i, 1)];
          u[NSFD_GRID_INDEX (imax, i, jmax + 1)] =
              u[NSFD_GRID_INDEX (imax, i, 2)];

          p[NSFD_GRID_INDEX (imax, i, 1)] = p[NSFD_GRID_INDEX (imax, i, jmax)];

          v[NSFD_GRID_INDEX (imax, i, 0)] =
              v[NSFD_GRID_INDEX (imax, i, jmax - 1)];
          v[NSFD_GRID_INDEX (imax, i, 1)] = v[NSFD_GRID_INDEX (imax, i, jmax)];
        }
      break;
    }

  // south (lower) boundary condition
  switch (b_cond_data_p->wS)
    {
    case kBCondNone:
      break;
    case kBCondNoSlip:
      for (size_t i = 0; i <= imax; ++i)
        {
          v[NSFD_GRID_INDEX (imax, i, 0)] = 0;
          u[NSFD_GRID_INDEX (imax, i, 0)] = -u[NSFD_GRID_INDEX (imax, i, 1)];
        }
      break;
    case kBCondFreeSlip:
      for (size_t i = 0; i <= imax; ++i)
        {
          v[NSFD_GRID_INDEX (imax, i, 0)] = 0;
          u[NSFD_GRID_INDEX (imax, i, 0)] = u[NSFD_GRID_INDEX (imax, i, 1)];
        }
      break;
    case kBCondOutflow:
      for (size_t i = 1; i <= imax; ++i)
        {
          u[NSFD_GRID_INDEX (imax, i, 0)] = u[NSFD_GRID_INDEX (imax, i, 1)];
          v[NSFD_GRID_INDEX (imax, i, 0)] = v[NSFD_GRID_INDEX (imax, i, 1)];
        }
      break;
    case kBCondPeriodic:
      // already set in the north boundary condition
      break;
    }
}

void
nsfd_comp_fg (NSFDGridValues   *u_p,
              NSFDGridValues   *v_p,
              NSFDGridValues   *f_p,
              NSFDGridValues   *g_p,
              NSFDTimeStepData *ts_p,
              NSFDProblemData  *prob_p)
{
  assert (u_p && v_p && f_p && g_p && ts_p && prob_p);
}
