#include <cstdlib>
#include <gtest/gtest.h>

extern "C" {
#include <nsfd.h>
}

namespace {
TEST(ns_s_field, init) {
  NSDomainShape shape = {10, 5};
  size_t field_size = ns_s_field_mem_size(shape);
  NSSField* sfp = (NSSField*)std::malloc(field_size);
  if (!sfp) {
    std::free(sfp);  // keep the linter from complaining
    ASSERT_TRUE(sfp);
  }
  double init_val = 0;
  sfp = ns_s_field_init(shape, sfp, init_val);
  if (!sfp) {
    std::free(sfp);
    ASSERT_TRUE(sfp);
  }

  NSDomainShape field_shape = ns_s_field_shape(sfp);
  ASSERT_EQ(shape.imax, field_shape.imax);
  ASSERT_EQ(shape.jmax, field_shape.jmax);

  for (size_t i = 1; i <= shape.imax; ++i) {
    for (size_t j = 1; j <= shape.jmax; ++j) {
      ASSERT_EQ(init_val, *ns_s_field_get(sfp, i, j));
    }
  }

  std::free(sfp);
}
}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
