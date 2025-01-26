cmake_minimum_required(VERSION 3.23)
project(nsfdpy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-pedantic -Wall -Wextra -Wconversion -Werror)

add_subdirectory(external/nsfd)
