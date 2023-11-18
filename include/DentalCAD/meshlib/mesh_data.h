//
// Created by carol on 2023/11/18.
//

#ifndef DENTALCAD_MESH_DATA_H
#define DENTALCAD_MESH_DATA_H

#include <vector>
#include <glm/glm.hpp>

struct Model;  // raylib model
struct Shader; // raylib shader
namespace DentalLib {
    enum class RenderMode {
        WIREFRAME = 0x00,
        SOLID     = 0x01
    };

    class DlMesh {
    public:
        explicit DlMesh(Model /* model */, RenderMode /* rmode */= RenderMode::SOLID);

        DlMesh(const DlMesh & /* dlmesh */) = default;

        DlMesh(DlMesh && /* dlmesh */) noexcept;

        ~DlMesh();

        void set_shader(Shader /* shader */);

        /**
         * called in each render loop to draw a dlmesh
         * called between `BeginMode3D` and `EndMode3D`
         */
        void draw_call();

    private:
        Model _drawable;  // raylib drawable

        // attributes
        RenderMode _rmode;
    };

}

#endif //DENTALCAD_MESH_DATA_H
