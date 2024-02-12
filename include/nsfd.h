#ifndef NSFD_H_
#define NSFD_H_

#include <stddef.h>

typedef double NSFDReal;

typedef struct
{
  size_t m_rows;
  size_t n_cols;
} NSFDGridShape;

typedef struct NSFDGrid NSFDGrid;

size_t        nsfd_sizeof_real (void);
NSFDGrid     *nsfd_grid_new (size_t m_rows, size_t n_cols);
void          nsfd_grid_free (NSFDGrid **grid_pp);
NSFDGridShape nsfd_grid_shape (NSFDGrid *grid_p);
size_t        nsfd_grid_size (NSFDGrid *grid_p);
void          nsfd_grid_init_const (NSFDGrid *grid_p, NSFDReal c);
void          nsfd_grid_init_grid (NSFDGrid *grid_p,
                                   NSFDReal  x_min,
                                   NSFDReal  x_max,
                                   NSFDReal  y_min,
                                   NSFDReal  y_max);
void          nsfd_grid_get_values (NSFDGrid *grid_p, NSFDReal *field);
void
nsfd_grid_get_coords (NSFDGrid *grid_p, NSFDReal *grid_x, NSFDReal *grid_y);

#endif
