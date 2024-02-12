#ifndef NSFD_GRID_INTERNAL_H_
#define NSFD_GRID_INTERNAL_H_

#include <stddef.h>

#include <nsfd.h>

// calculate field index
#define NSFD_F_INDEX(n_cols, row, col) (row * n_cols + col)

typedef struct
{
  NSFDReal x;
  NSFDReal y;
} NSFDGridCoord;

typedef struct
{
  NSFDGridCoord coord;
  NSFDReal      s;
} NSFDGridValue;

struct NSFDGrid
{
  NSFDGridShape shape;
  NSFDGridValue values[];
};

#endif
