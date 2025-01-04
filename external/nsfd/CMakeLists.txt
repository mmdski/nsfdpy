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
  src/nsfd/iterpressure.hpp
  src/nsfd/scalar.hpp
  src/nsfd/vector.hpp
  src/nsfd/bcond/apply.hpp
  src/nsfd/bcond/bcond.hpp
  src/nsfd/bcond/data.hpp
  src/nsfd/comp/fg.hpp
  src/nsfd/comp/rhs.hpp
  src/nsfd/field/field.hpp
  src/nsfd/grid/axis.hpp
  src/nsfd/grid/geom_data.hpp
  src/nsfd/grid/grid.hpp
  src/nsfd/grid/staggered_grid.hpp
  src/nsfd/ops/advection.hpp
  src/nsfd/ops/divergence.hpp
  src/nsfd/ops/gradient.hpp
  src/nsfd/ops/laplace.hpp
)

if(nsfd_BUILD_TESTS)
  macro(add_nsfd_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_link_libraries(${test_name} nsfd::nsfd GTest::gtest_main)
    gtest_add_tests(TARGET ${test_name} TEST_LIST ${test_name}.test-list)
    set_tests_properties(${${test_name}.test-list} PROPERTIES ENVIRONMENT "${TEST_ENV}")
  endmacro()

  add_nsfd_test(bcond.apply.test src/nsfd/bcond/apply.test.cpp)
  add_nsfd_test(bcond.bcond.test src/nsfd/bcond/bcond.test.cpp)
  add_nsfd_test(comp.fg.test src/nsfd/comp/fg.test.cpp)
  add_nsfd_test(comp.time_step.test src/nsfd/comp/time_step.test.cpp)
  add_nsfd_test(field.scalar.test src/nsfd/field/scalar.test.cpp)
  add_nsfd_test(field.vector.test src/nsfd/field/vector.test.cpp)
  add_nsfd_test(geom.geometry.test src/nsfd/geometry.test.cpp)
  add_nsfd_test(grid.staggered_grid.test src/nsfd/grid/staggered_grid.test.cpp)
  add_nsfd_test(iterpressure.test src/nsfd/iterpressure.test.cpp)
  add_nsfd_test(ops.gradient.test src/nsfd/ops/gradient.test.cpp)
  add_nsfd_test(ops.laplace.test src/nsfd/ops/laplace.test.cpp)
  add_nsfd_test(scalar.test src/nsfd/scalar.test.cpp)
  add_nsfd_test(vector.test src/nsfd/vector.test.cpp)
endif()

if(nsfd_BUILD_EXAMPLES)
endif()
