
#include "PositionLight.hpp"

#include <utility>

using namespace light;

void PositionLight::to_light_info(LightInfo *light_info, glm::mat4 model_mat) {
    Light::to_light_info(light_info, model_mat);
    light_info->type = LIGHT_TYPE_POINT;
}

PositionLight::PositionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                             float constant_attenuation, float linear_attenuation, float quadratic_attenuation) : Light(
        ambient, diffuse, specular) {
    m_light_behaviors.push_back(new PositionLightBehavior(constant_attenuation,linear_attenuation,quadratic_attenuation));
}
