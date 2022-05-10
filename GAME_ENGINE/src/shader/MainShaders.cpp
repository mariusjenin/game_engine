//
// Created by mariusjenin on 09/05/2022.
//

#include "MainShaders.hpp"

using namespace shader;

MainShaders::MainShaders(const char *vertex_file_path, const char *fragment_file_path) : Shaders(
        vertex_file_path, fragment_file_path) {
    m_shadow_map_shaders = new ShadowMapShaders("../shader/depth_shader/depth_vertex_shader.glsl","../shader/depth_shader/depth_fragment_shader.glsl");
}


void MainShaders::load_location() const {
    m_shader_data_manager->load_matrices_locations(m_program_id);
    m_shader_data_manager->load_view_pos_location(m_program_id);
    m_shader_data_manager->load_node_on_top_locations(m_program_id);
    m_shader_data_manager->load_lights_const(m_program_id);
    m_shader_data_manager->load_material_const(m_program_id);
    m_shader_data_manager->load_material_locations(m_program_id);
    m_shader_data_manager->load_lights_locations(m_program_id);
    m_shader_data_manager->load_shadow_maps_location(m_program_id);
}

ShadowMapShaders *MainShaders::get_shadow_map_shaders() const {
    return m_shadow_map_shaders;
}
