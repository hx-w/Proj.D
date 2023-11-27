# setup all dependencies
# - [x] remote lib with cmakelists.txt, use CPMAddPackage("gh:<user>/<repo>#<tag>").
# - [x] remote lib without cmakelists.txt, write a <repo>.cmake in `3rdparty/<repo>/`,
#    which to build static lib in, and `include(${PROJECT_3RDPARTY_DIR}/<repo>/<repo>.cmake)` to build.
# - [x] local lib, also build in `3rdparty/<repo>`, with code locally.

set(PROJECT_3RDPARTY_DIR ${PROJECT_SOURCE_DIR}/3rdparty)

include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/FindOpenMP.cmake)

# Add deps
## 01 [glm] 3D mathlib - remote
CPMAddPackage("gh:g-truc/glm#0.9.9.8")

## 02 [raylib] Renderlib - remote
## most deps like `glad`, `glfw` and `stb` are included in raylib,
## so don't need to add these package twice
## external deps of raylib at: https://github.com/raysan5/raylib/wiki/raylib-dependencies
CPMAddPackage(
    raylib
    GITHUB_REPOSITORY "raysan5/raylib"
    GIT_TAG "4.5.0"
    OPTIONS
    "CUSTOMIZE_BUILD ON"
    "USE_AUDIO OFF"
    "PLATFORM Desktop"
)

## 03 [glad] OpenGL interface - remote
## althrough `raylib` has already included `glad`, but I can't include <glad/glad.h> directly in proj.
## and I need OpenGL API to fix the mesh indexing problem in raylib, so it is.
CPMAddPackage("gh:Dav1dde/glad@0.1.35")

## 04 [imgui] GUI - 3rdparty
include(${PROJECT_3RDPARTY_DIR}/imgui/imgui.cmake)

## 05 [rlImGui] GUI-interface between `raylib` and `imgui` - 3rdparty
## must be built after `raylib` and `imgui` since they are the static libs of `rlImGui`
include(${PROJECT_3RDPARTY_DIR}/rlImGui/rlImGui.cmake)

## 06 [imguizmo] toolkit for 3D transform - 3rdparty
# include(${PROJECT_3RDPARTY_DIR}/imguizmo/imguizmo.cmake)

## 07 [tinynurbs] nurbs c++ - remote
CPMAddPackage("gh:hx-w/tinynurbs@0.1.1")


set_target_properties(imgui rlImGui PROPERTIES FOLDER ExternalTargets)

# All deps list in DEPS
list(APPEND DEPS glm raylib glad imgui rlImGui tinynurbs)
