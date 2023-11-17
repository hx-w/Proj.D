cmake_minimum_required(VERSION 3.14)
project(DentalCAD VERSION 0.1.0)

message("-- system arch:  ${CMAKE_SYSTEM_PROCESSOR}")

# c++17
set(CMAKE_CXX_STANDARD 17)


# install deps and append to list ${DEPS}
include(${PROJECT_SOURCE_DIR}/cmake/Dependencies.cmake)

add_subdirectory(DentalCAD)
add_subdirectory(libs)
# add_subdirectory(backend)
# add_subdirectory(tests)
