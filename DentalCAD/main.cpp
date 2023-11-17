/**
 * @copyright SCUBot 2023-2024
 */

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
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <rlImGui.h>

#include <communication/ContextHub.h>

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "gui/style.h"


// Utility function to prepare a mesh made of n triangles
// only fills vertices and indices. rest of the member are not used.
void makeTriangleMesh(Mesh &mesh, int n) {
	mesh.triangleCount = n;
	mesh.vertexCount = n * 3;
	mesh.vertices = new float[mesh.vertexCount * 3];
	mesh.normals = new float[mesh.vertexCount * 3];
	mesh.indices = new unsigned short[mesh.vertexCount];

	for (int i = 0; i < n; ++i) {
		float triangle[9] = {0.f, 0.f, static_cast<float>(i), 1.f, 0.f, static_cast<float>(i), 0.f, 1.f, static_cast<float>(i)};
		for (int j = 0; j < 9; ++j)
			mesh.vertices[i * 9 + j] = triangle[j];
		for (int j = 0; j < 3; ++j) {
			mesh.normals[i * 9 + j * 3 + 0] = 0.0f;
			mesh.normals[i * 9 + j * 3 + 1] = 0.0f;
			mesh.normals[i * 9 + j * 3 + 2] = -1.0f;
		}
	}
	for (unsigned short ix = 0; ix < n * 3; ++ix)
		mesh.indices[ix] = ix;
}

namespace DentalLib {
	void split_words(const std::string& line, std::vector<std::string>& words, char delim) {
        std::stringstream ss(line);
        std::string word = "";
        while (std::getline(ss, word, delim)) {
            words.emplace_back(word);
        }
    }

	void loadOBJ(const std::string& filename, Mesh& mesh, int& status) {
		// new mesh
		mesh = Mesh{0};

		std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: cannot open file " << filename << std::endl;
            status = 1; // error: can't open file
            return;
        }

        std::vector<glm::vec3> vertices;
		std::vector<glm::u16vec3> faces;

        std::string line;

        try {
			while (getline(file, line)) {
				if (line.empty()) {
					continue;
				}
				if (line[0] == '#') {
					continue;
				}
				std::vector<std::string> words;
				split_words(line, words, ' ');

				if (words[0] == "v") {
					vertices.emplace_back(glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
				}
				else if (words[0] == "f") {
					faces.emplace_back(glm::u16vec3(
						uint16_t(std::stoi(words[1]) - 1),
						uint16_t(std::stoi(words[2]) - 1),
						uint16_t(std::stoi(words[3]) - 1)
					));
				}
			}
            status = 0;
        }
        catch (std::exception& e) {
            std::cerr << "load mesh err: " << e.what() << std::endl;
			status = 2; // error: process failed
        }

        file.close();

		// transfer to raylib mesh
		mesh.triangleCount = faces.size();
		mesh.vertexCount = vertices.size();
		mesh.vertices = new float[mesh.vertexCount * 3];
		mesh.indices = new uint16_t[mesh.triangleCount * 3];
		std::memcpy(mesh.vertices, vertices.data(), mesh.vertexCount * sizeof(glm::vec3));
		std::memcpy(mesh.indices, faces.data(), mesh.triangleCount * sizeof(glm::u16vec3));
	}
}

int main() {
	Mesh mesh {};
	Model model;

	InitWindow(1200, 1000, "Dynamic Mesh test");
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	// SetTargetFPS(144);
	rlDisableBackfaceCulling();
	rlEnableSmoothLines();
	
    rlImGuiSetup(true);
    DentalGUI::set_custom_imgui_style();
	
	Camera camera = {{5.0f, 5.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, 0};

	// makeTriangleMesh(mesh, numTriangles); // start with 1 triangle
	int status = 0;
	DentalLib::loadOBJ("../tooth_morphology/datasets/11_Outside/n3_kongqi/n3.obj", mesh, status);
	UploadMesh(&mesh, true);
	model = LoadModelFromMesh(mesh);

    BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);
    bool selected = false;
    // Load basic lighting shader
    Shader shader = LoadShader("resources/shaders/raylib/lighting.vs",
                               "resources/shaders/raylib/lighting.fs");
    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    // NOTE: "matModel" location name is automatically assigned on shader
    // loading, no need to get the location again if using that uniform name
    // shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader,
    // "matModel");

    int ambientLoc = GetShaderLocation(shader, "ambient");
	float ambientVal[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    SetShaderValue(shader, ambientLoc, ambientVal, SHADER_UNIFORM_VEC4);

    // Assign out lighting shader to model
    model.materials[0].shader = shader;

    Light light = CreateLight(LIGHT_POINT, Vector3{-2, 10, -2}, Vector3Zero(), YELLOW, shader);

	while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        float cameraPos[3] = {camera.position.x, camera.position.y, camera.position.z};
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(shader, light);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Check collision between ray and box
			
            if (GetRayCollisionBox(GetMouseRay(GetMousePosition(), camera), bounds).hit)
                selected = !selected;
            else
                selected = false;
        }

		BeginDrawing();
		ClearBackground(RAYWHITE);

		rlImGuiBegin();

		{ // imgui
			ImGui::Begin("MeshViewer");
			ImGui::Button("All meshes created:");
			ImGui::End();
		}

		BeginMode3D(camera);
			DrawModelWires(model, {0.0f, 0.0f, 0.0f}, 1.0f, BLUE);
			DrawGrid(50, 1.0);

		EndMode3D();

		DrawFPS(10, 10);

		rlImGuiEnd();
		EndDrawing();
	}

	UnloadModel(model);

    rlImGuiShutdown();
	CloseWindow();

	return 0;
}