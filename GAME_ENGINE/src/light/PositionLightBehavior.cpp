//
// Created by mariusjenin on 24/04/2022.
//

#include "PositionLightBehavior.hpp"

using namespace light;

PositionLightBehavior::PositionLightBehavior(float ca, float la, float qa) {
    m_constant_attenuation = ca;
    m_linear_attenuation = la;
    m_quadratic_attenuation = qa;
}

void PositionLightBehavior::apply_to(LightShader *light_shader) {
    light_shader->constant_attenuation = m_constant_attenuation;
    light_shader->linear_attenuation = m_linear_attenuation;
    light_shader->quadratic_attenuation = m_quadratic_attenuation;
}
