CPMAddPackage("gh:ocornut/imgui@1.90") # 2023.11.16 release

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp    # for this task
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp # use glfw & opengl3
)
target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS) # optional imgui setting
target_link_libraries(imgui PUBLIC glfw)

# for ${PROJECT_NAME}
include_directories(${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)