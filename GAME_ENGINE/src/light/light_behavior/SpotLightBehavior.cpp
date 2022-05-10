//
// Created by mariusjenin on 24/04/2022.
//

#include "SpotLightBehavior.hpp"
#include <src/utils/printer.hpp>

using namespace light;
using namespace light::behavior;

SpotLightBehavior::SpotLightBehavior(GLuint id_texture_shadow_map, float icoa, float ocoa, int resol, float z_near, float z_far) {
    m_inner_cut_off = glm::cos(glm::radians(icoa));
    m_outer_cut_off = glm::cos(glm::radians(ocoa));
    m_cut_off_angle = std::fmax(icoa,ocoa);
    m_id_texture_shadow_map = id_texture_shadow_map;
    m_z_near = z_near;
    m_z_far = z_far;
    m_shadow_map = new ShadowMap(resol,resol,id_texture_shadow_map);
}

void SpotLightBehavior::apply_to(LightInfo *light_info, glm::mat4 model_mat) {
    light_info->inner_cut_off = m_inner_cut_off;
    light_info->outer_cut_off = m_outer_cut_off;

    light_info->generate_shadow_map = true;
    light_info->shadow_map = m_shadow_map;
    light_info->depth_map = (int) m_id_texture_shadow_map;
    Transform light_trsf_tmp = Transform();
    light_trsf_tmp.set_matrix(model_mat);

    glm::vec3 light_position_tmp = NODE_INIT_POSITION;
    glm::vec3 light_direction_tmp = NODE_INIT_FORWARD;
    glm::vec3 light_up_tmp = NODE_INIT_UP;
    glm::vec3 eye_camera = light_trsf_tmp.apply_to_point(light_position_tmp);
    glm::vec3 dir_camera = light_trsf_tmp.apply_to_versor(light_direction_tmp);
    glm::vec3 up_camera = light_trsf_tmp.apply_to_versor(light_up_tmp);
    glm::mat4 view_mat = glm::lookAt(eye_camera,eye_camera + dir_camera,up_camera);

    glm::mat4 proj_mat = glm::perspective<float>(2*glm::radians(m_cut_off_angle),1.,m_z_near,m_z_far);

    light_info->depth_vp_mat = proj_mat * view_mat;
}
