file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE HPPS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
add_library(INFRASTRUCTURE ${SRCS})
target_include_directories(INFRASTRUCTURE INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# set INFRASTRUCTURE_INCLUDE_DIRS 
set(INFRASTRUCTURE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "INFRASTRUCTURE include directories" FORCE)
