//
// Created by mariusjenin on 01/03/2022.
//

#include "RootSG.hpp"

#include <utility>

using namespace scene_graph;

glm::mat4 RootSG::get_matrix_recursive(bool inverse) {
    if(inverse) {
        return m_trsf->get_inverse();
    }else{
        return m_trsf->get_matrix();
    }
}

RootSG::RootSG(Shaders *shaders) : ElementSG(shaders) {

}
