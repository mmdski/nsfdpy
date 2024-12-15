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

add_library(nsfd src/nsfd/vector.cpp)
add_library(nsfd::nsfd ALIAS nsfd)
target_sources(nsfd
    PUBLIC
    FILE_SET api
    TYPE HEADERS
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
)

if(nsfd_BUILD_TESTS)
endif()

if(nsfd_BUILD_EXAMPLES)
endif()
