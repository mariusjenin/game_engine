//
// Created by mariusjenin on 05/03/2022.
//

#include "LightNodeSG.hpp"

#include <utility>

using namespace shader;
using namespace scene_graph;

LightNodeSG::LightNodeSG(Shaders *shaders, ElementSG *parent, Light *light, std::string name)
        : NodeSG(shaders, parent, std::move(name)) {
    m_light = light;
}

LightShader LightNodeSG::generate_light_struct() {
    LightShader light_struct{};
    m_light->to_light_shader(&light_struct);

    if (m_light->positionned_light()) {
        Transform light_trsf_tmp = Transform();
        light_trsf_tmp.set_matrix(get_matrix_recursive_intern());
        glm::vec3 light_position_tmp = {0, 0, 0};
        light_position_tmp = light_trsf_tmp.apply_to_point(light_position_tmp);
        light_struct.position = glsl_vec3(light_position_tmp);
    }
    return light_struct;
}