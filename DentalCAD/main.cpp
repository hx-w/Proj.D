/**
 * @copyright SCUBot 2023-2024
 */

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

#include <glm/glm.hpp>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
// #include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <rlImGui.h>

#include <communication/ContextHub.h>

#define RLIGHTS_IMPLEMENTATION

#include "rlights.h"
#include "gui/style.h"
#include "meshlib/mesh_loader.h"
#include "meshlib/mesh_data.h"


int main() {
    InitWindow(1200, 1000, "Dynamic Mesh test");
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(144);
    rlDisableBackfaceCulling();
    rlEnableSmoothLines();

    rlImGuiSetup(true);
    DentalGUI::set_custom_imgui_style();

    Camera camera = {{5.0f, 5.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, 0};

    // Load basic lighting shader
    Shader shader = LoadShader(ASSETS_PATH "/shaders/raylib/lighting.vs",
                               ASSETS_PATH "/shaders/raylib/lighting.fs");
    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambientVal[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    SetShaderValue(shader, ambientLoc, ambientVal, SHADER_UNIFORM_VEC4);

    Light light = CreateLight(LIGHT_POINT, Vector3{-2, 10, -2}, Vector3Zero(), YELLOW, shader);

    DentalLib::MeshLoadStatus status;
    std::vector<std::shared_ptr<DentalLib::DlMesh>> dlmeshes;
    while (!WindowShouldClose()) {
        if (IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();
            for (int i = 0; i < (int)droppedFiles.count; ++i) {
                auto ptr_dlmesh = DentalLib::MeshLoader::load(droppedFiles.paths[i], status);
                assert(status == DentalLib::MeshLoadStatus::Success);

                ptr_dlmesh->set_shader(shader);
                dlmeshes.emplace_back(ptr_dlmesh);
            }
            UnloadDroppedFiles(droppedFiles);
        }

        UpdateCamera(&camera, CAMERA_ORBITAL);
        float cameraPos[3] = {camera.position.x, camera.position.y, camera.position.z};
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(shader, light);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        rlImGuiBegin();

        { // imgui
            ImGui::Begin("MeshViewer");
            ImGui::Button("All meshes created:");
            ImGui::End();
        }

        BeginMode3D(camera);
        for (auto& dlmesh : dlmeshes) {
            dlmesh->draw_call();
        }
        DrawGrid(50, 1.0);

        EndMode3D();

        DrawFPS(10, 10);

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
