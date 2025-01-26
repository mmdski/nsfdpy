#ifndef NSFD_H_
#define NSFD_H_

typedef struct {
  double x;
  double y;
} NSVector;

// vector field
typedef struct NSFieldVec NSFieldVec;

NSFieldVec *ns_field_vec_init(size_t imax, size_t jmax, NSVector init_val);
void ns_field_vec_free(NSFieldVec *vf);
NSVector *ns_field_vec_get(NSFieldVec *vf, size_t i, size_t j);

#endif
