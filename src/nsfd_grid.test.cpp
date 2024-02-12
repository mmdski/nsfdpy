#include <gtest/gtest.h>

#define GRID_INDEX(n_cols, row, col) (row * n_cols + col)

extern "C"
{
#include <nsfd.h>
}

namespace
{
TEST (NSFDGrid, new)
{
  size_t    m_rows = 10;
  size_t    n_cols = 10;
  NSFDGrid *grid_p = nsfd_grid_new (m_rows, n_cols);

  nsfd_grid_free (&grid_p);
  ASSERT_FALSE (grid_p);
}

TEST (NSFDGrid, shape)
{
  size_t        m_rows      = 10;
  size_t        n_cols      = 10;
  NSFDGrid     *grid_p      = nsfd_grid_new (m_rows, n_cols);
  NSFDGridShape field_shape = nsfd_grid_shape (grid_p);

  ASSERT_EQ (field_shape.m_rows, m_rows);
  ASSERT_EQ (field_shape.n_cols, n_cols);
  nsfd_grid_free (&grid_p);
}

TEST (NSFDGrid, size)
{
  size_t    m_rows = 10;
  size_t    n_cols = 10;
  NSFDGrid *grid_p = nsfd_grid_new (m_rows, n_cols);
  size_t    size   = nsfd_grid_size (grid_p);

  ASSERT_EQ (size, m_rows * n_cols);
  nsfd_grid_free (&grid_p);
}

TEST (NSFDGrid, init_const)
{
  size_t    m_rows = 10;
  size_t    n_cols = 10;
  NSFDGrid *grid_p = nsfd_grid_new (m_rows, n_cols);

  NSFDReal const_field_value = 1;
  nsfd_grid_init_const (grid_p, const_field_value);

  NSFDReal *field = new NSFDReal[m_rows * n_cols];

  nsfd_grid_get_values (grid_p, field);

  for (size_t i = 0; i < m_rows * n_cols; ++i)
    {
      ASSERT_EQ (field[i], const_field_value);
    }

  delete[] field;
  nsfd_grid_free (&grid_p);
}

TEST (NSFDGrid, init_grid)
{
  size_t    m_rows = 11;
  size_t    n_cols = 11;
  NSFDGrid *grid_p = nsfd_grid_new (m_rows, n_cols);

  NSFDReal x_min = -5, x_max = 5;
  NSFDReal y_min = 0, y_max = 10;
  NSFDReal x_range = x_max - x_min;
  NSFDReal y_range = y_max - y_min;

  NSFDReal dx = x_range / (n_cols - 1);
  NSFDReal dy = y_range / (m_rows - 1);

  NSFDReal *x_arr = new NSFDReal[m_rows * n_cols];
  NSFDReal *y_arr = new NSFDReal[m_rows * n_cols];

  for (size_t i = 0; i < m_rows; ++i)
    {
      y_arr[i] = y_max - dy * i;
    }

  for (size_t j = 0; j < n_cols; ++j)
    {
      x_arr[j] = x_min + dx * j;
    }

  nsfd_grid_init_grid (grid_p, x_min, x_max, y_min, y_max);

  NSFDReal *grid_x = new NSFDReal[m_rows * n_cols];
  NSFDReal *grid_y = new NSFDReal[m_rows * n_cols];
  nsfd_grid_get_coords (grid_p, grid_x, grid_y);

  size_t grid_idx;

  for (size_t i = 0; i < m_rows; i++)
    {
      for (size_t j = 0; j < n_cols; j++)
        {
          grid_idx = GRID_INDEX (n_cols, i, j);
          ASSERT_EQ (x_arr[j], grid_x[grid_idx]);
          ASSERT_EQ (y_arr[i], grid_y[grid_idx]);
        }
    }

  delete[] y_arr;
  delete[] x_arr;
  delete[] grid_y;
  delete[] grid_x;
  nsfd_grid_free (&grid_p);
}

}

int
main (int argc, char **argv)
{
  ::testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
