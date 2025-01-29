cmake_minimum_required(VERSION 3.23)
project(nsfdpy LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(BUILD_SHARED_LIBS ON)

add_subdirectory(external/nsfd)

if(MSVC)
    set_target_properties(nsfd PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS YES)
endif()

install(TARGETS nsfd DESTINATION nsfdpy)

# find_package(Python
# COMPONENTS Interpreter Development.Module
# REQUIRED)

# add_custom_command(
# OUTPUT example.c
# COMMENT
# "Making ${CMAKE_CURRENT_BINARY_DIR}/example.c from ${CMAKE_CURRENT_SOURCE_DIR}/example.pyx"
# COMMAND Python::Interpreter -m cython
# "${CMAKE_CURRENT_SOURCE_DIR}/example.pyx" --output-file example.c
# DEPENDS example.pyx
# VERBATIM)

# python_add_library(example MODULE example.c WITH_SOABI)

# install(TARGETS example DESTINATION .)
