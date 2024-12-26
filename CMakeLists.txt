cmake_minimum_required(VERSION 3.23)
project(nsfdpy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-pedantic -Wall -Wextra -Wconversion -Werror)

add_subdirectory(external/pybind11)

add_subdirectory(external/nsfd)

set(PYBIND11_NEWPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_nsfd
    src/nsfdpy/_nsfd.cpp
    src/nsfdpy/bind_scalar.cpp
    src/nsfdpy/bind_vector.cpp
    src/nsfdpy/bind_iterpressure.cpp
    src/nsfdpy/bcond/bind_apply_bcond.cpp
    src/nsfdpy/bcond/bind_bc_data.cpp
    src/nsfdpy/bcond/bind_bc_type.cpp
    src/nsfdpy/comp/bind_fg.cpp
    src/nsfdpy/comp/bind_rhs.cpp
    src/nsfdpy/comp/bind_u_next.cpp
    src/nsfdpy/field/bind_scalar.cpp
    src/nsfdpy/field/bind_vector.cpp
    src/nsfdpy/grid/bind_axis.cpp
    src/nsfdpy/grid/bind_grid.cpp
    src/nsfdpy/grid/bind_staggered.cpp
    src/nsfdpy/ops/bind_advection.cpp
    src/nsfdpy/ops/bind_gradient.cpp
    src/nsfdpy/ops/bind_laplace.cpp
)
target_link_libraries(_nsfd PRIVATE nsfd::nsfd)

install(TARGETS _nsfd LIBRARY DESTINATION ${PROJECT_NAME})
