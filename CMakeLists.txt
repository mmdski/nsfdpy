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
