#ifndef NSFD_INTERNAL_H_
#define NSFD_INTERNAL_H_

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <nsfd.h>

// number of boundary cells in x and y directions
#define N_B_CELLS 2

// calculate grid index
#define NSFD_GRID_INDEX(imax, i, j) (j * (imax + N_B_CELLS) + i)

#define nsfd_real_abs(x) fabs (x)
#define nsfd_real_min(x, y) fmin (x, y)

typedef struct
{
  NSFDReal x;
  NSFDReal y;
} NSFDGridCoord;

struct NSFDGridGeom
{
  NSFDGridGeomData geom_data;
  NSFDGridCoord    coords[];
};

struct NSFDGridValues
{
  NSFDGridGeomData geom_data;
  NSFDReal         values[];
};

static inline size_t
nsfd_size_t_add (size_t x, size_t y, const char *file, int line)
{
  if (SIZE_MAX - x < y)
    {
      fprintf (stderr,
               "%s, %i: The operation %zu + %zu will wrap\n",
               file,
               line,
               x,
               y);
      exit (EXIT_FAILURE);
    }

  return x + y;
}

static inline size_t
nsfd_size_t_mult (size_t x, size_t y, const char *file, int line)
{
  if (x > SIZE_MAX / y)
    {
      fprintf (stderr,
               "%s, %i: The operation %zu * %zu will wrap\n",
               file,
               line,
               x,
               y);
      exit (EXIT_FAILURE);
    }

  return x * y;
}

static inline NSFDReal
nsfd_size_t_to_real (size_t s, const char *file, int line)
{
  // s is unsigned so cannot be less than DBL_MIN
  if (s > DBL_MAX)
    {
      fprintf (stderr, "%s, %i: Cast to out of range value\n", file, line);
      exit (EXIT_FAILURE);
    }

  return (NSFDReal) s;
}

static inline bool
nsfd_grid_geom_data_eq (NSFDGridGeomData *a_p, NSFDGridGeomData *b_p)
{
  if (!a_p || !b_p)
    return false;
  if (a_p == b_p)
    return true;
  if (a_p->imax != b_p->imax || a_p->jmax != b_p->jmax)
    return false;
  if (a_p->xlength != b_p->xlength || a_p->ylength != b_p->ylength)
    return false;
  return true;
}

#endif
