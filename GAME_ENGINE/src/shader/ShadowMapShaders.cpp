//
// Created by mariusjenin on 09/05/2022.
//

#include "ShadowMapShaders.hpp"

using namespace shader;

ShadowMapShaders::ShadowMapShaders(const char *vertex_file_path, const char *fragment_file_path) : Shaders(
        vertex_file_path, fragment_file_path) {}

void ShadowMapShaders::load_location() const {
    m_shader_data_manager->load_shadow_map_matrix_location(m_program_id);
    m_shader_data_manager->load_matrices_locations(m_program_id);
}
