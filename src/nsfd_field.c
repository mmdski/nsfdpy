#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "nsfd_field_internal.h"
#include "nsfd_mem.h"

NSFDScalarField *
nsfd_s_field_new (size_t m_rows, size_t n_cols)
{
  assert (m_rows > 0 && n_cols > 0);

  NSFDScalarField *s_field_p = nsfd_mem_malloc (
      sizeof (NSFDScalarField) + m_rows * n_cols * sizeof (NSFDFieldSValue));

  s_field_p->shape.m_rows = m_rows;
  s_field_p->shape.n_cols = n_cols;

  return s_field_p;
}

void
nsfd_s_field_free (NSFDScalarField **s_field_pp)
{
  nsfd_mem_free ((void **) s_field_pp);
}

NSFDGridShape
nsfd_s_field_shape (NSFDScalarField *s_field_p)
{
  assert (s_field_p);
  return s_field_p->shape;
}

size_t
nsfd_s_field_size (NSFDScalarField *s_field_p)
{
  assert (s_field_p);
  return s_field_p->shape.m_rows * s_field_p->shape.n_cols;
}

void
nsfd_s_field_init_const (NSFDScalarField *s_field_p, NSFDReal c)
{
  assert (s_field_p);
  size_t n_cols = s_field_p->shape.n_cols;

  for (size_t i = 0; i < s_field_p->shape.m_rows; ++i)
    {
      for (size_t j = 0; j < s_field_p->shape.n_cols; ++j)
        {
          s_field_p->values[NSFD_F_INDEX (n_cols, i, j)].s = c;
        }
    }
}

void
nsfd_s_field_init_grid (NSFDScalarField *s_field_p,
                        NSFDReal         x_min,
                        NSFDReal         x_max,
                        NSFDReal         y_min,
                        NSFDReal         y_max)
{
  assert (s_field_p);
  assert (isfinite (x_min) && isfinite (x_max) && isfinite (y_min) &&
          isfinite (y_max));
  assert (x_max > x_min && y_max > y_min);

  NSFDGridShape shape = s_field_p->shape;

  NSFDReal x_range = x_max - x_min;
  NSFDReal y_range = y_max - y_min;

  NSFDReal dx = x_range / (shape.n_cols - 1);
  NSFDReal dy = y_range / (shape.m_rows - 1);

  size_t arr_idx;

  for (size_t i = 0; i < shape.m_rows; ++i)
    {
      for (size_t j = 0; j < shape.n_cols; ++j)
        {
          arr_idx = NSFD_F_INDEX (shape.n_cols, i, j);
          s_field_p->values[arr_idx].coord.x = x_min + j * dx;
          s_field_p->values[arr_idx].coord.y = y_max - i * dy;
        }
    }
}

NSFDReal
nsfd_s_field_value (NSFDScalarField *s_field_p, size_t i, size_t j)
{
  assert (s_field_p);
  assert (i < s_field_p->shape.m_rows);
  assert (j < s_field_p->shape.n_cols);
  return s_field_p->values[NSFD_F_INDEX (s_field_p->shape.n_cols, i, j)].s;
}

void
nsfd_s_field_get_field (NSFDScalarField *s_field_p, NSFDReal *field)
{
  assert (s_field_p);
  assert (field);

  size_t n_cols = s_field_p->shape.n_cols;
  size_t arr_idx;

  for (size_t i = 0; i < s_field_p->shape.m_rows; ++i)
    {
      for (size_t j = 0; j < s_field_p->shape.n_cols; ++j)
        {
          arr_idx        = NSFD_F_INDEX (n_cols, i, j);
          field[arr_idx] = s_field_p->values[arr_idx].s;
        }
    }
}

void
nsfd_s_field_get_grid (NSFDScalarField *s_field_p,
                       NSFDReal        *grid_x,
                       NSFDReal        *grid_y)
{
  assert (s_field_p);
  assert (grid_x && grid_y);

  size_t n_cols = s_field_p->shape.n_cols;
  size_t arr_idx;

  for (size_t i = 0; i < s_field_p->shape.m_rows; ++i)
    {
      for (size_t j = 0; j < s_field_p->shape.n_cols; ++j)
        {
          arr_idx         = NSFD_F_INDEX (n_cols, i, j);
          grid_x[arr_idx] = s_field_p->values[arr_idx].coord.x;
          grid_y[arr_idx] = s_field_p->values[arr_idx].coord.y;
        }
    }
}
