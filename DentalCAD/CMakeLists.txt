# build DentalCAD executable file

list(APPEND LIBS INFRASTRUCTURE)

list(APPEND OS_ISSUES)
if (APPLE)
    list(APPEND OS_ISSUES "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})

file(GLOB_RECURSE ${PROJECT_NAME}_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${${PROJECT_NAME}_SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPS} ${LIBS} ${OS_ISSUES})

set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/resources)
add_compile_definitions(ASSETS_PATH="${ASSETS_DIR}")