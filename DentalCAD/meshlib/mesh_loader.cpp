//
// Created by carol on 2023/11/18.
//

#include <fstream>
#include <raylib.h>
#include <glm/glm.hpp>

#include "meshlib/mesh_data.h"
#include "meshlib/mesh_loader.h"

namespace DentalLib {
    using MshT = MeshLoadStatus;
    using std::vector;
    using std::string;

    const string WHITESPACE = " \n\r\t\f\v"; // used in string_strip

    vector<string> MeshLoader::string_split(const string &str, const string &pattern) {
        if (str.empty()) {
            return vector<string>{};
        }
        vector<string> res;
        string strs = str + pattern;
        size_t pos = strs.find(pattern);

        while (pos != string::npos) {
            string temp = strs.substr(0, pos);
            res.emplace_back(temp);
            strs = strs.substr(pos + 1, strs.size());
            pos = strs.find(pattern);
        }

        return res;
    }

    string MeshLoader::string_strip(std::string &str, bool inplace) {
        if (str.empty()) return "";

        if (inplace) {
            str.erase(str.find_last_not_of(WHITESPACE) + 1);
            str.erase(0, str.find_first_not_of(WHITESPACE));
            return "";
        }
        else {
            size_t start = str.find_first_not_of(WHITESPACE);
            string res = (start == string::npos) ? "" :  str.substr(start);
            size_t end = res.find_last_not_of(WHITESPACE);
            res = (end == string::npos) ? "" : res.substr(0, end + 1);
            return res;
        }
    }

    Ptr_DlMesh MeshLoader::load(const std::string &filename, MshT &status) {
        if (!FileExists(filename.c_str())) {
            status = MshT::FileNotFound;
            return nullptr;
        }

        // check file extension and process in different methods
        if (IsFileExtension(filename.c_str(), ".obj")) {
            return ObjLoader::load(filename, status);
        } else if (IsFileExtension(filename.c_str(), ".ply")) {
            return PlyLoader::load(filename, status);
        } else {
            status = MeshLoadStatus::FileExtErr;
            return nullptr;
        }
    }

    Ptr_DlMesh ObjLoader::load(const std::string &filename, MshT &status) {
        if (!FileExists(filename.c_str())) {
            status = MshT::FileNotFound;
            return nullptr;
        }

        std::ifstream file(filename);
        if (!file.is_open()) {
            status = MshT::FileOpenErr;
            return nullptr;
        }

        vector<glm::vec3> vertices;
        vector<glm::u16vec3> faces;
        string buffer;

        try {
            // if error occurs, return empty mesh
            while (std::getline(file, buffer)) {
                string_strip(buffer); // trimmed string
                if (buffer.empty()) continue;
                if (buffer[0] == '#') continue;

                auto splitted = string_split(buffer, " ");

                /// for a simple approach, only consider [v|f]
                if (splitted.size() != 4) continue;

                if (splitted[0] == "v") {
                    vertices.emplace_back(glm::vec3{
                        std::stof(splitted[1]),
                        std::stof(splitted[2]),
                        std::stof(splitted[3])
                    });
                }
                else if (splitted[0] == "f") {
                    faces.emplace_back(glm::u16vec3{
                        uint16_t(std::stoi(splitted[1]) - 1),
                        uint16_t(std::stoi(splitted[2]) - 1),
                        uint16_t(std::stoi(splitted[3]) - 1)
                    });
                }
                else {
                    // unimplemented tag
                }
            }
            file.close();
        }
        catch (std::exception& e) {
            file.close();
            status = MshT::UnexpectedErr;
            return nullptr;
        }

        // construct normal, not very good :(
//        vector<glm::vec3> normals(vertices.size(), glm::vec3{0.0f});
//        for (auto& f : faces) {
//            auto& v1 = vertices[f[0]];
//            auto& v2 = vertices[f[1]];
//            auto& v3 = vertices[f[2]];
//
//            auto nml = glm::normalize(glm::cross(v2 - v1, v3 - v1));
//            normals[f[0]] += nml;
//            normals[f[1]] += nml;
//            normals[f[2]] += nml;
//        }
//        for (auto& nml : normals) {
//            nml = glm::normalize(nml);
//        }

        // construct raylib mesh -> model
        Mesh mesh{};
        mesh.triangleCount = static_cast<int>(faces.size());
        mesh.vertexCount = static_cast<int>(vertices.size());
        mesh.vertices = new float[mesh.vertexCount * 3];
//        mesh.normals = new float[mesh.vertexCount];
        mesh.indices = new uint16_t[mesh.triangleCount * 3];


        std::memcpy(mesh.vertices, vertices.data(), mesh.vertexCount * 3 * sizeof(float));
//        std::memcpy(mesh.normals, normals.data(), mesh.vertexCount * sizeof(glm::vec3));
        std::memcpy(mesh.indices, faces.data(), mesh.triangleCount * 3 * sizeof(uint16_t));
        UploadMesh(&mesh, true);   // init vao & vbo

        status = MshT::Success;
        auto ptr = std::make_shared<DlMesh>(LoadModelFromMesh(mesh));
        return ptr;
    }

    Ptr_DlMesh PlyLoader::load(const std::string &filename, MshT &status) {
        /// [TODO] ply file loader
        status = MshT::UnImplErr;
        return nullptr;
    }
}
