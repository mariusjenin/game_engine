
#include "PositionLight.hpp"

#include <utility>

using namespace light;

void PositionLight::to_light_shader(LightShader *light_shader) {
    Light::to_light_shader(light_shader);
    light_shader->type = LIGHT_TYPE_POINT;
}

PositionLight::PositionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                             float constant_attenuation, float linear_attenuation, float quadratic_attenuation) : Light(
        ambient, diffuse, specular) {
    m_light_behaviors.push_back(new PositionLightBehavior(constant_attenuation,linear_attenuation,quadratic_attenuation));
}

bool PositionLight::positionned_light() {
    return true;
}