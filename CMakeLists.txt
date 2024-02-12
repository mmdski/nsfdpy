cmake_minimum_required(VERSION 3.23)
project(nsfd VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build tests" NO)

option(PY_BUILD "Build shared library for use with Python" NO)

if(PROJECT_IS_TOP_LEVEL)
    include(Sanitizers.cmake)
endif()

include(FetchContent)

if(BUILD_TESTS)
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

if(MSVC)
    # /wd6326 "Potential comparison of a constant with another constant."
    # /wd4200 "nonstandard extension used: zero-sized array in struct/union"
    add_compile_options(/guard:cf /analyze /permissive- /W4 /WX /wd6326 /wd4200)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(NSFD_SOURCES src/nsfd.c src/nsfd_grid.c)

if(PY_BUILD)
    find_package(Python COMPONENTS Development REQUIRED)
    add_compile_definitions(PY_BUILD)
    add_library(nsfd SHARED ${NSFD_SOURCES})
    set_target_properties(nsfd PROPERTIES OUTPUT_NAME "nsfd.pymem")
    target_link_libraries(nsfd PRIVATE Python::Python)
    install(TARGETS nsfd DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/nsfd)

    if(MSVC)
        set_target_properties(nsfd PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS YES)
    endif()
else()
    add_library(nsfd ${NSFD_SOURCES})
endif()

target_sources(nsfd
    PUBLIC
    FILE_SET api
    TYPE HEADERS
    BASE_DIRS include
    FILES include/nsfd.h
)

if(BUILD_TESTS)
    add_executable(nsfd_grid.test src/nsfd_grid.test.cpp)
    target_link_libraries(nsfd_grid.test nsfd GTest::gtest GTest::gtest_main)
    gtest_add_tests(TARGET nsfd_grid.test TEST_LIST nsfd_grid.test-list)
    set_tests_properties(${nsfd_grid.test-list} PROPERTIES ENVIRONMENT "${TEST_ENV}")
endif()
