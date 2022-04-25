//
// Created by mariusjenin on 01/03/2022.
//

#include "RootSG.hpp"

#include <utility>

using namespace scene_graph;

glm::mat4 RootSG::get_matrix_recursive() {
    glm::mat4 mat = glm::mat4(1.f);
    mat *= m_trsf->get_matrix();
    return mat;
}

void RootSG::draw(glm::vec3 pos_camera) {
    ElementSG::draw(pos_camera);
}

RootSG::RootSG(Shaders *shaders) : ElementSG(shaders) {

}
