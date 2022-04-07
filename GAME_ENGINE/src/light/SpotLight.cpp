//
// Created by mariusjenin on 07/03/2022.
//

#include "SpotLight.hpp"

using namespace light;

void SpotLight::to_light_shader(LightShader *light_shader) {
    DirectionLight::to_light_shader(light_shader);
    PositionLight::to_light_shader(light_shader);
    light_shader->type = LIGHT_TYPE_SPOT;
    light_shader->inner_cut_off = m_inner_cut_off;
    light_shader->outer_cut_off = m_outer_cut_off;
}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction,
                     float inner_cut_off_angle,
                     float outer_cut_off_angle, float constant_attenuation, float linear_attenuation,
                     float quadratic_attenuation) : Light(ambient,
                                                          diffuse,
                                                          specular),
                                                    DirectionLight(
                                                            ambient,
                                                            diffuse,
                                                            specular,
                                                            direction),
                                                    PositionLight(
                                                            ambient,
                                                            diffuse,
                                                            specular,
                                                            constant_attenuation,
                                                            linear_attenuation,
                                                            quadratic_attenuation) {
    m_inner_cut_off = glm::cos(glm::radians(inner_cut_off_angle));
    m_outer_cut_off = glm::cos(glm::radians(outer_cut_off_angle));
}

bool SpotLight::positionned_light() {
    return PositionLight::positionned_light();
}