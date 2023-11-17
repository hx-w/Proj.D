# build DentalCAD executable file

list(APPEND LIBS INFRASTRUCTURE)

include_directories(${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})

file(GLOB_RECURSE ${PROJECT_NAME}_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${${PROJECT_NAME}_SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPS} ${LIBS})

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()