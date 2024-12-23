/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <gtest/gtest.h>

#include <nsfd/field.hpp>
#include <nsfd/vector.hpp>

namespace {
TEST(FieldVectorTest, init) {
  nsfd::Field<nsfd::Vector> sf = nsfd::Field<nsfd::Vector>();
}
}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
