cmake_minimum_required(VERSION 3.23)
project(nsfdpy LANGUAGES CXX)

add_subdirectory(external/pybind11)

add_subdirectory(external/nsfd)

set(PYBIND11_NEWPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_nsfd src/nsfdpy/_nsfd.cpp)
target_link_libraries(_nsfd PRIVATE nsfd::nsfd)

install(TARGETS _nsfd LIBRARY DESTINATION ${PROJECT_NAME})
