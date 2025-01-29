cmake_minimum_required(VERSION 3.23)
project(nsfd LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(nsfd_BUILD_TESTS "Build tests" NO)
option(nsfd_BUILD_EXAMPLES "Build examples" NO)

if(PROJECT_IS_TOP_LEVEL)
  include(cmake/Sanitizers.cmake)

  if(MSVC)
    add_compile_options(/guard:cf /analyze /permissive- /W4 /WX)

  else()
    add_compile_options(-pedantic -Wall -Wextra -Wconversion -Werror -Wvla)
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

set(NSFD_SRCS
  src/nsfd.c
  src/ns_s_field.c
)
add_library(nsfd ${NSFD_SRCS})
add_library(nsfd::nsfd ALIAS nsfd)

target_include_directories(nsfd PUBLIC include)

target_sources(nsfd
  PUBLIC
  FILE_SET HEADERS
  FILES
  include/nsfd.h
)

if(nsfd_BUILD_TESTS)
  macro(add_nsfd_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_link_libraries(${test_name} nsfd::nsfd GTest::gtest_main)
    gtest_add_tests(TARGET ${test_name} TEST_LIST ${test_name}.test-list)
    set_tests_properties(${${test_name}.test-list} PROPERTIES ENVIRONMENT "${TEST_ENV}")
  endmacro()

  add_nsfd_test(nsfd_s_field.test src/ns_s_field.test.cpp)
endif()

if(nsfd_BUILD_EXAMPLES)
endif()
