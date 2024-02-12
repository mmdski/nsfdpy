#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "nsfd_grid_internal.h"
#include "nsfd_mem.h"

NSFDGrid *
nsfd_grid_new (size_t m_rows, size_t n_cols)
{
  assert (m_rows > 0 && n_cols > 0);

  NSFDGrid *grid_p = nsfd_mem_malloc (sizeof (NSFDGrid) +
                                      m_rows * n_cols * sizeof (NSFDGridValue));

  grid_p->shape.m_rows = m_rows;
  grid_p->shape.n_cols = n_cols;

  return grid_p;
}

void
nsfd_grid_free (NSFDGrid **grid_pp)
{
  nsfd_mem_free ((void **) grid_pp);
}

NSFDGridShape
nsfd_grid_shape (NSFDGrid *grid_p)
{
  assert (grid_p);
  return grid_p->shape;
}

size_t
nsfd_grid_size (NSFDGrid *grid_p)
{
  assert (grid_p);
  return grid_p->shape.m_rows * grid_p->shape.n_cols;
}

void
nsfd_grid_init_const (NSFDGrid *grid_p, NSFDReal c)
{
  assert (grid_p);
  size_t n_cols = grid_p->shape.n_cols;

  for (size_t i = 0; i < grid_p->shape.m_rows; ++i)
    {
      for (size_t j = 0; j < grid_p->shape.n_cols; ++j)
        {
          grid_p->values[NSFD_F_INDEX (n_cols, i, j)].s = c;
        }
    }
}

void
nsfd_grid_init_grid (NSFDGrid *grid_p,
                     NSFDReal  x_min,
                     NSFDReal  x_max,
                     NSFDReal  y_min,
                     NSFDReal  y_max)
{
  assert (grid_p);
  assert (isfinite (x_min) && isfinite (x_max) && isfinite (y_min) &&
          isfinite (y_max));
  assert (x_max > x_min && y_max > y_min);

  NSFDGridShape shape = grid_p->shape;

  NSFDReal x_range = x_max - x_min;
  NSFDReal y_range = y_max - y_min;

  NSFDReal dx = x_range / (shape.n_cols - 1);
  NSFDReal dy = y_range / (shape.m_rows - 1);

  size_t arr_idx;

  for (size_t i = 0; i < shape.m_rows; ++i)
    {
      for (size_t j = 0; j < shape.n_cols; ++j)
        {
          arr_idx                         = NSFD_F_INDEX (shape.n_cols, i, j);
          grid_p->values[arr_idx].coord.x = x_min + j * dx;
          grid_p->values[arr_idx].coord.y = y_max - i * dy;
        }
    }
}

NSFDReal
nsfd_grid_value (NSFDGrid *grid_p, size_t i, size_t j)
{
  assert (grid_p);
  assert (i < grid_p->shape.m_rows);
  assert (j < grid_p->shape.n_cols);
  return grid_p->values[NSFD_F_INDEX (grid_p->shape.n_cols, i, j)].s;
}

void
nsfd_grid_get_values (NSFDGrid *grid_p, NSFDReal *field)
{
  assert (grid_p);
  assert (field);

  size_t n_cols = grid_p->shape.n_cols;
  size_t arr_idx;

  for (size_t i = 0; i < grid_p->shape.m_rows; ++i)
    {
      for (size_t j = 0; j < grid_p->shape.n_cols; ++j)
        {
          arr_idx        = NSFD_F_INDEX (n_cols, i, j);
          field[arr_idx] = grid_p->values[arr_idx].s;
        }
    }
}

void
nsfd_grid_get_coords (NSFDGrid *grid_p, NSFDReal *grid_x, NSFDReal *grid_y)
{
  assert (grid_p);
  assert (grid_x && grid_y);

  size_t n_cols = grid_p->shape.n_cols;
  size_t arr_idx;

  for (size_t i = 0; i < grid_p->shape.m_rows; ++i)
    {
      for (size_t j = 0; j < grid_p->shape.n_cols; ++j)
        {
          arr_idx         = NSFD_F_INDEX (n_cols, i, j);
          grid_x[arr_idx] = grid_p->values[arr_idx].coord.x;
          grid_y[arr_idx] = grid_p->values[arr_idx].coord.y;
        }
    }
}
