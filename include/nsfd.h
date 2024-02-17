#ifndef NSFD_H_
#define NSFD_H_

#include <stddef.h>

typedef double NSFDReal;

size_t nsfd_sizeof_real (void);

typedef struct
{
  size_t   imax;
  size_t   jmax;
  NSFDReal xlength;
  NSFDReal ylength;
} NSFDGridGeomData;

typedef struct NSFDGridGeom NSFDGridGeom;

NSFDGridGeom *nsfd_grid_geom_new (NSFDGridGeomData *geom_data_p);
void          nsfd_grid_geom_free (NSFDGridGeom **grid_geom_pp);
size_t        nsfd_grid_geom_n_cells (NSFDGridGeom *grid_geom_p);
void          nsfd_grid_geom_init_p (NSFDGridGeom *grid_geom_p);
void          nsfd_grid_geom_init_u (NSFDGridGeom *grid_geom_p);
void          nsfd_grid_geom_init_v (NSFDGridGeom *grid_geom_p);
void
nsfd_grid_geom_coords (NSFDGridGeom *grid_geom_p, NSFDReal *x, NSFDReal *y);

typedef struct NSFDGridValues NSFDGridValues;

NSFDGridValues *nsfd_grid_values_new (NSFDGridGeomData *geom_data_p);
void            nsfd_grid_values_free (NSFDGridValues **grid_values_p);
size_t          nsfd_grid_values_n_cells (NSFDGridValues *grid_values_p);
void            nsfd_grid_values_init (NSFDGridValues *grid_values_p,
                                       NSFDReal        init_values);
void nsfd_grid_values (NSFDGridValues *grid_values_p, NSFDReal *values);
#endif
