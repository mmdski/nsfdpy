#include "ns_s_field.h"

#include <assert.h>
#include <stdio.h>

#include <nsfd.h>

#include "nsfd-internal.h"

size_t ns_s_field_mem_size(NSDomainShape shape) {
  return chk_size_t_add(sizeof(NSSField),
                        chk_size_t_mul(sizeof(real), calc_grid_size(shape)));
}

NSSField *ns_s_field_init(NSDomainShape shape, NSSField *sfp, real init_val) {
  if (shape.imax == 0 || shape.jmax == 0) return sfp;
  if (!sfp) return sfp;
  sfp->shape = shape;
  sfp->size = calc_grid_size(sfp->shape);
  for (size_t i = 0; i < sfp->size; ++i) {
    sfp->field[i] = init_val;
  }
  return sfp;
}

NSDomainShape ns_s_field_shape(NSSField *sfp) {
  if (!sfp) return (NSDomainShape){0};
  return sfp->shape;
}

NSGridShape ns_s_field_grid_shape(NSSField *sfp) {
  if (!sfp) return (NSGridShape){0};
  return (NSGridShape){sfp->shape.imax + 2, sfp->shape.jmax + 2};
}

real *ns_s_field_get(NSSField *sfp, size_t i, size_t j) {
  if (!sfp) return NULL;
  chk_indices(sfp->shape, i, j);
  return &sfp->field[i * (sfp->shape.jmax + 2) + j];
}

void ns_s_field_set(NSSField *sfp, size_t i, size_t j, real value) {
  if (!sfp) return;
  chk_indices(sfp->shape, i, j);
  sfp->field[i * (sfp->shape.jmax + 2) + j] = value;
}

void ns_s_field_values(NSSField *sfp, real *values) {
  if (!values) return;
  for (size_t i = 0; i < sfp->size; ++i) {
    values[i] = sfp->field[i];
  }
}
