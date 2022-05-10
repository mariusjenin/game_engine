//
// Created by mariusjenin on 24/04/2022.
//

#include "DirectionLightBehavior.hpp"

using namespace light;
using namespace light::behavior;

DirectionLightBehavior::DirectionLightBehavior() = default;

void DirectionLightBehavior::apply_to(LightInfo *light_info, glm::mat4 model_mat) {
    Transform light_trsf_tmp = Transform();
    light_trsf_tmp.set_matrix(model_mat);
    glm::vec3 light_position_tmp = NODE_INIT_FORWARD;
    light_position_tmp = light_trsf_tmp.apply_to_versor(light_position_tmp);
    light_info->direction = light_position_tmp;
}
