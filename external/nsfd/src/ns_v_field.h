#ifndef NS_V_FIELD_H_
#define NS_V_FIELD_H_

#include <stddef.h>

#include <nsfd.h>

struct NSVField {
  size_t size;
  NSDomainShape shape;
  NSVector field[];
};

#endif
