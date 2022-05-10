//
// Created by mariusjenin on 24/04/2022.
//

#include "PositionLightBehavior.hpp"

using namespace light;
using namespace light::behavior;

PositionLightBehavior::PositionLightBehavior(float ca, float la, float qa) {
    m_constant_attenuation = ca;
    m_linear_attenuation = la;
    m_quadratic_attenuation = qa;
}

void PositionLightBehavior::apply_to(LightInfo *light_info, glm::mat4 model_mat) {
    light_info->constant_attenuation = m_constant_attenuation;
    light_info->linear_attenuation = m_linear_attenuation;
    light_info->quadratic_attenuation = m_quadratic_attenuation;

    Transform light_trsf_tmp = Transform();
    light_trsf_tmp.set_matrix(model_mat);
    glm::vec3 light_position_tmp = NODE_INIT_POSITION;
    light_position_tmp = light_trsf_tmp.apply_to_point(light_position_tmp);
    light_info->position = light_position_tmp;
}
