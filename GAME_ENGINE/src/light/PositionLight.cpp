//
// Created by mariusjenin on 07/03/2022.
//

#include "PositionLight.hpp"

#include <utility>

using namespace light;

void PositionLight::to_light_shader(LightShader *light_shader) {
    Light::to_light_shader(light_shader);
    light_shader->type = LIGHT_TYPE_POINT;
    light_shader->constant_attenuation = m_constant_attenuation;
    light_shader->linear_attenuation = m_linear_attenuation;
    light_shader->quadratic_attenuation = m_quadratic_attenuation;
}

PositionLight::PositionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                             float constant_attenuation, float linear_attenuation, float quadratic_attenuation) : Light(
        ambient, diffuse, specular) {
    m_constant_attenuation = constant_attenuation;
    m_linear_attenuation = linear_attenuation;
    m_quadratic_attenuation = quadratic_attenuation;
}

bool PositionLight::positionned_light() {
    return true;
}