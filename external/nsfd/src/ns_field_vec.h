#ifndef NS_FIELD_VEC_
#define NS_FIELD_VEC_

#include <stdlib.h>

#include <nsfd.h>

struct NSFieldVec {
  size_t imax;
  size_t jmax;
  size_t size;
  NSVector field[];
};

#endif
