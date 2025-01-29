#ifndef NS_S_FIELD_H_
#define NS_S_FIELD_H_

#include <stddef.h>

#include <nsfd.h>

struct NSSField {
  size_t size;
  NSDomainShape shape;
  double field[];
};

#endif
