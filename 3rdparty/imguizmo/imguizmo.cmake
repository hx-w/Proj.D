CPMAddPackage("gh:CedricGuillemet/ImGuizmo#1.83") # [TODO] need to be fixed, since imgui's break upgrade

file(GLOB ImGuizmo_SRCS ${ImGuizmo_SOURCE_DIR}/*.cpp)
add_library(ImGuizmo STATIC ${ImGuizmo_SRCS})

target_link_libraries(ImGuizmo PUBLIC imgui)
target_compile_definitions(ImGuizmo PUBLIC -DIMGUI_DEFINE_MATH_OPERATORS=)

# for ${PROJECT_NAME}
include_directories(${ImGuizmo_SOURCE_DIR})