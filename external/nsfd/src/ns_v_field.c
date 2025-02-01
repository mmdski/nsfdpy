#include "ns_v_field.h"

#include <nsfd.h>

#include "nsfd-internal.h"

size_t ns_v_field_mem_size(NSDomainShape shape) {
  return chk_size_t_add(
      sizeof(NSVField),
      chk_size_t_mul(sizeof(NSVector), calc_grid_size(shape)));
}

NSVField *ns_v_field_init(NSDomainShape shape, NSVField *vfp,
                          NSVector init_val) {
  if (shape.imax == 0 || shape.jmax == 0) return vfp;
  if (!vfp) return vfp;
  vfp->shape = shape;
  vfp->size = calc_grid_size(vfp->shape);
  for (size_t i = 0; i < vfp->size; ++i) {
    vfp->field[i] = init_val;
  }

  return vfp;
}

NSDomainShape ns_v_field_shape(NSVField *vfp) {
  if (!vfp) return (NSDomainShape){0};
  return vfp->shape;
}

NSGridShape ns_v_field_grid_shape(NSVField *vfp) {
  if (!vfp) return (NSGridShape){0};
  return (NSGridShape){vfp->shape.imax + 2, vfp->shape.jmax + 2};
}

NSVector *ns_v_field_get(NSVField *vfp, size_t i, size_t j) {
  if (!vfp) return NULL;
  chk_indices(vfp->shape, i, j);
  return &vfp->field[i * (vfp->shape.jmax + 2) + j];
}

void ns_v_field_set(NSVField *vfp, size_t i, size_t j, real x, real y) {
  if (!vfp) return;
  chk_indices(vfp->shape, i, j);
  vfp->field[i * (vfp->shape.jmax + 2) + j].x = x;
  vfp->field[i * (vfp->shape.jmax + 2) + j].y = y;
}

void ns_v_field_values(NSVField *vfp, real *x_values, real *y_values) {
  if (!(x_values && y_values)) return;
  for (size_t i = 0; i < vfp->size; ++i) {
    x_values[i] = vfp->field[i].x;
    y_values[i] = vfp->field[i].y;
  }
}
