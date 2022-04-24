//
// Created by mariusjenin on 24/04/2022.
//

#include "DirectionLightBehavior.hpp"

using namespace light;

DirectionLightBehavior::DirectionLightBehavior(glm::vec3 direction) {
    m_direction = direction;
}

void DirectionLightBehavior::apply_to(LightShader *light_shader) {
    light_shader->direction = glsl_vec3(m_direction);
}
