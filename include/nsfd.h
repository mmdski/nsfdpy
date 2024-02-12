#ifndef NSFD_H_
#define NSFD_H_

#include <stddef.h>

typedef double NSFDReal;

typedef struct
{
  size_t m_rows;
  size_t n_cols;
} NSFDGridShape;

typedef struct NSFDScalarField NSFDScalarField;

size_t           nsfd_sizeof_real (void);
NSFDScalarField *nsfd_s_field_new (size_t m_rows, size_t n_cols);
void             nsfd_s_field_free (NSFDScalarField **s_field_pp);
NSFDGridShape    nsfd_s_field_shape (NSFDScalarField *s_field_p);
size_t           nsfd_s_field_size (NSFDScalarField *s_field_p);
void nsfd_s_field_init_const (NSFDScalarField *s_field_p, NSFDReal c);
void nsfd_s_field_init_grid (NSFDScalarField *s_field_p,
                             NSFDReal         x_min,
                             NSFDReal         x_max,
                             NSFDReal         y_min,
                             NSFDReal         y_max);
void nsfd_s_field_get_values (NSFDScalarField *s_field_p, NSFDReal *field);
void nsfd_s_field_get_grid (NSFDScalarField *s_field_p,
                            NSFDReal        *grid_x,
                            NSFDReal        *grid_y);

#endif
