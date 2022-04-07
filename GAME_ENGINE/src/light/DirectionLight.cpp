//
// Created by mariusjenin on 07/03/2022.
//

#include "DirectionLight.hpp"

#include <utility>

using namespace light;

void DirectionLight::to_light_shader(LightShader *light_shader) {
    Light::to_light_shader(light_shader);
    light_shader->type = LIGHT_TYPE_DIRECTIONAL;
    light_shader->direction = glsl_vec3(m_direction);
}

DirectionLight::DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                               glm::vec3 direction) : Light(ambient, diffuse, specular) {
    m_direction = direction;
}
