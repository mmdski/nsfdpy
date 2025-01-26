#include "ns_field_vec.h"

#include <assert.h>
#include <stdlib.h>

#include <nsfd.h>

#include "ns_vector.h"

NSFieldVec *ns_field_vec_new(size_t imax, size_t jmax) {
  size_t field_size = (imax + 1) * (jmax + 1) * sizeof(NSVector);
  NSFieldVec *vf = malloc(sizeof(NSFieldVec) + field_size);
  if (!vf) return NULL;
  vf->imax = imax;
  vf->jmax = jmax;
  vf->size = (imax + 1) * (jmax + 1);
  return vf;
}

NSFieldVec *ns_field_vec_init(size_t imax, size_t jmax, NSVector init_val) {
  if (imax == 0 || jmax == 0) return NULL;
  NSFieldVec *vf = ns_field_vec_new(imax, jmax);
  if (!vf) return NULL;
  for (size_t i = 0; i < vf->size; ++i) {
    vf->field[i] = init_val;
  }
  return vf;
}

void ns_field_vec_free(NSFieldVec *vec_field) { free(vec_field); }

NSVector *ns_field_vec_get(NSFieldVec *vf, size_t i, size_t j) {
  if (!vf) return NULL;
  assert(i * j < vf->size);
  return &vf->field[i * (vf->jmax + 2) + j];
}
