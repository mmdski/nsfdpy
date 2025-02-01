#include <cstdlib>
#include <gtest/gtest.h>

extern "C" {
#include <nsfd.h>
}

namespace {
TEST(ns_v_field, init) {
  NSDomainShape shape = {10, 5};
  size_t field_size = ns_v_field_mem_size(shape);
  NSVField* vfp = (NSVField*)std::malloc(field_size);
  if (!vfp) {
    std::free(vfp);  // keep the linter from complaining
    ASSERT_TRUE(vfp);
  }
  NSVector init_val = {.x = 0, .y = 1};
  vfp = ns_v_field_init(shape, vfp, init_val);
  if (!vfp) {
    std::free(vfp);
    ASSERT_TRUE(vfp);
  }

  NSDomainShape field_shape = ns_v_field_shape(vfp);
  ASSERT_EQ(shape.imax, field_shape.imax);
  ASSERT_EQ(shape.jmax, field_shape.jmax);

  for (size_t i = 1; i <= shape.imax; ++i) {
    for (size_t j = 1; j <= shape.jmax; ++j) {
      ASSERT_EQ(init_val.x, ns_v_field_get(vfp, i, j)->x);
      ASSERT_EQ(init_val.y, ns_v_field_get(vfp, i, j)->y);
    }
  }

  std::free(vfp);
}
}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
