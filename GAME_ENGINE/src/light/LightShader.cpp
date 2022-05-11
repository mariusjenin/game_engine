//
// Created by mariusjenin on 09/05/2022.
//

#include "src/light/LightShader.hpp"

void LightInfo::load_depth_vp_matrix(Shaders* shaders) {
    glUniformMatrix4fv(shaders->get_shader_data_manager()->get_location(ShadersDataManager::SHADOW_MAP_DEPTH_VP_MAT_LOC_NAME), 1, GL_FALSE, &depth_vp_mat[0][0]);
}

LightShader::LightShader(LightInfo li) {
    type = li.type;
    ambient = glsl_vec3(li.ambient);
    diffuse = glsl_vec3(li.diffuse);
    specular = glsl_vec3(li.specular);
    position = glsl_vec3(li.position);
    constant_attenuation = li.constant_attenuation;
    linear_attenuation = li.linear_attenuation;
    quadratic_attenuation = li.quadratic_attenuation;
    direction = glsl_vec3(li.direction);
    inner_cut_off = li.inner_cut_off;
    outer_cut_off = li.outer_cut_off;
    generate_depth_map = li.generate_depth_map;
//    bias_depth_map = li.bias_depth_map;
    depth_vp_mat = glsl_mat4(li.depth_vp_mat);
}

LightShader::LightShader() = default;
