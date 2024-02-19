#include <gtest/gtest.h>

#define GRID_INDEX(imax, i, j) (j * (imax + 2) + i)

extern "C"
{
#include <nsfd.h>
}

namespace
{

TEST (NSFDGridGeom, new)
{
  NSFDGridGeomData geom_data   = { 10, 10, 100, 100 };
  NSFDGridGeom    *grid_geom_p = nsfd_grid_geom_new (&geom_data);
  nsfd_grid_geom_free (&grid_geom_p);
}

TEST (NSFDGridGeom, init_p)
{
  NSFDGridGeomData geom_data   = { 10, 10, 100, 100 };
  NSFDGridGeom    *grid_geom_p = nsfd_grid_geom_new (&geom_data);
  nsfd_grid_geom_init_p (grid_geom_p);
  nsfd_grid_geom_free (&grid_geom_p);
  ASSERT_EQ ((size_t) grid_geom_p, NULL);
}

TEST (NSFDGridGeom, init_u)
{
  NSFDGridGeomData geom_data   = { 10, 10, 100, 100 };
  NSFDGridGeom    *grid_geom_p = nsfd_grid_geom_new (&geom_data);
  nsfd_grid_geom_init_u (grid_geom_p);
  nsfd_grid_geom_free (&grid_geom_p);
  ASSERT_EQ ((size_t) grid_geom_p, NULL);
}

TEST (NSFDGridGeom, init_v)
{
  NSFDGridGeomData geom_data   = { 10, 10, 100, 100 };
  NSFDGridGeom    *grid_geom_p = nsfd_grid_geom_new (&geom_data);
  nsfd_grid_geom_init_v (grid_geom_p);
  nsfd_grid_geom_free (&grid_geom_p);
  ASSERT_EQ ((size_t) grid_geom_p, NULL);
}

TEST (NSFDGridGeom, n_cells)
{
  size_t           imax        = 10;
  size_t           jmax        = 15;
  NSFDGridGeomData geom_data   = { imax, jmax, 100, 100 };
  NSFDGridGeom    *grid_geom_p = nsfd_grid_geom_new (&geom_data);
  nsfd_grid_geom_init_p (grid_geom_p);
  size_t n_cells = nsfd_grid_geom_n_cells (grid_geom_p);
  ASSERT_EQ (n_cells, (imax + 2) * (jmax + 2));
  nsfd_grid_geom_free (&grid_geom_p);
}

TEST (NSFDGridGeom, coords)
{
  size_t           imax        = 10;
  size_t           jmax        = 15;
  NSFDGridGeomData geom_data   = { imax, jmax, 100, 100 };
  NSFDGridGeom    *grid_geom_p = nsfd_grid_geom_new (&geom_data);
  nsfd_grid_geom_init_p (grid_geom_p);

  size_t    n_cells  = nsfd_grid_geom_n_cells (grid_geom_p);
  NSFDReal *x_coords = new NSFDReal[n_cells];
  NSFDReal *y_coords = new NSFDReal[n_cells];

  // just checking for memory issues for now
  nsfd_grid_geom_coords (grid_geom_p, x_coords, y_coords);

  delete[] x_coords;
  delete[] y_coords;
  nsfd_grid_geom_free (&grid_geom_p);
}

TEST (NSFDGridValues, new)
{
  NSFDGridGeomData geom_data     = { 10, 15, 100, 200 };
  NSFDGridValues  *grid_values_p = nsfd_grid_values_new (&geom_data);
  nsfd_grid_values_free (&grid_values_p);
}

TEST (NSFDGridValues, init)
{
  size_t           imax          = 10;
  size_t           jmax          = 15;
  NSFDGridGeomData geom_data     = { imax, jmax, 100, 100 };
  NSFDGridValues  *grid_values_p = nsfd_grid_values_new (&geom_data);
  nsfd_grid_values_init (grid_values_p, 10);
  nsfd_grid_values_free (&grid_values_p);
}

TEST (NSFDGridValues, n_cells)
{
  size_t           imax          = 10;
  size_t           jmax          = 15;
  NSFDGridGeomData geom_data     = { imax, jmax, 100, 100 };
  NSFDGridValues  *grid_values_p = nsfd_grid_values_new (&geom_data);
  size_t           n_cells       = nsfd_grid_values_n_cells (grid_values_p);
  ASSERT_EQ (n_cells, (imax + 2) * (jmax + 2));
  nsfd_grid_values_free (&grid_values_p);
}
}

int
main (int argc, char **argv)
{
  ::testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
