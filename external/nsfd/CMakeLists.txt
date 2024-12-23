cmake_minimum_required(VERSION 3.23)
project(nsfd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(nsfd_BUILD_TESTS "Build tests" NO)
option(nsfd_BUILD_EXAMPLES "Build examples" NO)

if(PROJECT_IS_TOP_LEVEL)
  include(cmake/Sanitizers.cmake)

  if(MSVC)
    add_compile_options(/guard:cf /analyze /permissive- /W4 /WX)

  else()
    add_compile_options(-pedantic -Wall -Wextra -Wconversion -Werror)
  endif()
endif()

if(nsfd_BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
  )

  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  enable_testing()
  include(GoogleTest)
endif()

add_library(nsfd INTERFACE)
add_library(nsfd::nsfd ALIAS nsfd)
target_sources(nsfd
  INTERFACE
  FILE_SET HEADERS
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src
  FILES
  src/nsfd/scalar.hpp
  src/nsfd/vector.hpp
  src/nsfd/field/field.hpp
  src/nsfd/field/scalar.hpp
  src/nsfd/field/vector.hpp
  src/nsfd/grid/axis.hpp
  src/nsfd/grid/geom_data.hpp
  src/nsfd/grid/grid.hpp
  src/nsfd/grid/staggered_grid.hpp
  src/nsfd/ops/gradient.hpp
)

if(nsfd_BUILD_TESTS)
  add_executable(scalar.test src/nsfd/scalar.test.cpp)
  target_link_libraries(scalar.test nsfd::nsfd GTest::gtest_main)
  gtest_add_tests(TARGET scalar.test TEST_LIST scalar.test-list)
  set_tests_properties(${scalar.test-list} PROPERTIES ENVIRONMENT "${TEST_ENV}")

  add_executable(vector.test src/nsfd/vector.test.cpp)
  target_link_libraries(vector.test nsfd::nsfd GTest::gtest_main)
  gtest_add_tests(TARGET vector.test TEST_LIST vector.test-list)
  set_tests_properties(${vector.test-list} PROPERTIES ENVIRONMENT "${TEST_ENV}")

  add_executable(field.scalar.test src/nsfd/field/scalar.test.cpp)
  target_link_libraries(field.scalar.test nsfd::nsfd GTest::gtest_main)
  gtest_add_tests(TARGET field.scalar.test TEST_LIST field.scalar.test-list)
  set_tests_properties(${field.scalar.test-list} PROPERTIES ENVIRONMENT "${TEST_ENV}")

  add_executable(field.vector.test src/nsfd/field/vector.test.cpp)
  target_link_libraries(field.vector.test nsfd::nsfd GTest::gtest_main)
  gtest_add_tests(TARGET field.vector.test TEST_LIST field.vector.test-list)
  set_tests_properties(${field.vector.test-list} PROPERTIES ENVIRONMENT "${TEST_ENV}")

  add_executable(ops.gradient.test src/nsfd/ops/gradient.test.cpp)
  target_link_libraries(ops.gradient.test nsfd::nsfd GTest::gtest_main)
  gtest_add_tests(TARGET ops.gradient.test TEST_LIST ops.gradient.test-list)
  set_tests_properties(${ops.gradient.test-list} PROPERTIES ENVIRONMENT "${TEST_ENV}")
endif()

if(nsfd_BUILD_EXAMPLES)
endif()
