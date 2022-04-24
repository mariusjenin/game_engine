//
// Created by mariusjenin on 07/03/2022.
//

#include "SpotLight.hpp"

using namespace light;

void SpotLight::to_light_shader(LightShader *light_shader) {
    Light::to_light_shader(light_shader);
    light_shader->type = LIGHT_TYPE_SPOT;
}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction,
                     float inner_cut_off_angle,
                     float outer_cut_off_angle, float constant_attenuation, float linear_attenuation,
                     float quadratic_attenuation) : Light(ambient,
                                                          diffuse,
                                                          specular) {

    m_light_behaviors.push_back(new PositionLightBehavior(constant_attenuation,linear_attenuation,quadratic_attenuation));
    m_light_behaviors.push_back(new DirectionLightBehavior(direction));
    m_light_behaviors.push_back(new SpotLightBehavior(glm::cos(glm::radians(inner_cut_off_angle)),glm::cos(glm::radians(outer_cut_off_angle))));
}

bool SpotLight::positionned_light() {
    return true;
}