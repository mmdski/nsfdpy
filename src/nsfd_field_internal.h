#ifndef NSFD_FIELD_INTERNAL_H_
#define NSFD_FIELD_INTERNAL_H_

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
} NSFDFieldSValue;

struct NSFDScalarField
{
  NSFDGridShape   shape;
  NSFDFieldSValue values[];
};

#endif
