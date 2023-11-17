CPMAddPackage("gh:hx-w/rlImGui#1.0.1")

file(GLOB_RECURSE rlImGui_SRCS ${rlImGui_SOURCE_DIR}/*.cpp)
add_library(rlImGui STATIC ${rlImGui_SRCS})

target_link_libraries(rlImGui PUBLIC raylib imgui)

# for ${PROJECT_NAME}
include_directories(${rlImGui_SOURCE_DIR})