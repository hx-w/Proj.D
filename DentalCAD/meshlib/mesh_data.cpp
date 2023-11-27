//
// Created by carol on 2023/11/18.
//


#include "meshlib/mesh_data.h"

namespace DentalLib {
    DlMesh::DlMesh(Model model, RenderMode rmode) :
            _drawable(model), _rmode(rmode) {}

    DlMesh::DlMesh(DlMesh &&dlmesh) noexcept:
            _drawable(dlmesh._drawable), _rmode(dlmesh._rmode) {}

    DlMesh::~DlMesh() {
        UnloadModel(_drawable);
    };

    void DlMesh::set_shader(Shader shader) {
        _drawable.materials[0].shader = shader;
    }

    void DlMesh::draw_call() {
        // a simple approach
        /// [TODO] consider data consistency further, get a lock?
        /// [TODO] get a Model Transform changeable

        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        float m_scale = {1.0f};

        Vector3 m_position = *reinterpret_cast<Vector3 *>(&position); // type punning: glm::vec3 -> Vector3

        switch (_rmode) {
            case RenderMode::WIREFRAME:
                DrawModelWires(_drawable, m_position, m_scale, BLUE);
                break;
            case RenderMode::SOLID:
                DrawModel(_drawable, m_position, m_scale, BLUE);
                break;
            default:
                /* not expected rende rmode */
                break;
        }
    }

}
