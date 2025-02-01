#ifndef NSFD_H_
#define NSFD_H_

#include <stddef.h>

typedef double real;

size_t ns_real_size(void);

typedef struct {
  real x;
  real y;
} NSVector;

// shape of internal domain
typedef struct {
  size_t imax;
  size_t jmax;
} NSDomainShape;

// shape of grid, including boundary cells
typedef struct {
  size_t nx;
  size_t ny;
} NSGridShape;

// scalar field
typedef struct NSSField NSSField;

size_t ns_s_field_mem_size(NSDomainShape shape);
NSSField *ns_s_field_init(NSDomainShape shape, NSSField *sfp, real init_val);
NSDomainShape ns_s_field_shape(NSSField *sfp);
NSGridShape ns_s_field_grid_shape(NSSField *sfp);
real *ns_s_field_get(NSSField *sfp, size_t i, size_t j);
void ns_s_field_set(NSSField *sfp, size_t i, size_t j, real value);
void ns_s_field_values(NSSField *sfp, real *values);

// vector field
typedef struct NSVField NSVField;

size_t ns_v_field_mem_size(NSDomainShape shape);
NSVField *ns_v_field_init(NSDomainShape shape, NSVField *vfp,
                          NSVector init_val);
NSDomainShape ns_v_field_shape(NSVField *vfp);
NSGridShape ns_v_field_grid_shape(NSVField *vfp);
NSVector *ns_v_field_get(NSVField *vfp, size_t i, size_t j);
void ns_v_field_set(NSVField *vfp, size_t i, size_t j, real x, real y);
void ns_v_field_values(NSVField *vfp, real *x_values, real *y_values);

#endif
