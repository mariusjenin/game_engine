//
// Created by mariusjenin on 07/03/2022.
//

#include "DirectionLight.hpp"

#include <utility>

using namespace light;
using namespace light::behavior;

void DirectionLight::to_light_info(LightInfo *light_info, glm::mat4 model_mat) {
    Light::to_light_info(light_info, model_mat);
    light_info->type = LIGHT_TYPE_DIRECTIONAL;
}

DirectionLight::DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(ambient, diffuse, specular) {

    m_light_behaviors.push_back(new DirectionLightBehavior());
}
