//
// Created by mariusjenin on 07/03/2022.
//

#include "SpotLight.hpp"

using namespace light;

void SpotLight::to_light_info(LightInfo *light_info, glm::mat4 model_mat) {
    Light::to_light_info(light_info, model_mat);
    light_info->type = LIGHT_TYPE_SPOT;
}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLuint id_texture_shadow_map,
                     float inner_cut_off_angle, float outer_cut_off_angle, int resol, float z_near, float z_far,
                     float bias, float constant_attenuation, float linear_attenuation,
                     float quadratic_attenuation) : Light(ambient,
                                                          diffuse,
                                                          specular) {

    inner_cut_off_angle = (float) std::fmin(inner_cut_off_angle, 90.f);
    outer_cut_off_angle = (float) std::fmin(outer_cut_off_angle, 90.f);
    m_light_behaviors.push_back(
            new PositionLightBehavior(constant_attenuation, linear_attenuation, quadratic_attenuation));
    m_light_behaviors.push_back(new DirectionLightBehavior());
    m_light_behaviors.push_back(
            new SpotLightBehavior(id_texture_shadow_map, inner_cut_off_angle, outer_cut_off_angle, resol, z_near,
                                  z_far,bias));
}
