//
// Created by mariusjenin on 07/03/2022.
//

#include "Light.hpp"

#include <utility>

using namespace light;

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_light_behaviors = {};
}

void Light::to_light_shader(LightShader *light_shader) {
    light_shader->ambient = glsl_vec3(m_ambient);
    light_shader->diffuse = glsl_vec3(m_diffuse);
    light_shader->specular = glsl_vec3(m_specular);
    for(auto & light_behavior : m_light_behaviors){
        light_behavior->apply_to(light_shader);
    }
}

bool Light::positionned_light() {
    return false;
}
