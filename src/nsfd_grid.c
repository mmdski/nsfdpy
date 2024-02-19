#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <nsfd.h>

#include "nsfd_internal.h"
#include "nsfd_mem.h"

NSFDReal
nsfd_grid_values_abs_max (NSFDGridValues *values_p)
{
  assert (values_p);

  NSFDReal abs_max = 0;
  NSFDReal abs_value;

  size_t n_cells = nsfd_grid_values_n_cells (values_p);

  for (size_t i = 0; i < n_cells; ++i)
    {
      abs_value = nsfd_real_abs (values_p->values[i]);
      if (abs_value > abs_max)
        abs_max = abs_value;
    }

  return abs_max;
}

NSFDGridGeom *
nsfd_grid_geom_new (NSFDGridGeomData *geom_data_p)
{
  assert (geom_data_p);
  assert (geom_data_p->imax > 0 && geom_data_p->jmax > 0);
  assert (geom_data_p->xlength > 0 && geom_data_p->xlength > 0);

  size_t n_cells = nsfd_size_t_mult (geom_data_p->imax + N_B_CELLS,
                                     geom_data_p->jmax + N_B_CELLS,
                                     __FILE__,
                                     __LINE__);
  size_t cells_size =
      nsfd_size_t_mult (n_cells, sizeof (NSFDGridCoord), __FILE__, __LINE__);
  size_t size =
      nsfd_size_t_add (cells_size, sizeof (NSFDGridGeom), __FILE__, __LINE__);

  NSFDGridGeom *grid_geom_p = nsfd_mem_malloc (size);

  grid_geom_p->geom_data.imax    = geom_data_p->imax;
  grid_geom_p->geom_data.jmax    = geom_data_p->jmax;
  grid_geom_p->geom_data.xlength = geom_data_p->xlength;
  grid_geom_p->geom_data.ylength = geom_data_p->ylength;

  return grid_geom_p;
}

void
nsfd_grid_geom_free (NSFDGridGeom **grid_geom_pp)
{
  nsfd_mem_free ((void **) grid_geom_pp);
}

static void
nsfd_grid_geom_init (NSFDGridGeom *grid_geom_p,
                     NSFDReal      x_offset,
                     NSFDReal      y_offset)
{
  assert (grid_geom_p);

  NSFDGridGeomData *geom_data_p = &grid_geom_p->geom_data;

  size_t   imax      = geom_data_p->imax;
  NSFDReal real_imax = nsfd_size_t_to_real (imax, __FILE__, __LINE__);
  NSFDReal real_jmax =
      nsfd_size_t_to_real (geom_data_p->jmax, __FILE__, __LINE__);

  NSFDReal delx = geom_data_p->xlength / real_imax;
  NSFDReal dely = geom_data_p->ylength / real_jmax;

  NSFDGridCoord *coord_p;

  for (size_t i = 0; i < geom_data_p->imax + N_B_CELLS; ++i)
    {
      for (size_t j = 0; j < geom_data_p->jmax + N_B_CELLS; ++j)
        {
          NSFDReal real_i = nsfd_size_t_to_real (i, __FILE__, __LINE__);
          NSFDReal real_j = nsfd_size_t_to_real (j, __FILE__, __LINE__);
          coord_p         = &grid_geom_p->coords[NSFD_GRID_INDEX (imax, i, j)];
          coord_p->x      = (real_i - x_offset) * delx;
          coord_p->y      = (real_j - y_offset) * dely;
        }
    }
}

void
nsfd_grid_geom_init_p (NSFDGridGeom *grid_geom_p)
{
  assert (grid_geom_p);
  nsfd_grid_geom_init (grid_geom_p, 0.5, 0.5);
}

void
nsfd_grid_geom_init_u (NSFDGridGeom *grid_geom_p)
{
  assert (grid_geom_p);
  nsfd_grid_geom_init (grid_geom_p, 0.0, 0.5);
}

void
nsfd_grid_geom_init_v (NSFDGridGeom *grid_geom_p)
{
  assert (grid_geom_p);
  nsfd_grid_geom_init (grid_geom_p, 0.5, 0.0);
}

size_t
nsfd_grid_geom_n_cells (NSFDGridGeom *grid_geom_p)
{
  assert (grid_geom_p);
  return (grid_geom_p->geom_data.imax + N_B_CELLS) *
         (grid_geom_p->geom_data.jmax + N_B_CELLS);
}

void
nsfd_grid_geom_coords (NSFDGridGeom *grid_geom_p, NSFDReal *x, NSFDReal *y)
{
  assert (grid_geom_p);
  assert (x);
  assert (y);

  size_t length = (grid_geom_p->geom_data.imax + N_B_CELLS) *
                  (grid_geom_p->geom_data.jmax + N_B_CELLS);
  NSFDGridCoord *coord_p;
  for (size_t i = 0; i < length; ++i)
    {
      coord_p = &grid_geom_p->coords[i];
      x[i]    = coord_p->x;
      y[i]    = coord_p->y;
    }
}

NSFDGridValues *
nsfd_grid_values_new (NSFDGridGeomData *geom_data_p)
{
  assert (geom_data_p);
  assert (geom_data_p->imax > 0 && geom_data_p->jmax);
  assert (geom_data_p->xlength > 0 && geom_data_p->xlength);

  size_t n_cells = nsfd_size_t_mult (geom_data_p->imax + N_B_CELLS,
                                     geom_data_p->jmax + N_B_CELLS,
                                     __FILE__,
                                     __LINE__);
  size_t cells_size =
      nsfd_size_t_mult (n_cells, sizeof (NSFDReal), __FILE__, __LINE__);
  size_t size =
      nsfd_size_t_add (cells_size, sizeof (NSFDGridValues), __FILE__, __LINE__);

  NSFDGridValues *grid_values_p = nsfd_mem_malloc (size);

  grid_values_p->geom_data.imax    = geom_data_p->imax;
  grid_values_p->geom_data.jmax    = geom_data_p->jmax;
  grid_values_p->geom_data.xlength = geom_data_p->xlength;
  grid_values_p->geom_data.ylength = geom_data_p->ylength;

  return grid_values_p;
}

void
nsfd_grid_values_free (NSFDGridValues **grid_values_p)
{
  nsfd_mem_free ((void **) grid_values_p);
}

size_t
nsfd_grid_values_n_cells (NSFDGridValues *grid_values_p)
{
  assert (grid_values_p);
  return (grid_values_p->geom_data.imax + N_B_CELLS) *
         (grid_values_p->geom_data.jmax + N_B_CELLS);
}

void
nsfd_grid_values_init (NSFDGridValues *grid_values_p, NSFDReal init_value)
{
  assert (grid_values_p);

  size_t n_cells = nsfd_grid_values_n_cells (grid_values_p);
  for (size_t i = 0; i < n_cells; ++i)
    {
      grid_values_p->values[i] = init_value;
    }
}

void
nsfd_grid_values (NSFDGridValues *grid_values_p, NSFDReal *values)
{
  assert (grid_values_p);
  assert (values);

  size_t n_cells = nsfd_grid_values_n_cells (grid_values_p);
  for (size_t i = 0; i < n_cells; ++i)
    {
      values[i] = grid_values_p->values[i];
    }
}
