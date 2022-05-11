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

void Light::to_light_info(LightInfo *light_info, glm::mat4 model_mat) {
    light_info->ambient = m_ambient;
    light_info->diffuse = m_diffuse;
    light_info->specular = m_specular;
    light_info->generate_depth_map = false;
    for(auto & light_behavior : m_light_behaviors){
        light_behavior->apply_to(light_info,model_mat);
    }
}
