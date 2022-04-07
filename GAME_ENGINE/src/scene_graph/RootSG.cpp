//
// Created by mariusjenin on 01/03/2022.
//

#include "RootSG.hpp"

#include <utility>

using namespace scene_graph;

glm::mat4 RootSG::get_matrix_recursive_extern() {
    glm::mat4 mat = glm::mat4(1.f);
    for (auto trsf: m_trsfs_general) {
        mat *= trsf->get_matrix();
    }
    return mat;
}

void RootSG::draw(glm::vec3 pos_camera) {
    ElementSG::draw(pos_camera);
}

RootSG::RootSG(Shaders *shaders, std::string name) : ElementSG(shaders, std::move(name)) {

}
